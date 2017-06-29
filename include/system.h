#ifndef SYSTEM_H
#define SYSTEM_H
#include <mu_uart.h>
#include <timer.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <display.h>

// defined in boot.s
extern void _interrupt_enable(void);
extern unsigned int get_midr(void);
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
#define CPSR_F_IRQ_MASK                 (1 << 6)
#define CPSR_CPU_STATE_MASK             (1 << 5)
//------------------------------------------------
// FPSID REGISTER
#define FPSID_IMPLEMENTER_MASK           (0xFF << 24)
#define FPSID_SW_MASK                    (1 << 23)
#define FPSID_SUBARCHITECTURE_MASK       (0x7F << 16)
#define FPSID_PARTNUMBER_MASK            (0xFF << 8)
#define FPSID_VARIANT_MASK               (0xF << 4)
#define FPSID_REVISION_MASK              (0xF << 0)
//------------------------------------------------
// HSR REGISTER
#define HSR_EC_MASK                      (0x7F << 26)
#define HSR_COND_MASK                    (0xF << 20)
#define HSR_CV_MASK                      (0x1 << 24)
#define HSR_IL_MASK                      (1 << 25)
#define HSR_ISS_MASK                     (0x1FFFFFF << 0)
//-------------------------------------------------
// HDCR HYP DEBUG CONFIG REGISTER
#define HDCR_HPMN_MASK                   (0xF << 0)
#define HDCR_TPMCR_MASK                  (0x1 << 5)
#define HDCR_TPM_MASK                    (0x1 << 6)
#define HDCR_HPME_MASK                   (0x1 << 7)
#define HDCR_TDE_MASK                    (0x1 << 8)
#define HDCR_TDA_MASK                    (0x1 << 9)
#define HDCR_TDOSA_MASK                  (0x1 << 10)
#define HDCR_TDRA_MASK                   (0x1 << 11)

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
extern void irqEnableTimerIrq(void);
void enableAUXIRQ(void);
void disableAUXIRQ(void);

//------ MAIN ID REGISTER
uint32_t getRevision(uint32_t midr);
uint32_t getPrimaryNumber(uint32_t midr);
uint32_t getArchitecture(uint32_t midr);
uint32_t getVariant(uint32_t midr);
uint32_t getImplementer(uint32_t midr);

//-----------------------------

// COPROCESSOR ACCESSC CONTROL REGISTER
// CONTROL ACCESS TO  COPROCESSOR CP10 AND CP11
//--------------------------------------------------
extern unsigned int get_CPACR(void);
uint32_t getCP10AccessState(uint32_t cpacr);
uint32_t getCP11AccessState(uint32_t cpacr);
uint32_t getASEDIS(uint32_t cpacr);
uint32_t getD32DIS(uint32_t cpacr);

//-------------------------------------------------

// CPSR Register
extern unsigned int get_CPSR(void);
extern unsigned int getSPSR(void);
uint32_t getIntState(uint32_t cpsr);
uint32_t getCPSRMode(uint32_t cpsr);
uint32_t getIRQState(uint32_t cpsr);
uint32_t getFIRQState(uint32_t cpsr);
uint32_t getIndianess(uint32_t cpsr);
uint32_t getCPUState(uint32_t cpsr);



//-------------------------------------------------
// FPSID Register
extern uint32_t get_FPSID(void);
uint32_t getFPSIDRevision(uint32_t fpsid);
uint32_t getFPSIDImplementer(uint32_t fpsid);
uint32_t getFPSIDSW(uint32_t fpsid);
uint32_t getFPSIDSubarchitecture(uint32_t fpsid);
uint32_t getFPSIDPartNumber(uint32_t fpsid);
uint32_t getFPSIDVariant(uint32_t fpsid);


// HSCTLR Register
extern uint32_t get_HSCTLR(void);
extern uint32_t get_HSR(void);
uint32_t getHsrec(uint32_t hsr);
uint32_t getHSRCCOND(uint32_t hsr);
uint32_t getHSRIL(uint32_t hsr);
uint32_t getHSRCV(uint32_t hsr);
uint32_t getHSRISS(uint32_t hsr);


// HCRD Hyp Debug Config register
extern uint32_t getHDCR(void);

uint32_t getHDCRHPMN(uint32_t hdcr);
uint32_t getHDCRTPMCR(uint32_t hdcr);
uint32_t getHDCRTPM(uint32_t hdcr);
uint32_t getHDCRHPME(uint32_t hdcr);
uint32_t getHDCRTDE(uint32_t hdcr);
uint32_t getHDCRTDA(uint32_t hdcr);
uint32_t getHDCRTDOSA(uint32_t hdcr);
uint32_t getHDCRTDRA(uint32_t hdcr);
extern uint32_t *getLinkRegister(void);
void printLinkRegister(void);
void printDebugState(void);
void printCPSRState(void);
void printSPSRState(void);
void printFPSID(void);
void printHSRState(void);
void printCP10CP11Access(void);
void displayInit(void);
void printProcessorInfo(void);
void confirm(void);

#endif // SYSTEM_H
