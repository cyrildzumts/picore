.text
.global _start
.global _reset
.global _get_stack_pointer
.global hyp_spsr_boot
.global hyp_elr_boot
.global hyp_elr_mod
.global modified_spsr
.global PAGE_TABLE_FISRT_LEVEL
.global PAGE_TABLE_SECOND_LEVEL
.global _cpu_registers
.global _cpu_reg
.global _cpu_registers_mod
.global _cpu_reg_mod
.global _core_counter
.global _core_started
.extern __bss_start__
.extern __bss_end__
.extern _stack
.global _PCB_TABLE_
#include <asm-def.h>
.data
_core_started : .word 0
hyp_vec_addr: .word 0
sec_sup_vec_addr: .word 0
sup_vec_addr: .word 0
mon_vec_addr: .word 0
hyp_spsr_boot: .word 0
hyp_elr_boot : .word 0
next_pc      : .word 0
modified_spsr: .word 0
hyp_elr_mod : .word 0


_PCB_TABLE_:
PCB_TABLE:
   .word PCB_TASK_1
   .word PCB_TASK_2
   .word PCB_TASK_3

PCB_TASK_1 : .space 4*18     // Allocate an array of 17 Words for the PCB
                             // This contains in the order :R14 - R0, PC + 4,SPSR
PCB_TASK_1_PTR:
PCB_TASK_2 : .space 4*18
PCB_TASK_2_PTR:
PCB_TASK_3: .space 4*18
PCB_TASK_3_PTR:
PCB_CURRENT_TASK : .word 0x0
PCB_NEXT_TASK    : .word 0x01
PCB_CURRENT_TASK_ID: .word 0x0

// CPU Registers Table. This is not a standard by ARM.

_cpu_reg : .space 4*18 // R0-R14 + CPSR + SPSR
_cpu_registers: .word _cpu_reg
_core_counter : .word 0
_cpu_reg_mod : .space 4*18 // R0-R14 + CPSR + SPSR
_cpu_registers_mod: .word _cpu_reg_mod
_spi_data: .word 0




.text
_start: /* This is our Entry point */
/* Hypervisor */
/* ***********************************************
   This code is for the Raspberry Pi 2.
   Since the Raspberry Pi 2 / 3 bootloader set
   the cpu in Hyp mode, _start points to the 
   hyp_vector_table. If your Raspberry Pi 2 boots
   in Supervisor mode, then swap _hyp_vector_table
   with _sec_supervisor_vector_table since ARM CPU
   boot in Secure Supervisor Mode 
 **************************************************/
_hyp_vector_table:
        LDR PC, _hyp_reset_handler_
        LDR PC, _hyp_undef_inst_handler_
        LDR PC, _hyp_hvc_handler_
        LDR PC, _hyp_prefetch_abort_handler_ 
        LDR PC, _hyp_data_abort_handler_ 
        LDR PC, _hyp_trap_handler_ /* reserved */
        LDR PC, _hyp_irq_handler_ /* IRQ */
        LDR PC, _hyp_firq_handler_ /* FIQ */

_hyp_reset_handler_             :       .word hyp_reset_
_hyp_undef_inst_handler_        :       .word hyp_undefined_instr_handler_
_hyp_hvc_handler_               :       .word hyp_hvc_handler_
_hyp_prefetch_abort_handler_    :       .word hyp_prefetch_abort_handler_
_hyp_data_abort_handler_        :       .word hyp_data_abort_handler_
_hyp_trap_handler_              :       .word hyp_trap_handler_
_hyp_irq_handler_               :       .word hyp_irq_handler_
_hyp_firq_handler_              :       .word hyp_firq_handler_

/* Secure Supervisor */
/*************************************************************
 The ARM CPU by default boot in Secure Supervisor Mode.
 in this mode, IRQ / FIRQ are masked by default.
 ************************************************************/
_sec_supervisor_vector_table:
        LDR PC, _sec_supervisor_reset_handler_
        LDR PC, _sec_supervisor_undef_inst_handler_
        LDR PC, _sec_supervisor_svc_handler_
        LDR PC, _sec_supervisor_prefetch_abort_handler_
        LDR PC, _sec_supervisor_data_abort_handler_
        LDR PC, _sec_supervisor_unused_handler_ /* unused */
        LDR PC, _sec_supervisor_irq_handler_
        LDR PC, _sec_supervisor_firq_handler_

_sec_supervisor_reset_handler_             :       .word sec_supervisor_reset_
_sec_supervisor_undef_inst_handler_        :       .word sec_supervisor_undefined_instr_handler_
_sec_supervisor_svc_handler_               :       .word sec_supervisor_svc_handler_
_sec_supervisor_prefetch_abort_handler_    :       .word sec_supervisor_prefetch_abort_handler_
_sec_supervisor_data_abort_handler_        :       .word sec_supervisor_data_abort_handler_
_sec_supervisor_unused_handler_            :       .word sec_supervisor_trap_handler_/* unused */
_sec_supervisor_irq_handler_               :       .word sec_supervisor_irq_handler_
_sec_supervisor_firq_handler_              :       .word sec_supervisor_firq_handler_

/* Non Secure Supervisor */
/*************************************************************
 * This is the mode we want the CPU to be in.
   I provide all handler so that I can catch
   every exceptions thrown.
************************************************************/
_supervisor_vector_table:
        LDR PC, _supervisor_reset_handler_ /* unused */
        LDR PC, _supervisor_undef_inst_handler_
        LDR PC, _supervisor_svc_handler_
        LDR PC, _supervisor_prefetch_abort_handler_
        LDR PC, _supervisor_data_abort_handler_
        LDR PC, _supervisor_unused_handler_ /* unused */
        LDR PC, _supervisor_irq_handler_
        LDR PC, _supervisor_firq_handler_

_supervisor_reset_handler_             :       .word supervisor_trap_handler_/* unused */
_supervisor_undef_inst_handler_        :       .word supervisor_undefined_instr_handler_
_supervisor_svc_handler_               :       .word supervisor_svc_handler_
_supervisor_prefetch_abort_handler_    :       .word supervisor_prefetch_abort_handler_
_supervisor_data_abort_handler_        :       .word supervisor_data_abort_handler_
_supervisor_unused_handler_            :       .word supervisor_trap_handler_ /* unused */
_supervisor_irq_handler_               :       .word supervisor_irq_handler_
_supervisor_firq_handler_              :       .word supervisor_firq_handler_


/* Monitor Mode */
/***************************************************************
 This mode is always in secure mode. 
 For this code base, this mode is only used to change the 
 secure state of the CPU (SCR.NS). There is no need to provide
 all the handlers listed in the vector table.
 The Interrupts Bits will be left masked.
 Only the smc_handler_ needs to be provided
 **************************************************************/

_monitor_vector_table:
        LDR PC, _monitor_reset_handler_ /* unused */
        LDR PC, _monitor_undef_inst_handler_ /* unused */
        LDR PC, _monitor_smc_handler_
        LDR PC, _monitor_prefetch_abort_handler_
        LDR PC, _monitor_data_abort_handler_
        LDR PC, _monitor_unused_handler_ /* unused */
        LDR PC, _monitor_irq_handler_
        LDR PC, _monitor_firq_handler_

_monitor_reset_handler_             :       .word monitor_trap_handler_ /* unused */
_monitor_undef_inst_handler_        :       .word monitor_trap_handler_ /* unused */
_monitor_smc_handler_               :       .word monitor_smc_handler_
_monitor_prefetch_abort_handler_    :       .word monitor_prefetch_abort_handler_
_monitor_data_abort_handler_        :       .word monitor_data_abort_handler_
_monitor_unused_handler_            :       .word monitor_trap_handler_ /* unused */
_monitor_irq_handler_               :       .word monitor_trap_handler_
_monitor_firq_handler_              :       .word monitor_trap_handler_


hyp_reset_:
sec_supervisor_reset_:
    // Save the current Registers into _cpu_reg table
    CPSID if
    // Count numbers of cores
    LDR R0, =_core_started
    LDR R1, [R0]
    ADD R1, R1, #1
    STR R1, [R0]
    LDR R5, =_cpu_reg
    LDR SP, =#HYP_STACK
    
    MOV R6, R5
    MRS R2, SPSR
    MRS R3, ELR_hyp
    MRS R4, CPSR
    STM R5!, {R0-R4}
    STM R5!, {R6,R13,R14}
    
// clear bss sections as required by the C ANSI
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0
bss_clear_loop:
    cmp     r0,r1
    it      lt
    strlt   r2,[r0], #4
    blt     bss_clear_loop


    
// Copy vectors to 0x0000, 16 words
copy_vec_tables:
    /** COPY HYP_VECTOR_TABLE AND SAVE IT ADDRESS in hyp_vec_addr */
    /* THE HVBAR holds the address of this vector table */

    mov r0, #0x0000
    LDR R1, =hyp_vec_addr
    STR R0, [R1]
    ldr r1, =_hyp_vector_table
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}

    /** COPY SECURE SUPERVISOR VECTOR TABLE
     * The address of this vector is written into VBAR
    */
    LDR R1, =sec_sup_vec_addr
    STR R0, [R1]
    ldr r1, =_sec_supervisor_vector_table
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}

    /** COPY NON SECURE SUPERVISOR VECTOR TABLE
     * The address of this vector is written into VBAR
    */
    LDR R1, =sup_vec_addr
    STR R0, [R1]
    ldr r1, =_supervisor_vector_table
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}

    /** COPY MONITOR VECTOR TABLE
     * The address of this vector is written into VBAR
    */
    LDR R1, =mon_vec_addr
    STR R0, [R1]
    //mov R10, R6
    ldr r1, =_monitor_vector_table
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}
    ldmia r1!, {r2-r9}
    stmia r0!, {r2-r9}

/* ALL VECTORS TABLE HAVE BEEN COPIED to the address 0x00000000. */
/* Now the first 128B starting from address 0x00000000 contains
 all the vectors tables. */



// update HVBAR : HVBAR holds the exception base address.
    LDR R0, =hyp_vec_addr
    LDR R1, [R0]
    //MCR P15, 4, R1, C12, C0, 0 // WRITE to HVBAR

    /**********************************************************
     * ACTIVATE VFP and then Enable the Mini Uart Device
     * so that we will have an activated console for debugging
     * ********************************************************/
    BL _VFP_enable
    ISB // synchronize context on this processor
    
    // We send a notification by blinking the ACK LED 5 times.
    MOV R0, #47
    MOV R1, #5
    BL core_blink
    BL assert

    /** here we have vfp enabled and the console is ON.
     ** Now we change the CPU to Supervisor Mode*/
    // After that the CPU will be in Non Secure Supervisor Mode
    // Actually we should first check if the CPU is in HYP Mode
    LDR LR, =after_change_mode
    MSR ELR_hyp, LR
    //CPSR contains the processor mode
    MRS R4, CPSR
    //Clear the processor mode bits 4-0
    BIC R4, R4, #0x1F
    ORR R4, R4, #CPSR_MODE_SVR //0x13

    // Here we change the processor mode finally.
    // since we are in Hyp Mode, ERET(Exception RETurn) is used to return to after_change_mode
    MSR SPSR_hyp, R4
    ERET

after_change_mode:
/* We are now in Non Secure Supervisor Mode. By default IRQs are masked
   so we need to change to Monitor Mode, set SCR.NS (Non Secure Mode)
   and change back to Non Secure Supervisor and then Activate the IRQs.
 * befor that, let set the secure copy of VBAR 
 ************************************************/
    ISB
    LDR SP, =#SVC_STACK
    LDR R0, =sup_vec_addr
    LDR R1, [R0]
    MCR P15, 0, R1, C12, C0, 0 // WRITE to Non Secured version of VBAR

    MSR CPSR_c, #(NO_INT|CPSR_MODE_UNDEFINED)
    LDR SP, =#UNDEF_STACK

    MSR CPSR_c, #(NO_INT | CPSR_MODE_IRQ)
    LDR SP, =#IRQ_STACK

    MSR CPSR_c, #(NO_INT | CPSR_MODE_SYSTEM)  // set user mode stack
    LDR SP, =#SYS_STACK

    MSR CPSR_c, #NO_INT | CPSR_MODE_SVR

.text
next:
// first initialize the mini-uart device.
    LDR R0, =#270
    BL mu_init
    //BL  spi_init
    //BL displayInit
    BL minimal_init
    MOV R0, #47
    BL assert  
    BL main
trap:
    B trap





/* hypervisor Handlers */
hyp_undefined_instr_handler_:
    PUSH {R0-R12, LR}
    SUB R0, LR, #4

    BL hyp_undefined_instr_handler
    POP {R0-R12, LR}

    ERET

hyp_hvc_handler_:
    PUSH {R0-R12, LR}
    MRS R1, ELR_hyp
    LDR R0, [R1, #-4]
    BIC R0, R0, #0xFF000000
    BL hyp_hvc_handler
    BL printHCR
    POP {R0-R12, LR}
    ERET

hyp_prefetch_abort_handler_ :
    PUSH {R0-R12, LR}
    BL hyp_prefetch_abort_handler
    POP {R0-R12, LR}
    ERET

hyp_data_abort_handler_ :
    PUSH {R0-R12, LR}
    BL hyp_data_abort_handler
    POP {R0-R12, LR}
    ERET

hyp_trap_handler_:
    BL hyp_trap_handler
    B _hyp_trap_handler_

hyp_irq_handler_:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL hyp_irq_handler
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    ERET
hyp_firq_handler_:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL hyp_firq_handler
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    ERET


/* secure supervisor Handlers */
sec_supervisor_undefined_instr_handler_:
    PUSH {R0-R12, LR}
    SUB R0, LR, #4
    BL sec_supervisor_undefined_instr_handler
    POP {R0-R12, LR}
    BX LR

sec_supervisor_svc_handler_:
    PUSH {R0-R12, LR}
    MOV R1, LR
    LDR R0, [R1, #-4]
    BIC R0, R0, #0xFF000000
    BL sec_svc_handler
    POP {R0-R12, LR}
    BX LR

sec_supervisor_prefetch_abort_handler_ :
    PUSH {R0-R12, LR}
    BL sec_supervisor_prefetch_abort_handler
    POP {R0-R12, LR}
    BX LR

sec_supervisor_data_abort_handler_ :
    PUSH {R0-R12, LR}
    BL sec_supervisor_data_abort_handler
    POP {R0-R12, LR}
    BX LR

sec_supervisor_trap_handler_:
    
    BL sec_supervisor_trap_handler
    B sec_supervisor_trap_handler_

sec_supervisor_irq_handler_:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL sec_supervisor_irq_handler
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    SUBS PC, LR, #4
sec_supervisor_firq_handler_:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL sec_supervisor_firq_handler
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    SUBS PC, LR, #4


/* Non secure supervisor Handlers */
supervisor_undefined_instr_handler_:
    PUSH {R0-R3, LR}
    SUB R0, LR, #4
    BL supervisor_undefined_instr_handler
    POP {R0-R3, LR}
    MOVS PC, LR

supervisor_svc_handler_:
    PUSH {R0-R3, LR}
    MOV R1, LR
    LDR R0, [R1, #-4]
    BIC R0, R0, #0xFF000000
    BL supervisor_svc_handler
    POP {R0-R3, LR}
    MOVS PC, LR

supervisor_prefetch_abort_handler_ :
    PUSH {R0-R3, LR}
    SUB R0, LR, #4
    BL supervisor_prefetch_abort_handler
    POP {R0-R3, LR}
    SUBS PC, LR, #4

supervisor_data_abort_handler_ :
    PUSH {R0-R3, LR}
    SUB R0, LR, #8
    BL supervisor_data_abort_handler
    POP {R0-R3, LR}
    SUBS PC, LR, #8

supervisor_trap_handler_:
    
    BL supervisor_trap_handler
    B supervisor_trap_handler_

supervisor_irq_handler_:
    PUSH {R0-R12, LR}
    BL supervisor_irq_handler
    POP {R0-R12, LR}
    SUBS PC, LR, #4

supervisor_firq_handler_:
    PUSH {R0-R12, LR}
    BL supervisor_firq_handler
    POP {R0-R12, LR}
    SUBS PC, LR, #4


/* monitor Handlers */
monitor_undefined_instr_handler_:
    SUB R0, LR, #4
    BL monitor_undefined_instr_handler
    B monitor_undefined_instr_handler_

monitor_smc_handler_:
    PUSH {R0-R12, LR}
    MOV R1, LR
    LDR R0, [R1, #-4]
    BIC R0, R0, #0xFF000000
    BL monitor_smc_handler
    POP {R0-R12, LR}
    BX LR

monitor_prefetch_abort_handler_ :
    PUSH {R0-R12, LR}
    BL monitor_prefetch_abort_handler
    POP {R0-R12, LR}
    BX LR

monitor_data_abort_handler_ :
    PUSH {R0-R12, LR}
    BL monitor_data_abort_handler
    POP {R0-R12, LR}
    BX LR

monitor_trap_handler_:
    
    B monitor_trap_handler_

monitor_irq_handler_:
    PUSH {R0-R12, LR}
    BL monitor_irq_handler
    POP {R0-R12, LR}
    SUBS PC, LR, #4
monitor_firq_handler_:
    PUSH {R0-R12, LR}
    BL monitor_firq_handler
    POP {R0-R12, LR}
    SUBS PC, LR, #4



//monitor_disable_secure_mode:

//    CMP R0, #MON_SERVICE_DI_SECURE_MODE

//    BX LR



_VFP_enable:
   PUSH {R0-R1}
   // Enable VFP ------------------------------------------------------------
   // r1 = Access Control Register
   MRC P15, #0, R1, C1, C0, #2
   // enable full access for p10,11
   ORR R1, R1, #(0xf << 20)
   // Access Control Register = r1
   MCR P15, #0, R1, C1, C0, #2
   MOV r1, #0
   // flush prefetch buffer because of FMXR below
   MCR P15, #0, R1, C7, C5, #4
   // and CP 10 & 11 were only just enabled
   // Enable VFP and NEON
   LDR R0,=#0x40000000 // 0x1 << 30
   // FPEXC = r0
   VMSR FPEXC, R0
   POP {R0-R1}
   BX LR


_HVC_enable:

    BX LR

_SMC_enable: //
    MRC P15, 4, R1, C1, C1, 0
    BIC R1, R1, #HYP_HCR_TSC
    MCR P15, 4, R1, C1, C1, 0
    BX LR


_get_stack_pointer:
    str     sp, [sp]
    ldr     r0, [sp]
    BX LR

.global get_midr
get_midr:
    MRC P15, 0, R0, C0, C0, 0
    BX LR

.global _get_mpidr
_get_mpidr:
    MRC P15, 0, R0, C0, C0, 5
    BX LR

.global get_CPACR
get_CPACR:
    MRC P15, 0, R0, C1, C0, 2 // Read Coprocessor Access Control Register
    BX LR

.global get_CPSR
get_CPSR:
    MRS R0, CPSR
    BX LR

.global getSPSR
getSPSR:
    MRS R0, SPSR
    BX LR

.global get_FPSID
get_FPSID:
    VMRS R0,FPSID
    BX LR

// Actually I have to check first
// if we are in Hypervisor Mode
.global get_HSCTLR
get_HSCTLR:
    MRC P15, 4, R0, C1, C0, 0
    BX LR


.global get_HSR
get_HSR:
    MRC P15, 4, R0, C1, C0, 0
    BX LR

.global getHDCR
getHDCR:
    MCR P15, 4, R0, C1, C1, 1
    BX LR

.global getLinkRegister
getLinkRegister:
    MOV R0, LR
    BX LR

.global _interrupt_enable
_interrupt_enable:
    PUSH {R0}
    CPSIE if
    POP {R0}
    BX LR

.global _interrupt_disable
_interrupt_disable:
    PUSH {R0, LR}
    CPSID if
    POP {R0, LR}
    BX LR


.global _enter_user_mode
_enter_user_mode:
    PUSH {LR}
    MSR CPSR_c, #0x50 // 0101 0000; IFT Bits = 010 --> FIQ masked ;Mode = 10000 --> User mode
    POP {LR}
    BX LR

.global __wfi
__wfi:
    PUSH {R0, LR}
    WFI
    POP {R0, LR}
    BX LR
.end