/* 
 * File:   hardware.h
 * Author: lucas
 *
 * Created on 4 de Maio de 2017, 21:40
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

/**
 * Inicia o timer de referencia do contador de ticks do sistema operacional
 */
void start_tick_counter();

/**
 * Configura os componentes de hardware. Basicamente vai chamar outras funcoes
 * e rotinas que irao efetivamente fazer os ajustes necessarios
 */
void configure_hardware();

/**
 * Rotina de tratamento de interrupcoes de alta prioridade. Poderao ser
 * incluidas aqui tratamento de varias fontes, como timers ou entradas
 */
void interrupt high_vector();

#endif	/* HARDWARE_H */

