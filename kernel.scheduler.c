/*
 * File:   kernel.scheduler.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 15:29
 */

#include "kernel.h"

extern task_queue_t run_queue;

#ifndef ENABLE_PRIORITY

_list_index round_robin_sc()
{
    _list_index next_task = run_queue.task_running;
    
    do
    {
        if(++next_task == run_queue.installed_tasks)
            next_task = 0;
    } while(run_queue.task_list[next_task].state != RUNNING);
    
    return next_task;
}

#else // Prioridades

_list_index priority_sc()
{
    _list_index i, next_task = run_queue.task_running;

    do
    {
        if(++next_task == run_queue.installed_tasks)
            next_task = 0;
    } while(run_queue.task_list[next_task].state != RUNNING);

    for(i = next_task; i != run_queue.task_running;)
    {
        if(run_queue.task_list[i].state == RUNNING)
            if(run_queue.task_list[i].priority >
                    run_queue.task_list[next_task].priority)
                next_task = i;
        if(++i == run_queue.installed_tasks)
            i = 0;
    }

    return next_task;
}

#endif