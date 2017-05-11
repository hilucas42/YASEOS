/*
 * File:   interruptions.c
 * Author: lucas
 *
 * Created on 4 de Maio de 2017, 21:39
 */

#include <pic18f4520.h>

#include "config.h"
#include "hardware.h"
#include "kernel.h"


/* Timer0 eh configurado como o timer padrao do sistema operacional. Nao esta
 * associado a uma rotina rtc, porem serve de base para a rotina de delay do
 * sistema operacional. Tem o prescaler configurado em 1:256, garantindo uma
 * precisao de 256 us e range de ate 16 s em 4MHz.
 */
#define configure_timer1()                                                     \
    T0CONbits.T08BIT    = 0;    /* Configura como um timer de 16 bits        */\
    T0CONbits.T0CS      = 0;    /* Fonte de clock como o ciclo de instrucao  */\
    T0CONbits.PSA       = 0;    /* Ativa o prescaler                         */\
    T0CONbits.T0PS      = 0b111;/* Define o valor do prescaler para 1:256    */\
    T0CONbits.TMR0ON    = 1;    /* Habilita o timer 0                        */


/* Timer3 eh configurado para disparar o escalonamento do sistema operacional
 * quando o Round-Robin esta ativo. Seu prescaler esta definido em 1:2 para que
 * seja possivel definir um range de quantum de ate 131 milissegundos (em 4MHz).
 */
#define configure_timer3()                                                     \
    T3CONbits.RD16      = 1;    /* Configura R/W em uma operacao de 16 bits  */\
    T3CONbits.TMR3CS    = 0;    /* Fonte de clock como o ciclo de instrucao  */\
    T3CONbits.T3CKPS    = 0b01; /* Define o valor do prescaler para 1:2      */\
    T3CONbits.TMR3ON    = 1;    /* Habilita o timer 0                        */\
    PIE2bits.TMR3IE     = 1;    /* Habilita interrupcoes pelo Timer3         */


/* O valor definido para TMR3 eh a conversao do periodo de quantum do algoritmo
 * scheduler Round-Robin em ticks (ciclos de instrucao do controlador), que eh
 * dada pelo produto do valor do quantum em segundos (RR_QUANTUM/1000), do valor
 * da frequencia de oscilacao em 1/segundos (_XTAL_FREQ) e a razao da frequencia
 * de oscilacao interna em ticks por oscilacao (1/4), subtraido do valor de
 * overflow do registrador. Por fim eh multiplicado toda a equacao pelo valor do
 * prescaler (definido como 1/2). Dessa forma, Timer3 dispara uma interrupcao a
 * cada RR_QUANTUM ms apos a chamada do procedimento reset_timer3().
 */
#define reset_timer3()                                                         \
    PIR2bits.TMR3IF     = 0;    /* Reseta a flag de interrupcao do Timer3    */\
    TMR3 = 0xFFFF-RR_QUANTUM*(_XTAL_FREQ/8000); /* Converte ms em ticks      */


void configure_hardware()
{
    configure_timer1();
    
#ifdef RR
    INTCONbits.PEIE = 1;    // Habilita interrupcoes por perifericos (ie Timer3)
    configure_timer3();
    reset_timer3();
#endif
}

/* Pretende-se tratar todas as interrupcoes dentro desta rotina, incluindo timer
 * do algoritmo scheduler, entrada de dados e quaisquer outras possiveis. Cada
 * interrupcao sera testada por uma condicional propria checando sua flag e deve
 * ter seu proprio salvamento de contexto. As condicionais devem ser dispostas
 * seguindo a ordem de prioridade de tratamento, com excecao do Timer3, que deve
 * ser tratado primeiro quando o algoritmo Round-Robin estiver ativo.
 */
void interrupt high_vector()
{
#ifdef RR
    if(PIR2bits.TMR3IF)
    {
        reset_timer3();
        dispatcher();
    }
#endif
}