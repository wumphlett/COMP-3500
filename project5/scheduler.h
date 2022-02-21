/*
 * COMP 3500: Project 5 Scheduling
 * Will Humphlett
 * Version 2.0  2/21/2022
 *
 * scheduler.h: The header file of scheduler.c
 */
#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define MAX_TASK_NUM 32

typedef unsigned int u_int;

typedef struct task_info {
    u_int pid;
    u_int arrival_time;
    u_int start_time;
    u_int burst_time;
    u_int remaining_time;
    u_int finish_time;
} task_t;

#endif
