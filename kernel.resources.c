/*
 * File:   kernel.resources.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:33
 */

#include <pic18f4520.h>

#include "kernel.h"

extern task_queue_t run_queue;
extern task_delay_list_t delay_list;

/* A funcao delay converte o tempo recebido em milissegundos para informacoes de
 * passos do Timer0. Para obter o numero de passos de delay obtem o produto do
 * tempo recebido em segundos (time_to_delay/1000), da frequencia de oscilacao
 * em 1/segundos (_XTAL_FREQ), e a razao da frequencia de oscilacao interna em
 * ticks por oscilacao (1/4), que somado ao valor do Timer0 define o passo onde
 * a rotina deve acordar. Para completar a instrucao, a equacao eh multiplicada
 * pelo valor do prescaler do Timer0 (1/256).
 * A informacao de overflow tambem eh persistida, sendo obtida a partir do bit
 * OV do registrador STATUS.
 */
void delay(unsigned time_to_delay)
{
    //unsigned wake_up_at = TMR0 + time_to_delay*_XTAL_FREQ/1024000;
     /* Arredondamento com erro de 2,34% para fins de testes. Remover depois. */
    unsigned wake_up_at = TMR0 + time_to_delay*_XTAL_FREQ/1000000;
    
    delay_list.list[delay_list.list_size].task_index = run_queue.task_running;
    delay_list.list[delay_list.list_size].wake_up_at = wake_up_at;
    delay_list.list[delay_list.list_size].overflowed = STATUSbits.OV;
    delay_list.list_size++;
    
    run_queue.task_list[run_queue.task_running].state = WAITING;
    dispatcher();
}

/* O refresh da lista de tarefas bloqueadas remove da lista tarefas com tempo de
 * bloqueio excedido, se baseando nas informacoes de timestamp de desbloqueio e
 * overflow do Timer0. Os tratamentos seguem a tabela:
 * Overflow | TS atingido | Tratamento
 *    0     |      0      | Mantem a rotina no delay
 *    0     |      1      | Acorda a rotina do delay
 *    1     |      0      | Limpa a flag de overflow
 *    1     |      1      | Mantem a rotina no delay
 */
void refresh_delay_list()
{
    int i, j;
    for(i = 0, j = 0; j < delay_list.list_size; j++)
    {
        if((delay_list.list[j].wake_up_at <= TMR0) == delay_list.list[j].overflowed)
        {
            if(i != j)
                delay_list.list[i] = delay_list.list[j];
            i++;
        }
        else if(delay_list.list[j].overflowed)
        {
            delay_list.list[j].overflowed   = 0;
            i++;
        }
        else
            run_queue.task_list[delay_list.list[j].task_index].state = RUNNING;
    }
    delay_list.list_size -= (j-i);
}
