#ifndef _SUPERVISOR_H
#define _SUPERVISOR_H
#include <stdio.h>
#include <timer.h>
#include <gpio.h>
// SECURE Supervisor handlers
/*
void sec_supervisor_undefined_instr_handler(int *iaddr);
void sec_supervisor_svc_handler(unsigned int arg);

void sec_supervisor_prefetch_abort_handler(unsigned int *op_addr);
void sec_supervisor_data_abort_handler(unsigned int *data_addr);
void sec_supervisor_irq_handler();
void sec_supervisor_firq_handler();


// Non Secure Handlers
void supervisor_undefined_instr_handler();
void supervisor_svc_handler(unsigned int arg);

void supervisor_prefetch_abort_handler(unsigned int *op_addr);
void supervisor_data_abort_handler(unsigned int *data_addr);
void supervisor_irq_handler();
void supervisor_firq_handler();
*/


//void cpu_core_running();
void print_cpu_core();
#endif // _SUPERVISOR_H
