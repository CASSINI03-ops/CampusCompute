
#include <stdio.h>
#include "banker.h"

#define PROCESSES 5
#define RESOURCES 3

static int check_safety(int available[],
                        int allocation[PROCESSES][RESOURCES],
                        int maximum[PROCESSES][RESOURCES])
{
    int need[PROCESSES][RESOURCES];
    int finish[PROCESSES] = {0};
    int work[RESOURCES];
    int safe_sequence[PROCESSES];
    int completed = 0;

    for (int j = 0; j < RESOURCES; j++)
        work[j] = available[j];

    for (int i = 0; i < PROCESSES; i++)
    {
        for (int j = 0; j < RESOURCES; j++)
        {
            need[i][j] = maximum[i][j] - allocation[i][j];

            if (need[i][j] < 0)
            {
                printf("Invalid resource allocation data.\n");
                return 0;
            }
        }
    }

    while (completed < PROCESSES)
    {
        int found = 0;

        for (int i = 0; i < PROCESSES; i++)
        {
            if (finish[i])
                continue;

            int possible = 1;

            for (int j = 0; j < RESOURCES; j++)
            {
                if (need[i][j] > work[j])
                {
                    possible = 0;
                    break;
                }
            }

            if (possible)
            {
                for (int j = 0; j < RESOURCES; j++)
                    work[j] += allocation[i][j];

                safe_sequence[completed] = i;
                completed++;
                finish[i] = 1;
                found = 1;
            }
        }

        if (!found)
            break;
    }

    if (completed == PROCESSES)
    {
        printf("System is in a SAFE state.\n");
        printf("Safe sequence: ");

        for (int i = 0; i < PROCESSES; i++)
        {
            printf("P%d", safe_sequence[i]);

            if (i < PROCESSES - 1)
                printf(" -> ");
        }

        printf("\n");
        return 1;
    }

    printf("System is in an UNSAFE state.\n");
    printf("No safe sequence exists.\n");

    return 0;
}

void run_banker_demo(void)
{
    int safe_available[RESOURCES] = {3, 3, 2};

    int safe_allocation[PROCESSES][RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int safe_maximum[PROCESSES][RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    int unsafe_available[RESOURCES] = {0, 0, 0};

    int unsafe_allocation[PROCESSES][RESOURCES] = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    int unsafe_maximum[PROCESSES][RESOURCES] = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    printf("\n--- Banker's Algorithm ---\n");

    printf("\nSAFE STATE TEST:\n");
    check_safety(safe_available,
                 safe_allocation, safe_maximum);

    printf("\nUNSAFE STATE TEST:\n");
    check_safety(unsafe_available,
                 unsafe_allocation, unsafe_maximum);
}
