#ifndef SYSTEM_H
#define SYSTEM_H
#include <mu_uart.h>
#include <timer.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// defined in boot.s
extern void _interrupt_enable();
extern unsigned int get_midr();
// MIDR REGISTER
#define MIDR_REVISION_MASK              (0xF << 0)
#define MIDR_PRIMARY_NUMBER_MASK        (0xFFF << 4)
#define MIDR_ARCH_MASK                  (0xF << 16)
#define MIDR_VARIANT_MASK               (0xF << 20)
#define MIDR_IMPLEMENTER_MASK           (0xFF << 24)
//------------------------------------------------
// CPACR REGISTER
#define CPACR_ASEDIS_MASK               (1 << 31)
#define CPACR_D32DIS_MASK               (1 << 30)
#define CPACR_CP11_ACCESS_MASK          (0x3 << 22)
#define CPACR_CP10_ACCESS_MASK          (0x3 << 20)
//------------------------------------------------
// CPSR REGISTER
#define CPSR_MODE_MASK                  (0x1F << 0)
#define CPSR_INDIANESS                  (1 << 9)
#define CPSR_MASKBITS_MASK              (0x7 << 6)
#define CPSR_IRQ_MASK                   (1 << 7)
#define CPSR_IRQ_MASK                   (1 << 7)
#define CPSR_F_IRQ_MASK                 (1 << 6)
//------------------------------------------------
// FPSID REGISTER
#define FPSID_IMPLEMENTER_MASK           (0xFF << 24)
#define FPSID_SW_MASK                    (1 << 23)
#define FPSID_SUBARCHITECTURE_MASK       (0x7F << 16)
#define FPSID_PARTNUMBER_MASK            (0xFF << 8)
#define FPSID_VARIANT_MASK               (0xF << 4)
#define FPSID_REVISION_MASK              (0xF << 0)
//------------------------------------------------
// INTERRUPT CONTROLLER ADDRESS
#define IRQ_REG_BASE                    (PERI_BASE + 0xB200)
#define ARM_TMER_IRQ                    (1 << 0)
#define ARM_MAILBOX_IRQ                 (1 << 1)
#define ARM_DOORBELL0_IRQ               (1 << 2)
#define ARM_DOORBELL1_IRQ               (1 << 3)
#define ARM_GPU0_HALTED_IRQ             (1 << 4)
#define ARM_GPU1_HALTED_IRQ             (1 << 5)
#define ARM_ILLEGAL_ACCESS_TYPE0_IRQ    (1 << 6)
#define ARM_ILLEGAL_ACCESS_TYPE1_IRQ    (1 << 7)

typedef struct IRQ_REGISTERS
{
    volatile uint32_t IRQ_Basic_Pending;
    volatile uint32_t IRQ_Pending_1;
    volatile uint32_t IRQ_Pending_2;
    volatile uint32_t FIQ_Control;
    volatile uint32_t Enable_IRQ_1;
    volatile uint32_t Enable_IRQ_2;
    volatile uint32_t Enable_Basic_IRQs;
    volatile uint32_t Disable_IRQs_1;
    volatile uint32_t Disable_IRQs_2;
    volatile uint32_t Disable_Basic_IRQs;
}IRQ_REGISTERS;

extern IRQ_REGISTERS *getIRQREGISTERS(void);
extern void irqEnableTimerIrq();

//------ MAIN ID REGISTER
uint32_t getRevision(uint32_t midr);
uint32_t getPrimaryNumber(uint32_t midr);
uint32_t getArchitecture(uint32_t midr);
uint32_t getVariant(uint32_t midr);
uint32_t getImplementer(uint32_t midr);
void printProcessorInfo();
//-----------------------------

// COPROCESSOR ACCESSC CONTROL REGISTER
// CONTROL ACCESS TO  COPROCESSOR CP10 AND CP11
//--------------------------------------------------
extern unsigned int get_CPACR();
uint32_t getCP10AccessState(uint32_t cpacr);
uint32_t getCP11AccessState(uint32_t cpacr);
uint32_t getASEDIS(uint32_t cpacr);
uint32_t getD32DIS(uint32_t cpacr);
//-------------------------------------------------

// CPSR Register
extern unsigned int get_CPSR();
uint32_t getIntState(uint32_t cpsr);
uint32_t getCPSRMode(uint32_t cpsr);
uint32_t getIRQState(uint32_t cpsr);
uint32_t getFIRQState(uint32_t cpsr);
uint32_t getIndianess(uint32_t cpsr);


//-------------------------------------------------
// FPSID Register
extern uint32_t get_FPSID();
uint32_t getFPSIDRevision(uint32_t fpsid);
uint32_t getFPSIDImplementer(uint32_t fpsid);
uint32_t getFPSIDSW(uint32_t fpsid);
uint32_t getFPSIDSubarchitecture(uint32_t fpsid);
uint32_t getFPSIDPartNumber(uint32_t fpsid);
uint32_t getFPSIDVariant(uint32_t fpsid);
#endif // SYSTEM_H
