.text
.global _start
.global _reset
.global _get_stack_pointer
.extern __bss_start__
.extern __bss_end__
.extern _stack

.equ    HYP_STACK,              0x80000
.equ    SVC_STACK,              HYP_STACK + 0x400
.equ    UNDEF_STACK,            SVC_STACK + 0x400
.equ    IRQ_STACK,              UNDEF_STACK + 0x400

.equ    NO_INT,                 0xC0 // disable Interrupts

.equ    INT_EN_MASK,            0x80
.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_HYP,          0x1A
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20
.equ    DEFAULT_BAUD,           270

_start: /* This is our Entry point */
        LDR PC, _reset_handler_ /* called when the system is reset */
        LDR PC, _undef_inst_vector_
        LDR PC, _swi_vector_ /* SWI */
        LDR PC, _prefetch_abort_vector_ /* Prefetch Abort */
        LDR PC, _data_abort_vetor_ /* Data Abort */
        LDR PC, _hypervisor_vector_ /* reserved */
        LDR PC, _irq_vector_ /* IRQ */
        LDR PC, _firq_vector_ /* FIQ */
	
_reset_handler_         :       .word _reset_
_undef_inst_vector_     :       .word undefined_instr
_swi_vector_            :       .word swi_vector /* SWI */
_prefetch_abort_vector_ :       .word prefetch_abort_vector /* Prefetch Abort */
_data_abort_vetor_      :       .word data_abort_vector /* Data Abort */
_hypervisor_vector_     :       .word trap_irq /* reserved */
_irq_vector_            :       .word irq /* IRQ */
_firq_vector_           :       .word fast_interrupt_vector /* FIQ */




_reset_:
    // On boot I first check in what mode we are
    // Disable Interrupt first
    //CPSID if
    //Copy Interrupt vector to its place
    // LDR R0, =_start
    // MOV R1, 0x0000

    //SET HYP MODE SP
    // Initialize HVBAR:
    // Set HVBAR to the vector base table
    MOV R0, #0x8000
    MCR P15, 4, R0, C12, C0, 0

    MOV SP, #0x80000
    MSR CPSR_c, #NO_INT | CPSR_MODE_UNDEFINED
    LDR SP, =#UNDEF_STACK
    MSR CPSR_c, #NO_INT | CPSR_MODE_IRQ
    LDR SP, =#IRQ_STACK
    MSR CPSR_c, #NO_INT | CPSR_MODE_HYP

// Enable VFP ------------------------------------------------------------

    // r1 = Access Control Register
    MRC p15, #0, r1, c1, c0, #2
    // enable full access for p10,11
    ORR r1, r1, #(0xf << 20)
    // ccess Control Register = r1
    MCR p15, #0, r1, c1, c0, #2
    MOV r1, #0
    // flush prefetch buffer because of FMXR below
    MCR p15, #0, r1, c7, c5, #4
    // and CP 10 & 11 were only just enabled
    // Enable VFP and NEON 
    LDR r0,=#0x40000000 // 0x1 << 30 
    // FPEXC = r0
    VMSR FPEXC, r0

// clear bss sectons ...
    ldr r0, =__bss_start__
    ldr r1, =__bss_end__
    mov r2, #0

bss_clear_loop:
    cmp     r0,r1
    it      lt
    strlt   r2,[r0], #4
    blt     bss_clear_loop
// AUX UART INIT :

    LDR R0, =#270
    BL mu_init
    BL displayInit
    BL boot_init
    BL main
trap:   B trap

_get_stack_pointer:
    // Return the stack pointer value
    str     sp, [sp]
    ldr     r0, [sp]
    // Return from the function
    BX LR

.global get_midr
get_midr:
    MRC P15, 0, R0, C0, C0, 0
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
    MRS R0, CPSR
    BIC R0, R0, #INT_EN_MASK
    MSR CPSR_c, R0
    POP {R0}
    BX LR

.global _interrupt_disable
_interrupt_disable:
    PUSH {R0}
    MRS R0, CPSR
    ORR R0, R0, #INT_EN_MASK
    MSR CPSR_c, R0
    POP {R0}
    BX LR

swi_vector:

    PUSH {R0-R12, LR}
    BL swi_handler
    POP {R0-R12, LR}
    ERET

irq:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL interrupt_vector
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    ERET

undefined_instr:
    PUSH {R0-R12, LR}
    BL undefined_instr_vector
    POP {R0-R12, LR}
    BX LR

trap_irq:
    PUSH {r0, r1, r2, r3, r4, r5, ip, lr}
    BL trap_irq_hanlder
    POP {r0, r1, r2, r3, r4, r5, ip, lr}
    B trap_irq


_hypervisor_vector:
    B trap_irq
