
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include "resource.h"

void run_resource_demo(void)
{
    printf("\n--- Process Resource Limit Demo ---\n");

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        struct rlimit limit;

        limit.rlim_cur = 32;
        limit.rlim_max = 32;

        if (setrlimit(RLIMIT_NOFILE, &limit) == -1)
        {
            perror("setrlimit");
            _exit(EXIT_FAILURE);
        }

        printf("[Child] RLIMIT_NOFILE soft limit: %lu\n",
               (unsigned long)limit.rlim_cur);

        int descriptors[64];
        int opened = 0;

        while (opened < 64)
        {
            int fd = open("/dev/null", O_RDONLY);

            if (fd == -1)
            {
                if (errno == EMFILE)
                    printf("[Child] Resource limit reached: Too many open files.\n");
                else
                    perror("[Child] open");

                break;
            }

            descriptors[opened++] = fd;
        }

        printf("[Child] Successfully opened %d additional descriptors.\n",
               opened);

        for (int i = 0; i < opened; i++)
            close(descriptors[i]);

        printf("[Child] All opened descriptors closed.\n");
        fflush(stdout);

        _exit(EXIT_SUCCESS);
    }

    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
        printf("[Parent] Resource-limit demonstration completed.\n");
    else
        printf("[Parent] Resource-limit demonstration failed.\n");
}
