.text
.global _start
.global _reset
.global _get_stack_pointer

.equ INT_EN_MASK, 0x80

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

.equ    CPSR_IRQ_INHIBIT,       0x80
.equ    CPSR_FIQ_INHIBIT,       0x40
.equ    CPSR_THUMB,             0x20

_start: /* This is our Entry point */
        LDR PC, _reset_handler_ /* called when the system is reset */
        LDR PC, _undef_inst_vector_
        LDR PC, _swi_vector_ /* SWI */
        LDR PC, _prefetch_abort_vector_ /* Prefetch Abort */
        LDR PC, _data_abort_vetor_ /* Data Abort */
        LDR PC, unsued_handler_ /* reserved */
        LDR PC, _interrupt_vector_ /* IRQ */
        LDR PC, _fast_interrupt_vector_ /* FIQ */
	
_reset_handler_:            .word _reset_
_undef_inst_vector_ :       .word undefined_instr_vector
_swi_vector_        :       .word swi_vector /* SWI */
_prefetch_abort_vector_ :   .word prefetch_abort_vector /* Prefetch Abort */
_data_abort_vetor_      :   .word data_abort_vector /* Data Abort */
unsued_handler_         :   .word trap_irq /* reserved */
_interrupt_vector_      :   .word irq /* IRQ */
_fast_interrupt_vector_ :   .word fast_interrupt_vector /* FIQ */




_reset_:
    MOV R0, #0x8000
    MCR P15, 4, R0, C12, C0, 0
    mov sp, #0x8000

/* Actually, the bss section needs to be zeroed first */
    //LDR sp, =_stack
/* ENABLE VFP */
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
    // Enable VFP itself
    MOV r0,#0x40000000
    // FPEXC = r0
.force_thumb
    VMSR FPEXC, r0
    //FMXR FPEXC, R0

.arm
	BL main
trap:   B trap

_get_stack_pointer:
    // Return the stack pointer value
    str     sp, [sp]
    ldr     r0, [sp]
    // Return from the function
    mov pc, lr

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

.global _interrupt_enable
_interrupt_enable:
    MRS R0, CPSR
    BIC R0, R0, #INT_EN_MASK
    MSR CPSR_c, R0
    BX LR

irq:
    push {R0-R12, LR}
    BL interrupt_vector
    pop {R0-R12, LR}
    //SUBS PC, LR, #4
    ERET

trap_irq:
    BL trap_irq_hanlder
    B trap_irq
