/*
 * File:   kernel.scheduler.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 15:29
 */

#include "kernel.h"

extern task_queue_t run_queue;

#ifdef RR // Round Robin

_list_index round_robin_sc()
{
    _list_index next_task;
    
    do
    {
        next_task = (run_queue.task_running + 1) % run_queue.installed_tasks;
    } while(run_queue.task_list[next_task].state != RUNNING);
    
    return next_task;
}

#else // Prioridades

_list_index priority_sc()
{
    // Todo code here
}

#endif