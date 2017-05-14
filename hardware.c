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
 * associado a uma rotina rtc, porem define o periodo de um tick do sistema com
 * base em seu overflow permitindo implementacao futura. Como timer de 8 bits
 * permite ticks com periodos entre 512 e 65536 ciclos de instrucao, ajustados
 * pelo seu prescaler.
 */
#define configure_timer0()                                                     \
    T0CONbits.T08BIT    = 1;    /* Configura como um timer de 8 bits         */\
    T0CONbits.T0CS      = 0;    /* Fonte de clock como o ciclo de instrucao  */\
    T0CONbits.PSA       = 0;    /* Ativa o prescaler                         */\
    T0CONbits.T0PS      = PSTT; /* Define o valor do prescaler para 1:256    */

void start_tick_counter()
{
    INTCONbits.TMR0IE   = 1;
    T0CONbits.TMR0ON    = 1;
}

void configure_hardware()
{
    configure_timer0();
}

/* Rotina instalada no vetor de interrupcao de alta prioridade, responsavel por
 * identificar a interrupcao e chamar o ISR adequado para seu tratamento.
 * Rotinas de salvamento e restauracao de contexto devem ser executadas no ISR.
 */
void interrupt high_vector()
{
    if(INTCONbits.TMR0IF == 1)
    {
        INTCONbits.TMR0IF = 0;
        dispatcher();
    }
}