/*
 * COMP 3500: Project 5 Scheduling
 * Will Humphlett
 * Version 2.0  2/21/2022
 *
 * This source code shows how to conduct separate compilation.
 *
 * How to compile using Makefile?
 * make
 *
 * How to manually compile?
 * g++ -c open.cpp
 * g++ -c read.cpp
 * g++ -c stats.cpp
 * g++ open.o read.o print.o scheduler.cpp -o scheduler
 * 
 * How to run?
 * ./scheduler <file-name> [FCFS|RR|SRFT] [time_quantum]
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <queue>
#include <vector>

#include "scheduler.h"
#include "open.h"
#include "read.h"
#include "stats.h"

/*
 * First Come First Serve (FCFS) scheduling policy.
 * Processes are ran in order of their arrival to completion.
 */
float fcfs_policy(task_t task_array[], u_int count) {
    int time = 0, i = 0, wasted = 0;
    while (i < count) {
        if (task_array[i].remaining_time == 0) {
            printf("<time %i> process %i is finished...\n", time, task_array[i].pid);
            task_array[i].finish_time = time;
            i++;
        } else if (task_array[i].arrival_time <= time) {
            printf("<time %i> process %i is running\n", time, task_array[i].pid);
            if (task_array[i].start_time == -1) {
                task_array[i].start_time = time;
            }
            task_array[i].remaining_time--;
            time++;
        } else {
            printf("<time %i> no process is currently running\n", time);
            time++;
            wasted++;
        }
    }

    printf("<time %i> All processes finished\n");
    return (time != 0) ? (float)(time - wasted) / (float)time : 0;
}

/*
 * Round Robin (RR) scheduling policy.
 * Processes are ran in order of their arrival for the length of time perscribed in the parameter quantum.
 * Should a process not complete after their quantum, the process will move to the back of the queue and wait for another quantum.
 */
float rr_policy(task_t task_array[], u_int count, u_int quantum) {
    int time = 0, i = 0, j = 0, finished = 0, wasted = 0;
    std::queue<task_t *> process_queue;

    while (finished < count) {
        while (i < count && task_array[i].arrival_time <= time) {
            process_queue.push(&task_array[i]);
            i++;
        }

        if (!process_queue.empty()) {
            task_t *current_process = process_queue.front();
            if (current_process->remaining_time == 0) {
                printf("<time %i> process %i is finished...\n", time, current_process->pid);
                current_process->finish_time = time;
                process_queue.pop();
                finished++;
                j = 0;
            } else {
                if (j < quantum) {
                    printf("<time %i> process %i is running\n", time, current_process->pid);
                    if (current_process->start_time == -1) {
                        current_process->start_time = time;
                    }
                    current_process->remaining_time--;
                    time++;
                    j++;
                } else {
                    j = 0;
                    process_queue.pop();
                    process_queue.push(current_process);
                }
            }
        } else {
            printf("<time %i> no process is currently running\n", time);
            time++;
            wasted++;
        }
    }

    printf("<time %i> All processes finished\n");
    return (time != 0) ? (float)(time - wasted) / (float)time : 0;
}

/*
 * Shortest Remaining Time First (SRTF) scheduling policy.
 * Processes are ran in order of which has the shortest remaining time.
 */
float srtf_policy(task_t task_array[], u_int count) {
    int time = 0, i = 0, finished = 0, wasted = 0;
    auto cmp = [](task_t *a, task_t *b) { return a->remaining_time > b->remaining_time; };
    std::priority_queue<task_t*, std::vector<task_t*>, decltype(cmp)> process_queue(cmp);

    while (finished < count) {
        while (i < count && task_array[i].arrival_time <= time) {
            process_queue.push(&task_array[i]);
            i++;
        }

        if (!process_queue.empty()) {
            task_t *current_process = process_queue.top();
            if (current_process->remaining_time == 0) {
                printf("<time %i> process %i is finished...\n", time, current_process->pid);
                current_process->finish_time = time;
                finished++;
                process_queue.pop();
            } else {
                printf("<time %i> process %i is running\n", time, current_process->pid);
                if (current_process->start_time == -1) {
                    current_process->start_time = time;
                }
                current_process->remaining_time--;
                time++;
                process_queue.pop();
                process_queue.push(current_process);
            }
        } else {
            printf("<time %i> no process is currently running\n", time);
            time++;
            wasted++;
        }
    }

    printf("<time %i> All processes finished\n");
    return (time != 0) ? (float)(time - wasted) / (float)time : 0;
}

/*
 * Simulate the process scheduling of a given process list with varying scheduling policies.
 * Note: All policies assume that the process list given as input is sorted with respect to the process arrival time.
 */
int main(int argc, char *argv[])  {
    char *file_name; /* file name from the commandline */
    FILE *fp; /* file descriptor */
    task_t task_array[MAX_TASK_NUM];

    int error_code;
    u_int i;
    u_int count;
    float cpu_usage;

    if (argc == 1 || argc > 4) {
        printf("Usage: command file_name [FCFS|RR|SRFT] [time_quantum]\n");
        return EXIT_FAILURE;
    }
    if (argc < 2) {
        printf("Usage: input <file_name>\n");
        return EXIT_FAILURE;
    }
    if (strcmp(argv[2], "RR") == 0) {
        if (argc < 4) {
            printf("Please enter time_quantum for the RR policy!\n");
            return EXIT_FAILURE;
        }
    }

    error_code = open_file(argv[1], &fp);
    if (error_code == 1)
        return EXIT_FAILURE;

    printf("Scheduling Policy: %s\n", argv[2]);

    read_file(fp, task_array, &count);

    printf("There are %d tasks loaded from \"%s\". Press enter to continue...", count, argv[1]);
    getchar();
    printf("==================================================================\n");

    if (strcmp(argv[2], "FCFS") == 0) {
        cpu_usage = fcfs_policy(task_array, count);
    } else if (strcmp(argv[2], "RR") == 0) {
        cpu_usage = rr_policy(task_array, count, atoi(argv[3]));
    } else if (strcmp(argv[2], "SRTF") == 0) {
        cpu_usage = srtf_policy(task_array, count);
    } else {
        printf("%s is not a valid scheduling policy!\n", argv[2]);
        return EXIT_FAILURE;
    }

    printf("==================================================================\n");
    calculate_stats(task_array, count, cpu_usage);
    printf("==================================================================\n");

    fclose(fp);
    return EXIT_SUCCESS;
}
