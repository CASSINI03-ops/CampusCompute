#ifndef JOBS_H
#define JOBS_H
typedef enum {
    WAITING,
    RUNNING,
    COMPLETED
} JobStatus;

typedef struct {
    int job_id;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    JobStatus status;
} Job;

int add_job(int id, int arrival, int burst, int priority);

void display_jobs(void);

int update_job_status(int id, JobStatus status, int completion_time);

int get_job_count(void);
Job *get_jobs(void);
void reset_jobs(void);
void run_fcfs(void);
void run_round_robin(int quantum);
void run_priority(void);
#endif
