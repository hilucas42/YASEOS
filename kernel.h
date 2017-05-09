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

#include "types.h"

////////////////////////////////////////////////////////////////////////////////
/// kernel.main.c                                                            ///
////////////////////////////////////////////////////////////////////////////////
/// Rotinas de nucleo do sistema operacional                                 ///
////////////////////////////////////////////////////////////////////////////////

/**
 * Inicializa o sistema operacional
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
 * O despachante do sistema operacional, responsavel por fazer o salvamento e
 * restauracao do contexto das tarefas e chamar o scheduler
 */
void dispatcher();

/**
 * A tarefa do tempo ocioso do sistema
 */
TASK idle();

////////////////////////////////////////////////////////////////////////////////
/// kernel.resources.c                                                       ///
////////////////////////////////////////////////////////////////////////////////
/// Rotinas de apoio ao sistema operacional                                  ///
////////////////////////////////////////////////////////////////////////////////

/**
 * Rotina de bloqueio de tarefas baseado em tempo
 * @param ms o periodo de bloqueio em milissegundos
 */
void delay(unsigned ms);

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

#endif

#endif	/* KERNEL_H */

