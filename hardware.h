/* 
 * File:   hardware.h
 * Author: lucas
 *
 * Created on 4 de Maio de 2017, 21:40
 */

#ifndef HARDWARE_H
#define	HARDWARE_H

/**
 * Macros para controle de interrupcoes por rotinas sensiveis do kernel
 */
#define interruptions_off() INTCONbits.GIE  = 0;
#define interruptions_on()  INTCONbits.GIE  = 1;

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

