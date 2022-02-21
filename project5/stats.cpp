#include <stdio.h>
#include "scheduler.h"

/*
 * Print the stats of a task list.
 */
void calculate_stats(task_t task_list[], int size, float cpu_usage) {
    float waiting = 0, response = 0, turnaround = 0;

    for (int i = 0; i < size; i++) {
        waiting += (float)(task_list[i].finish_time - task_list[i].arrival_time - task_list[i].burst_time);
        turnaround += (float)(task_list[i].finish_time - task_list[i].arrival_time);
        response += (float)(task_list[i].start_time - task_list[i].arrival_time);
    }

    waiting = (size != 0) ? waiting / (float)size : 0;
    turnaround = (size != 0) ? turnaround / (float)size : 0;
    response = (size != 0) ? response / (float)size : 0;

    printf("Average waiting time:    %.2f\n", waiting);
    printf("Average response time:   %.2f\n", response);
    printf("Average turnaround time: %.2f\n", turnaround);
    printf("Overall CPU usage:       %5.2f%%\n", cpu_usage * 100);
}
