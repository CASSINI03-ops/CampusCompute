
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "jobs.h"
#include "scheduling.h"
#include "logger.h"
#include "ipc.h"
#include "banker.h"
#include "worker.h"
#include "resource.h"

static int read_integer(const char *prompt, int *value)
{
    char input[128];
    char *end;

    printf("%s", prompt);

    if (fgets(input, sizeof(input), stdin) == NULL)
        return 0;

    errno = 0;
    long number = strtol(input, &end, 10);

    if (end == input || errno == ERANGE ||
        number < -2147483648L || number > 2147483647L)
        return -1;

    while (*end == ' ' || *end == '\t')
        end++;

    if (*end != '\n' && *end != '\0')
        return -1;

    *value = (int)number;
    return 1;
}

static void show_help(void)
{
    printf("\n========== CAMPUSCOMPUTE HELP ==========\n");
    printf("1. Add Job\n");
    printf("2. Display Jobs\n");
    printf("3. FCFS Scheduling\n");
    printf("4. Round Robin Scheduling\n");
    printf("5. Priority Scheduling\n");
    printf("6. Scheduling Comparison\n");
    printf("7. Concurrent Activity Logger\n");
    printf("8. Shared Memory IPC\n");
    printf("9. Banker's Algorithm\n");
    printf("10. Worker Failure Detection\n");
    printf("11. Resource Limit Demonstration\n");
    printf("12. Update Job Status\n");
    printf("0. Exit\n");
    printf("========================================\n");
}

static void run_comparison(void)
{
    printf("\n========== SCHEDULING COMPARISON ==========\n");

    printf("\n--- FCFS ---\n");
    run_fcfs();

    printf("\n--- Round Robin (Quantum 2) ---\n");
    run_round_robin(2);

    printf("\n--- Priority ---\n");
    run_priority();
}

int main(void)
{
    int choice;
    int result;

    printf("\n====================================\n");
    printf("       CAMPUSCOMPUTE SYSTEM\n");
    printf(" Intelligent Job Scheduling System\n");
    printf("====================================\n");

    show_help();

    while (1)
    {
        result = read_integer("\nEnter your choice: ", &choice);

        if (result == 0)
        {
            printf("\nInput ended. Exiting.\n");
            break;
        }

        if (result == -1)
        {
            printf("Invalid input. Enter a valid integer.\n");
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                int id, arrival, burst, priority;

                if (read_integer("Enter Job ID: ", &id) != 1 ||
                    read_integer("Enter Arrival Time: ", &arrival) != 1 ||
                    read_integer("Enter CPU Burst Time: ", &burst) != 1 ||
                    read_integer("Enter Priority: ", &priority) != 1)
                {
                    printf("Invalid job input. Please retry.\n");
                    break;
                }

                add_job(id, arrival, burst, priority);
                break;
            }

            case 2:
                display_jobs();
                break;

            case 3:
                run_fcfs();
                break;

            case 4:
            {
                int quantum;

                if (read_integer("Enter Time Quantum: ", &quantum) != 1)
                {
                    printf("Invalid time quantum.\n");
                    break;
                }

                run_round_robin(quantum);
                break;
            }

            case 5:
                run_priority();
                break;

            case 6:
                run_comparison();
                break;

            case 7:
                run_logger_demo();
                break;

            case 8:
                run_ipc_demo();
                break;

            case 9:
                run_banker_demo();
                break;

            case 10:
                run_worker_demo();
                break;

            case 11:
                run_resource_demo();
                break;

            case 12:
            {
                int id, status, completion;

                if (read_integer("Enter Job ID: ", &id) != 1 ||
                    read_integer("Status (0=WAITING, 1=RUNNING, 2=COMPLETED): ", &status) != 1 ||
                    status < 0 || status > 2)
                {
                    printf("Invalid status input.\n");
                    break;
                }

                completion = -1;

                if (status == COMPLETED)
                {
                    if (read_integer("Enter Completion Time: ", &completion) != 1)
                    {
                        printf("Invalid completion time.\n");
                        break;
                    }
                }

                update_job_status(id, (JobStatus)status, completion);
                break;
            }

            case 0:
                printf("CampusCompute shutting down.\n");
                return 0;

            default:
                printf("Invalid command. Enter 0-12.\n");
                show_help();
        }
    }

    return 0;
}

