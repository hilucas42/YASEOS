/*
 * File:   kernel.resources.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:33
 */

#include <pic18f4520.h>

#include "kernel.h"

extern task_queue_t run_queue;

task_delay_list_t delay_list;
unsigned system_tick_counter;

void increment_tick_counter()
{
    system_tick_counter++;
}

/* A funcao delay poe a tarefa atual em bloqueio na lista de delay por um numero
 * determinado de ticks e entrega o processamento ao sistema operacional.
 * A informacao de overflow tambem eh persistida, sendo obtida a partir do bit
 * OV do registrador STATUS.
 */
void delay(unsigned ticks)
{
    ticks += system_tick_counter;
    delay_list.list[delay_list.list_size].task_index = run_queue.task_running;
    delay_list.list[delay_list.list_size].wake_up_at = ticks;
    delay_list.list[delay_list.list_size].overflowed = ticks < system_tick_counter;
    delay_list.list_size++;
    
    run_queue.task_list[run_queue.task_running].state = WAITING;
    yield();
}

/* O refresh da lista de tarefas bloqueadas remove da lista tarefas com periodo
 * de bloqueio atingido, com base na contagem de ticks do sistema e no overflow
 * obtido no calculo do prazo na rotina de bloqueio. As acoes sao:
 * Overflow | TS atingido | Tratamento
 *    0     |      0      | Mantem a rotina no delay
 *    0     |      1      | Acorda a rotina do delay
 *    1     |      0      | Limpa a flag de overflow
 *    1     |      1      | Mantem a rotina no delay
 */
void refresh_delay_list()
{
    int i, j;
    
    if(system_tick_counter == 0)
        for(j = 0; j < delay_list.list_size; j++)
            delay_list.list[j].overflowed   = 0;
    
    for(i = 0, j = 0; j < delay_list.list_size; j++)
    {
        if((delay_list.list[j].wake_up_at <= system_tick_counter)
                == delay_list.list[j].overflowed)
        {
            if(i != j)
                delay_list.list[i] = delay_list.list[j];
            i++;
        }
        else if(!delay_list.list[j].overflowed)
            run_queue.task_list[delay_list.list[j].task_index].state = RUNNING;
        else
            i++;
    }
    delay_list.list_size -= (j-i);
}
