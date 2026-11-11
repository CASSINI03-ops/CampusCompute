
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <errno.h>
#include "ipc.h"

#define MESSAGE_SIZE 128

typedef struct
{
    char message[MESSAGE_SIZE];
} SharedData;

void run_ipc_demo(void)
{
    char shm_name[64];
    char sem_name[64];

    snprintf(shm_name, sizeof(shm_name),
             "/campuscompute_shm_%ld", (long)getpid());

    snprintf(sem_name, sizeof(sem_name),
             "/campuscompute_sem_%ld", (long)getpid());

    printf("\n--- Shared Memory IPC Demo ---\n");

    int shm_fd = shm_open(shm_name, O_CREAT | O_EXCL | O_RDWR, 0600);

    if (shm_fd == -1)
    {
        perror("shm_open");
        return;
    }

    if (ftruncate(shm_fd, sizeof(SharedData)) == -1)
    {
        perror("ftruncate");
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }

    SharedData *shared = mmap(NULL, sizeof(SharedData),
                              PROT_READ | PROT_WRITE,
                              MAP_SHARED, shm_fd, 0);

    if (shared == MAP_FAILED)
    {
        perror("mmap");
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }

    sem_t *semaphore = sem_open(sem_name, O_CREAT | O_EXCL, 0600, 0);

    if (semaphore == SEM_FAILED)
    {
        perror("sem_open");
        munmap(shared, sizeof(SharedData));
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        sem_close(semaphore);
        sem_unlink(sem_name);
        munmap(shared, sizeof(SharedData));
        close(shm_fd);
        shm_unlink(shm_name);
        return;
    }

    if (pid == 0)
    {
        printf("[Child] Waiting for parent message...\n");

        if (sem_wait(semaphore) == -1)
        {
            perror("[Child] sem_wait");
            _exit(EXIT_FAILURE);
        }

        printf("[Child] Received: %s\n", shared->message);
        printf("[Child] Message received successfully.\n");

        munmap(shared, sizeof(SharedData));
        close(shm_fd);
        sem_close(semaphore);

        _exit(EXIT_SUCCESS);
    }
    else
    {
        snprintf(shared->message, MESSAGE_SIZE,
                 "Hello from parent process! Job data transferred.");

        printf("[Parent] Writing message to shared memory.\n");

        if (sem_post(semaphore) == -1)
            perror("[Parent] sem_post");

        int status;

        if (waitpid(pid, &status, 0) == -1)
            perror("waitpid");
        else if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
            printf("[Parent] Child completed successfully.\n");
        else
            printf("[Parent] Child did not complete successfully.\n");

        sem_close(semaphore);
        sem_unlink(sem_name);

        munmap(shared, sizeof(SharedData));
        close(shm_fd);
        shm_unlink(shm_name);

        printf("[Parent] Shared memory and semaphore cleaned up.\n");
    }
}
