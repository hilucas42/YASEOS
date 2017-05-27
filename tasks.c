/*
 * File:   tasks.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:32
 */

#include "kernel.h"
#include "tasks.h"

void task_1()
{
    int x;
    int *share = (int*)shm_get(42, sizeof(int));
    
    short long *share_sem = (short long *)shm_get(17, sizeof(short long));
    
    sem_t *sem1 = createSem(1);
    
    share_sem[0] = (short long)sem1;
    
    share[0] = 0;
    x = 1;
    
    while(1){
        
        semWait(sem1);
        
        if(x == 0) shm_release(17);
        
        for(x = share[0]; x < 5; x++)
        {
            PORTDbits.RD0 = ~LATDbits.LD0;
            delay_ms(500);
        }
        share[0] = x;
        semPost(sem1);
    }
}

void task_2()
{
    int x;
    int *share = (int**)shm_get(42, sizeof(int));
    
    short long *share_sem = (short long *)shm_get(17, sizeof(short long));
    
    sem_t *sem1 = (sem_t*)share_sem[0];
    
    while(1){
        
        semWait(sem1);
        for(x = share[0]; x < 10; x++)
        {
            PORTDbits.RD1 = ~LATDbits.LD1;
            delay_ms(500);
        }
        share[0] = x;
        semPost(sem1);
    }
    
    exit();
}

void task_3()
{
    int x;
    int *share = (int**)shm_get(42, sizeof(int));
    
    short long *share_sem = (short long *)shm_get(17, sizeof(short long));
    
    sem_t *sem1 = (sem_t*)share_sem[0];
    
    while(1){
        
        semWait(sem1);
        for(x = share[0]; x < 15; x++)
        {
            PORTDbits.RD2 = ~LATDbits.LD2;
            delay_ms(500);
        }
        share[0] = 0;
        semPost(sem1);
    }
}