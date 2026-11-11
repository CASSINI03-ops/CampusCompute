
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "worker.h"

static void run_worker(const char *name, int simulate_failure)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        printf("[%s] Worker started. PID: %d\n",
               name, getpid());

        if (simulate_failure)
        {
            printf("[%s] Simulating worker failure...\n", name);
            fflush(stdout);
            _exit(1);
        }

        printf("[%s] Work completed successfully.\n", name);
        fflush(stdout);
        _exit(0);
    }

    int status;

    if (waitpid(pid, &status, 0) == -1)
    {
        perror("waitpid");
        return;
    }

    if (WIFEXITED(status))
    {
        int exit_code = WEXITSTATUS(status);

        if (exit_code == 0)
        {
            printf("[Parent] %s completed normally. Exit code: 0\n",
                   name);
        }
        else
        {
            printf("[Parent] %s failed. Exit code: %d\n",
                   name, exit_code);
        }
    }
    else if (WIFSIGNALED(status))
    {
        printf("[Parent] %s terminated by signal: %d\n",
               name, WTERMSIG(status));
    }
}

void run_worker_demo(void)
{
    printf("\n--- Worker Failure Detection ---\n");

    run_worker("Normal Worker", 0);

    printf("\n");

    run_worker("Failed Worker", 1);
}
