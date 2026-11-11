#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jobs.h"

#define MAX_JOBS 100

static Job jobs[MAX_JOBS];
static int job_count = 0;
int add_job(int id, int arrival, int burst, int priority)
{
    if (job_count >= MAX_JOBS)
    {
        printf("Error: Maximum job limit reached.\n");
        return 0;
    }

    if (id <= 0 || arrival < 0 || burst <= 0 || priority < 0)
    {
        printf("Error: Invalid job details.\n");
        return 0;
    }

    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].job_id == id)
        {
            printf("Error: Job ID already exists.\n");
            return 0;
        }
    }

    jobs[job_count].job_id = id;
    jobs[job_count].arrival_time = arrival;
    jobs[job_count].burst_time = burst;
    jobs[job_count].priority = priority;
    jobs[job_count].remaining_time = burst;
    jobs[job_count].completion_time = -1;
    jobs[job_count].status = WAITING;

    job_count++;

    printf("Job J%d added successfully.\n", id);

    return 1;
}

void display_jobs(void)
{
    if (job_count == 0)
    {
        printf("No jobs registered.\n");
        return;
    }

    printf("\n--- Registered Jobs ---\n");

    printf("%-10s %-10s %-10s %-10s %-12s\n",
           "Job ID", "Arrival", "Burst", "Priority", "Status");

    for (int i = 0; i < job_count; i++)
    {
        char *status;

        switch (jobs[i].status)
        {
            case WAITING:
                status = "WAITING";
                break;

            case RUNNING:
                status = "RUNNING";
                break;

            case COMPLETED:
                status = "COMPLETED";
                break;

            default:
                status = "UNKNOWN";
        }

        printf("J%-9d %-10d %-10d %-10d %-12s\n",
               jobs[i].job_id,
               jobs[i].arrival_time,
               jobs[i].burst_time,
               jobs[i].priority,
               status);
    }
}


int add_job(int id, int arrival, int burst, int priority);

void display_jobs(void);

int update_job_status(int id, JobStatus status, int completion_time)
{
    for (int i = 0; i < job_count; i++)
    {
        if (jobs[i].job_id == id)
        {
            if (status == COMPLETED && completion_time < jobs[i].arrival_time + jobs[i].burst_time)
            {
                printf("Error: Invalid completion time.\n");
                return 0;
            }

            jobs[i].status = status;

            if (status == COMPLETED)
            {
                jobs[i].completion_time = completion_time;
                jobs[i].remaining_time = 0;
            }

            return 1;
        }
    }

    printf("Error: Job ID not found.\n");
    return 0;
}

int get_job_count(void)
{
    return job_count;
}

Job *get_jobs(void)
{
    return jobs;
}

void reset_jobs(void)
{
    job_count = 0;
}
