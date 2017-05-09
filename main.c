/*
 * File:   main.c
 * Author: lucas
 *
 * Created on 19 de Abril de 2017, 14:26
 */

#include "hardware.h"
#include "kernel.h"
#include "tasks.h"

void main() {
    
    configure_hardware();
    
    configure_kernel();
    
    /* Instalar as tarefas aqui */
    task_create(task_1, 1, 1);
    task_create(task_2, 2, 1);
    task_create(task_3, 3, 1);
    
    start_scheduling();
    
    while(1);
    
    /* Chamar as tarefas aqui para forcar a compilacao. Secao inatingivel. */
    task_1();
    task_2();
    task_3();
    idle();
}
