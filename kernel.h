/* 
 * File:   kernel.h
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 15:37
 */

#ifndef KERNEL_H
#define	KERNEL_H

#include <xc.h>
#include <p18f4520.h>
#include <pic18f4520.h>
#include "hardware.h"
#include "types.h"

////////////////////////////////////////////////////////////////////////////////
/// kernel.main.c                                                            ///
////////////////////////////////////////////////////////////////////////////////
/// Rotinas de nucleo do sistema operacional                                 ///
////////////////////////////////////////////////////////////////////////////////

/**
 * Inicializa as estruturas de dados do sistema operacional
 */
void configure_kernel();

/**
 * Instala uma nova tarefa no sistema operacional
 * @param id o identificador TID da tarefa. Nao deve ser repetido.
 * @param routine a funcao inicial da tarefa
 * @param priority a prioridade da tarefa
 */
void task_create(TASK_P routine, unsigned id, unsigned priority);

/**
 * Inicia o processo de escalonamento do sistema operacional
 */
void start_scheduling();

/**
 * ISR do timer contador de ticks. Responsavel por chamar a rotina de incremento
 * do contador de ticks do sistema operacional e atualizar a lista de tarefas
 * bloqueadas. Realiza troca de contexto ao chamar o escalonador.
 */
void dispatcher();

/**
 * Permite a tarefa liberar o hardware para o sistema operacional. Realiza troca
 * de contexto ao chamar o escalonador.
 */
void yield();

/**
 * Encerra a tarefa em execucao, definindo estado de bloqueio nao interruptivel
 */
#ifdef exit
#undef exit
#endif
void exit();

/**
 * A tarefa do tempo ocioso do sistema, ocupa o hardware enquanto nao ha nenhuma
 * tarfa em execucao.
 */
TASK idle();

////////////////////////////////////////////////////////////////////////////////
/// kernel.resources.c                                                       ///
////////////////////////////////////////////////////////////////////////////////
/// Rotinas de apoio ao sistema operacional                                  ///
////////////////////////////////////////////////////////////////////////////////

/**
 * Incrementa o contador de ticks do sistema operacional.
 */
void increment_tick_counter();

/**
 * Rotina de bloqueio de tarefas baseado em ticks do sistema operacional
 * @param ticks o periodo de bloqueio em ticks
 */
void delay(unsigned ticks);

/**
 * Macro para bloqueio de tarefas baseado em tempo, arredonda o tempo para ticks
 * @param ms o periodo de bloqueio em milissegundos
 */
#define delay_ms(ms) delay((unsigned)((ms)*(7812500.0/(_XTAL_FREQ*(1<<TTPS)))))

/**
 * Roda a cada troca de contexto, atualizando os tempos de wake-up das tarefas,
 * bem como desbloqueando as tarefas com o tempo de delay esgotado
 */
void refresh_delay_list();

///////////////////////////////////////////////////////////////////////////////
/// kernel.scheduler.c                                                      ///
///////////////////////////////////////////////////////////////////////////////
/// Escalonadores de tarefas do sistema operacional                         ///
///////////////////////////////////////////////////////////////////////////////

#ifdef RR

/**
 * Escalonador circular baseado em quantum
 * @return a posicao na fila de aptos do processo selecionado para usar a CPU
 */
_list_index round_robin_sc();

#define default_scheduler() round_robin_sc()

#else // Prioridades

/**
 * Escalonador baseado em prioridades
 * @return a posicao na fila de aptos do processo selecionado para usar a CPU
 */
_list_index priority_sc();

#define default_scheduler() priority_sc()

#endif  /* RR */

///////////////////////////////////////////////////////////////////////////////
/// kernel.scheduler.c                                                      ///
///////////////////////////////////////////////////////////////////////////////
/// Escalonadores de tarefas do sistema operacional                         ///
///////////////////////////////////////////////////////////////////////////////

/**
 * Inicializa a heap. Documentacao completa na implementacao.
 */
void SRAMInitHeap(void);

/**
 * Alocacao dinamica de memoria. Documentacao completa na implementacao.
 * @param nBytes o numero de bytes a serem alocados
 * @return um ponteiro para o espaco de memoria alocado ou nulo em caso de falha
 */
unsigned char * NEAR SRAMalloc(NEAR unsigned char nBytes);

/**
 * Libera memoria previamente alocada. Documentacao completa na implementacao.
 * @param pSRAM o ponteiro para a memoria a ser liberada
 */
void SRAMfree(unsigned char * NEAR pSRAM);

#endif	/* KERNEL_H */

