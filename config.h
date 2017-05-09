/* 
 * File:   config.h
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:31
 */

#ifndef CONFIG_H
#define	CONFIG_H

 /* Define a frequencia do oscilador */
#pragma config OSC=HS

 /* Desabilita o watchdog timer */
#pragma config WDT=OFF

 /* Frequencia de oscilacao */
#ifndef _XTAL_FREQ
#define _XTAL_FREQ      4000000
#endif

 /* Tamanho da pilha de hardware */
#define HW_STACK_SIZE   32

 /* Numero maximo de tarefas instaladas */
#define MAX_TASKS_NR    4

 /* quantum do escalonador round robin (em ms)) */
#define RR_QUANTUM      2

 /* Escalonador a ser utilizado Round Robin - Comentar para usar prioridades  */
#define RR

 /* Tamanho da memória HEAP (Precisa ser multiplo de 8) em bytes */
#define HEAP_SIZE       512

#endif	/* CONFIG_H */

