/*
 * File:   kernel.main.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:27
 */

#include "kernel.h"
#include "hardware.h"

task_queue_t run_queue;

void configure_kernel()
{
    run_queue.installed_tasks = 0;
    task_create(idle, 0, 1);
}

void task_create(TASK_P routine, unsigned id, unsigned priority)
{
    unsigned short long routine_pointer_handler = (unsigned short long) routine;
    
    run_queue.task_list[run_queue.installed_tasks].tid      = id;
    run_queue.task_list[run_queue.installed_tasks].state    = RUNNING;
    run_queue.task_list[run_queue.installed_tasks].routine  = routine;
    run_queue.task_list[run_queue.installed_tasks].priority = priority;
    
    run_queue.task_list[run_queue.installed_tasks].context.stack[0].tosl
            = (byte)(routine_pointer_handler);
    run_queue.task_list[run_queue.installed_tasks].context.stack[0].tosh
            = (byte)(routine_pointer_handler >> 8);
    run_queue.task_list[run_queue.installed_tasks].context.stack[0].tosu
            = (byte)(routine_pointer_handler >> 16);
    run_queue.task_list[run_queue.installed_tasks].context.stack_size = 1;
            
    run_queue.installed_tasks++;
}

/* Rotina para o salvamento de contexto da tarefa em execucao */
#define save_context()                       \
    current_task.context.work_r     = WREG;  \
    current_task.context.status_r   = STATUS;\
    current_task.context.bsr_r      = BSR;   \
    while(STKPTR != 0)                       \
    {                                        \
        current_task.context.stack[current_task_stk_size].tosu = TOSU;         \
        current_task.context.stack[current_task_stk_size].tosh = TOSH;         \
        current_task.context.stack[current_task_stk_size].tosl = TOSL;         \
        current_task_stk_size++;                                               \
        asm("POP");                                                            \
    }

/* Rotina para a restauracao do contexto da tarefa em execucao */
#define restore_context()               \
    while(current_task_stk_size != 0)   \
    {                                   \
        asm("PUSH");                    \
        current_task_stk_size--;        \
        TOSL = current_task.context.stack[current_task_stk_size].tosl;         \
        TOSH = current_task.context.stack[current_task_stk_size].tosh;         \
        TOSU = current_task.context.stack[current_task_stk_size].tosu;         \
    }                                           \
    current_task.context.bsr_r      = BSR;      \
    current_task.context.status_r   = STATUS;   \
    current_task.context.work_r     = WREG;

#define IDLE 0

void start_scheduling()
{
    run_queue.task_running = IDLE;
    restore_context();
    start_tick_counter();
    interruptions_on();
}

void dispatcher()
{
    asm("POP");
    
    interruptions_off();
    
    save_context();
    
    increment_tick_counter();
    
    refresh_delay_list();
    
    run_queue.task_running = default_scheduler();
    
    restore_context();
    
    interruptions_on();
}

void yield()
{
    asm("POP");
    
    interruptions_off();
    
    save_context();
    
    run_queue.task_running = default_scheduler();
    
    restore_context();
    
    interruptions_on();
}

TASK idle()
{
    while(1);
}
