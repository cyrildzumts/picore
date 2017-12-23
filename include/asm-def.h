#ifndef ASMDEF_H
#define ASMDEF_H

#define STACK_TART                  0x90000
#define STACK_END                   0x84000

#define    HYP_STACK                0x90000
#define    SVC_STACK                HYP_STACK + 0x400
#define    UNDEF_STACK              SVC_STACK + 0x400
#define    IRQ_STACK                UNDEF_STACK + 0x400
#define    SYS_STACK                IRQ_STACK + 0x400

#define    NO_INT                   0xC0 // disable Interrupts

#define    INT_EN_MASK              0x80
#define    CPSR_MODE_USER         0x10
#define    CPSR_MODE_FIQ          0x11
#define    CPSR_MODE_IRQ          0x12
#define    CPSR_MODE_SVR          0x13
#define    CPSR_MODE_ABORT        0x17
#define    CPSR_MODE_HYP          0x1A
#define    CPSR_MODE_UNDEFINED    0x1B
#define    CPSR_MODE_SYSTEM       0x1F

#define    CPSR_IRQ_INHIBIT       0x80
#define    CPSR_FIQ_INHIBIT       0x40
#define    CPSR_THUMB            0x20
#define    DEFAULT_BAUD           270
// SCR CONF
#define SCR_NS_DISABLE           0x01

// MONITOR SERVICE
#define MON_SERVICE_DI_SECURE_MODE 0x01 // DISABLE Secure MODE
#define MON_SERVICE_EN_SECURE_MODE 0x02 // ENABLE SECURE MODE



#define HYP_HCR_TSC             (1 << 19)
#define PCB_SIZE                18
#define TASK_COUNT              3

#endif // ASMDEF_H
