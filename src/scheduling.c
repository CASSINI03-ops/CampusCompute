
#include <stdio.h>
#include <stdlib.h>
#include "scheduling.h"

#define MAX_JOBS 100

static void sort_by_arrival(Job p[], int order[], int n)
{
    for (int i = 0; i < n; i++)
        order[i] = i;

    for (int i = 1; i < n; i++)
    {
        int key = order[i];
        int j = i - 1;

        while (j >= 0 &&
               p[order[j]].arrival_time > p[key].arrival_time)
        {
            order[j + 1] = order[j];
            j--;
        }

        order[j + 1] = key;
    }
}

static void print_metrics(Job p[], int n)
{
    double total_waiting = 0;
    double total_turnaround = 0;

    printf("\nJob\tArrival\tBurst\tCompletion\tTurnaround\tWaiting\n");

    for (int i = 0; i < n; i++)
    {
        int turnaround = p[i].completion_time - p[i].arrival_time;
        int waiting = turnaround - p[i].burst_time;

        total_waiting += waiting;
        total_turnaround += turnaround;

        printf("J%d\t%d\t%d\t%d\t\t%d\t\t%d\n",
               p[i].job_id,
               p[i].arrival_time,
               p[i].burst_time,
               p[i].completion_time,
               turnaround,
               waiting);
    }

    printf("\nAverage Waiting Time: %.2f\n", total_waiting / n);
    printf("Average Turnaround Time: %.2f\n", total_turnaround / n);
}

void run_fcfs(void)
{
    int n = get_job_count();

    if (n == 0)
    {
        printf("No jobs available.\n");
        return;
    }

    Job p[MAX_JOBS];
    int order[MAX_JOBS];
    Job *original = get_jobs();

    for (int i = 0; i < n; i++)
        p[i] = original[i];

    sort_by_arrival(p, order, n);

    int time = 0;

    printf("\n--- FCFS Scheduling ---\nTimeline: ");

    for (int i = 0; i < n; i++)
    {
        int k = order[i];

        if (time < p[k].arrival_time)
        {
            printf("[Idle %d-%d] ",
                   time, p[k].arrival_time);
            time = p[k].arrival_time;
        }

        printf("[J%d %d-%d] ",
               p[k].job_id, time, time + p[k].burst_time);

        time += p[k].burst_time;
        p[k].completion_time = time;
    }

    printf("\n");
    print_metrics(p, n);
}

void run_priority(void)
{
    int n = get_job_count();

    if (n == 0)
    {
        printf("No jobs available.\n");
        return;
    }

    Job p[MAX_JOBS];
    int completed[MAX_JOBS] = {0};
    Job *original = get_jobs();

    for (int i = 0; i < n; i++)
        p[i] = original[i];

    int time = 0;
    int done = 0;

    printf("\n--- Non-preemptive Priority Scheduling ---\n");
    printf("Tie-breaking: Earlier arrival, then input order.\n");
    printf("Lower priority number means higher priority.\n");
    printf("Timeline: ");

    while (done < n)
    {
        int selected = -1;

        for (int i = 0; i < n; i++)
        {
            if (!completed[i] && p[i].arrival_time <= time)
            {
                if (selected == -1 ||
                    p[i].priority < p[selected].priority ||
                    (p[i].priority == p[selected].priority &&
                     p[i].arrival_time < p[selected].arrival_time))
                {
                    selected = i;
                }
            }
        }

        if (selected == -1)
        {
            int next = -1;

            for (int i = 0; i < n; i++)
            {
                if (!completed[i] &&
                    (next == -1 ||
                     p[i].arrival_time < p[next].arrival_time))
                    next = i;
            }

            printf("[Idle %d-%d] ", time, p[next].arrival_time);
            time = p[next].arrival_time;
            continue;
        }

        printf("[J%d %d-%d] ",
               p[selected].job_id,
               time,
               time + p[selected].burst_time);

        time += p[selected].burst_time;
        p[selected].completion_time = time;

        completed[selected] = 1;
        done++;
    }

    printf("\n");
    print_metrics(p, n);
}

void run_round_robin(int quantum)
{
    int n = get_job_count();

    if (n == 0 || quantum <= 0)
    {
        printf("No jobs available or invalid time quantum.\n");
        return;
    }

    Job p[MAX_JOBS];
    int order[MAX_JOBS];
    int queue[MAX_JOBS];

    Job *original = get_jobs();

    for (int i = 0; i < n; i++)
    {
        p[i] = original[i];
        p[i].remaining_time = p[i].burst_time;
    }

    sort_by_arrival(p, order, n);

    int front = 0, rear = 0, count = 0;
    int next = 0, done = 0, time = 0;

    printf("\n--- Round Robin Scheduling ---\n");
    printf("Time Quantum: %d\n", quantum);
    printf("Timeline: ");

    while (done < n)
    {
        while (next < n &&
               p[order[next]].arrival_time <= time)
        {
            queue[rear] = order[next];
            rear = (rear + 1) % MAX_JOBS;
            count++;
            next++;
        }

        if (count == 0)
        {
            printf("[Idle %d-%d] ",
                   time, p[order[next]].arrival_time);
            time = p[order[next]].arrival_time;
            continue;
        }

        int k = queue[front];
        front = (front + 1) % MAX_JOBS;
        count--;

        int start = time;
        int execution = p[k].remaining_time < quantum
                        ? p[k].remaining_time : quantum;

        time += execution;
        p[k].remaining_time -= execution;

        printf("[J%d %d-%d] ", p[k].job_id, start, time);

        while (next < n &&
               p[order[next]].arrival_time <= time)
        {
            queue[rear] = order[next];
            rear = (rear + 1) % MAX_JOBS;
            count++;
            next++;
        }

        if (p[k].remaining_time > 0)
        {
            queue[rear] = k;
            rear = (rear + 1) % MAX_JOBS;
            count++;
        }
        else
        {
            p[k].completion_time = time;
            done++;
        }
    }

    printf("\n");
    print_metrics(p, n);
}
