/*****************************************************
 * Picore Module : schedule.s
 * Project       : Picore OS
 * Author        : Cyrille Ngassam Nkwenga
 ***************************************************/
#include <asm-def.h>

/*
.extern PCB_CURRENT_TASK_ID
.extern PCB_CURRENT_TASK
.extern PCB_NEXT_TASK
.extern PCB_TABLE
*/
/*
.global core_scheduler

.text //SCHEDULER

core_scheduler:
currentTask:
    LDR R3, =PCB_CURRENT_TASK_ID
    LDR R0, [R3]
    LDR R1, =PCB_TABLE
    LDR R1, [R1, R0, LSL#2] // R1 points to current Task address
    LDR R2, =PCB_CURRENT_TASK
    STR R1, [R2]

nextTask:
    ADD R0, R0, #1
    CMP R0, #3
    MOVEQ R0, #0
    STR R0, [R3]    // Save new task ID
    LDR R1, =PCB_TABLE
    LDR R1, [R1, R0, LSL#2]
    LDR R0, =PCB_NEXT_TASK
    STR R1, [R0] // Save the current pcb structure.

context_switch_handler:
    LDMFD SP!, {R0-R3, R12, R14}
    LDR R13, =PCB_CURRENT_TASK
    LDR R13, [R13]
    SUB R13, R13, #60
    STMIA R13, {R0-R14}^
    MRS R0, SPSR
    STMDB R13, {R0, R14}

    LDR R13, =PCB_NEXT_TASK
    LDR R13, [R13]
    SUB R13, R13, #60
    LDMDB R13, {R0, R14}
    MSR SPSR_cxsf, R0
    LDMIA R13, {R0-R14}^
    LDR R13, =#IRQ_STACK
    LDR R13, [R13]
    MOVS PC, R14
*/
