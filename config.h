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

 /* Tick timer Prescaler pode receber valores de 0 a 7. O periodo do tick em ms
  * e dado pela equacao P = 256*2^(PSTT+1)*4000/_XTAL_FREQ e o periodo maximo
  * de delay e D = P*65.536-1 ms */
#define TTPS            5

 /* Escalonador a ser utilizado Round Robin - Comentar para usar prioridades  */
#define RR

 /* Tamanho da memória HEAP em bytes */
#define	MAX_HEAP_SIZE		0x200

 /* Caso desejar usar o modificador near defe-se definir NEAR_MODEL antes  */
#if defined(NEAR_MODEL)
#define	NEAR	near
#else
#define NEAR
#endif

#endif	/* CONFIG_H */

