/* 
 * File:   types.h
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:34
 */

#ifndef TYPES_H
#define	TYPES_H

#include "config.h"

typedef unsigned char byte;
typedef enum {RUNNING, WAITING, STOPPED} state_t;
typedef void TASK;
typedef TASK (*TASK_P)();
typedef unsigned _list_index;

/**
 * Uma camada da pilha de enderecos de uma tarefa
 */
typedef struct
{
    byte tosu : 5;
    byte tosh;
    byte tosl;
} task_stack_t;

/**
 * O contexto de uma tarefa
 */
typedef struct
{
    byte bsr_r;
    byte work_r;
    byte status_r;
    byte stack_size;
    task_stack_t stack[HW_STACK_SIZE];
} task_context_t;

/**
 * A tabela de controle da tarefa, com as informacoes exigidas pelo SO
 */
typedef struct
{
    unsigned tid;
    state_t state;
    unsigned priority;
    TASK_P routine;
    task_context_t context;
} tcb_t;

/**
 * A fila de tarefas em execucao no SO
 */
typedef struct
{
    tcb_t task_list[MAX_TASKS_NR];
    _list_index task_running;
    unsigned installed_tasks;
} task_queue_t;

#define current_task run_queue.task_list[run_queue.task_running]
#define current_task_stk_size run_queue.task_list[run_queue.task_running].context.stack_size

/**
 * Informacao de uma tarefa em sleep
 */
typedef struct
{
    byte overflowed;
    unsigned wake_up_at;
    _list_index task_index;
} task_delay_t;

/**
 * Lista de tarefas em sleep
 */
typedef struct
{
    unsigned list_size;
    task_delay_t list[MAX_TASKS_NR];
} task_delay_list_t;
#endif	/* TYPES_H */

