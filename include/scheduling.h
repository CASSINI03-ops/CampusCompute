#ifndef SCHEDULING_H
#define SCHEDULING_H

#include "jobs.h"

void run_fcfs(void);
void run_round_robin(int quantum);
void run_priority(void);

#endif
