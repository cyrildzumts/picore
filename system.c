#include "system.h"

static IRQ_REGISTERS *irq_controller = (IRQ_REGISTERS*)IRQ_REG_BASE;
char *str_abort = "\nABORT IRQ \n";
char *str_fast = "\nFAST IRQ \n";
char *str_pref  = "\nPREFETCH ABORT\n";
char *str_undef = "\nUNDEF_IRQ\n";
char *str_svc   = "\nSupervsor Call IRQ\n";
char *str_data_abort = "\nData Abort IRQ\n";
char *str_hyp_trap = "\nHYP TRAP\n";
char str_irq[128] = "\nIRQ IRQ %d\n";
char *str_hello_irq = "Hello IRQ\n";
//    static int lit = 0;
volatile int ticks = 0;
volatile Event_Status_Reg reg_content;
volatile Event_Status_Reg gpio_reg;
//    static int seconds = 0;
static char str[512 + 1]  = {0};
static unsigned int entry[12] = {0};
void printProcessorInfo(void)
{
    unsigned int  midr = get_midr();
    entry[0] = getArchitecture(midr);
    entry[1] = getImplementer(midr);
    entry[2] = getVariant(midr);
    entry[3] = getPrimaryNumber(midr);
    entry[4] = getRevision(midr);
    sprintf(str, "\nRaspberry Processor Info : \n"
                 "---------------------\n"
                 "Architecture   : 0x%X\n"
                 "Implementer    : 0x%X\n"
                 "Variant        : 0x%X\n"
                 "Pt Number      : 0x%X\n"
                 "Revision       : 0x%X\n"
                 "---------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);
}

uint32_t getPrimaryNumber(uint32_t midr)
{
    return (midr & MIDR_PRIMARY_NUMBER_MASK) >> 4;
}

uint32_t getVariant(uint32_t midr)
{
    return (midr & MIDR_VARIANT_MASK) >> 20;
}

uint32_t getImplementer(uint32_t midr)
{
    return (midr & MIDR_IMPLEMENTER_MASK) >> 24;
}

uint32_t getArchitecture(uint32_t midr)
{
    return (midr & MIDR_ARCH_MASK) >> 16;
}

uint32_t getRevision(uint32_t midr)
{
    return midr & MIDR_REVISION_MASK;
}

//  CPACR Register access functions
uint32_t getCP10AccessState(uint32_t cpacr)
{
    return (cpacr & CPACR_CP10_ACCESS_MASK) >> 20;
}

uint32_t getCP11AccessState(uint32_t cpacr)
{
    return (cpacr & CPACR_CP11_ACCESS_MASK) >> 22;
}

uint32_t getASEDIS(uint32_t cpacr)
{
    return (cpacr & CPACR_ASEDIS_MASK) >> 31;
}

uint32_t getD32DIS(uint32_t cpacr)
{
    return (cpacr & CPACR_D32DIS_MASK) >> 30;
}

//-------------------------------------------
// CPSR REGISTER
uint32_t getIntState(uint32_t cpsr)
{
    return (CPSR_MASKBITS_MASK & cpsr) >> 6;
}

uint32_t getCPSRMode(uint32_t cpsr)
{
    return (cpsr & CPSR_MODE_MASK);
}

uint32_t getIRQState(uint32_t cpsr)
{
    return (cpsr & CPSR_IRQ_MASK) >> 7;
}

uint32_t getFIRQState(uint32_t cpsr)
{
    return (cpsr & CPSR_F_IRQ_MASK) >> 8;
}

uint32_t getIndianess(uint32_t cpsr)
{
    return (cpsr & CPSR_INDIANESS) >> 9;
}



IRQ_REGISTERS *getIRQREGISTERS(void)
{
    return irq_controller;
}
void irqEnableTimerIrq()
{
    getIRQREGISTERS()->Enable_Basic_IRQs = ARM_TMER_IRQ;
}

void  reset_vector(void)
{
    mini_uart_stream(str_abort);
}

void  undefined_instr_vector(void)
{

    //printHSRState();
    //printDebugState();
    printCPSRState();
    printSPSRState();
    printLinkRegister();
    mini_uart_stream(str_undef);

    //RAIO_print(str_undef);
}

void  prefetch_abort_vector(void)
{
    mini_uart_stream(str_pref);
}
void  swi_handler(void)
{
    mini_uart_stream(str_svc);
}

void data_abort_vector(void)
{
    mini_uart_stream(str_data_abort);
}

// Hyp Mode Trap
void trap_irq_hanlder(void)
{
    mini_uart_stream(str_hyp_trap);
}

void // __attribute__((interrupt("IRQ")))
interrupt_vector(void)
{
    //getArmTimer()->IRQClear = 1;
    //static int ticks = 0;
    ticks = ticks + 1;
    printf("\nIRQ %d\n", ticks);
    reg_content = gpio_event_status_register();
    gpio_reg = gpio_get_pin_level_register();
    handleEvent();
    gpio_clear_event_detect(PIN_22);
    //gpio_clear_event_detect(PIN_29);
}

void fast_interrupt_vector(void)
{
    mini_uart_stream(str_fast);
}

void handleEvent()
{
    printf("\n----------------------------------------------\n"
           "New Event Caught. Debuging starts : \n");
    printf("GPIO Event status Register LOW : %X\n"
           "GPIO Event Status Register HIGH : %X\n"
           "IRQ Register Pending 1 : %X\n"
           "IRQ Register Pending 2 : %X\n"
           "----------------------------------------------\n",
           reg_content.low, reg_content.high,
           getIRQREGISTERS()->IRQ_Pending_1,
           getIRQREGISTERS()->IRQ_Pending_2);
    printf("\n-------------------------------------\n");
    printf("GPIO PIN LEVEL STATUS : \n"
           "LOW : %X\n"
           "HIGH: %X\n", gpio_reg.low, gpio_reg.high);
    printf("\n-------------------------------------\n");
    reg_content.high = 0;
    reg_content.low = 0;
    gpio_reg = reg_content;
}

uint32_t getFPSIDRevision(uint32_t fpsid)
{
    return (fpsid & FPSID_REVISION_MASK) >> 0 ;
}

uint32_t getFPSIDImplementer(uint32_t fpsid)
{
    return (fpsid & FPSID_IMPLEMENTER_MASK) >> 24;
}

uint32_t getFPSIDSW(uint32_t fpsid)
{
    return (fpsid & FPSID_SW_MASK) >> 23;
}

uint32_t getFPSIDSubarchitecture(uint32_t fpsid)
{
    return (fpsid & FPSID_SUBARCHITECTURE_MASK) >> 16;
}

uint32_t getFPSIDPartNumber(uint32_t fpsid)
{
    return (fpsid & FPSID_PARTNUMBER_MASK) >> 8;
}

uint32_t getFPSIDVariant(uint32_t fpsid)
{
    return (fpsid & FPSID_VARIANT_MASK) >> 4;
}

uint32_t getHsrec(uint32_t hsr)
{
    return (hsr & HSR_EC_MASK) >> 26;
}

uint32_t getHSRCCOND(uint32_t hsr)
{
    return (hsr & HSR_COND_MASK) >> 20;
}

uint32_t getHSRIL(uint32_t hsr)
{
    return (hsr & HSR_IL_MASK) >> 25;
}

uint32_t getHSRCV(uint32_t hsr)
{
    return (hsr & HSR_CV_MASK) >> 24;
}

uint32_t getHSRISS(uint32_t hsr)
{
    return (hsr & HSR_ISS_MASK);
}

void printHSRState(void)
{
    uint32_t hsr = get_HSR();
    entry[0] = getHsrec(hsr);
    entry[1] = getHSRIL(hsr);
    entry[2] = getHSRCV(hsr);
    entry[3] = getHSRCCOND(hsr);
    entry[4] = getHSRISS(hsr);
    memset(str, 0, 512);
    sprintf(str, "\nHSR Register Info : \n"
                 "--------------------\n"
                 "EC            : 0x%X\n"
                 "IL            : 0x%X\n"
                 "CV            : 0x%X\n"
                 "COND          : 0x%X\n"
                 "ISS           : 0x%X\n"
                 "--------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);

}

void printDebugState(void)
{
    uint32_t hdcr = getHDCR();

    entry[0] = getHDCRHPMN(hdcr);
    entry[1] = getHDCRTPMCR(hdcr);
    entry[2] = getHDCRTPM(hdcr);
    entry[3] = getHDCRHPME(hdcr);
    entry[4] = getHDCRTDE(hdcr);
    entry[5] = getHDCRTDA(hdcr);
    entry[6] = getHDCRTDOSA(hdcr);
    entry[7] = getHDCRTDRA(hdcr);
    memset(str, 0, 512);
    sprintf(str, "\nHDCR Register Info : \n"
                 "---------------------\n"
                 "HPMN           : 0x%X\n"
                 "TPMCR          : 0x%X\n"
                 "TPM            : 0x%X\n"
                 "HPME           : 0x%X\n"
                 "TDE            : 0x%X\n"
                 "TDA            : 0x%X\n"
                 "TDOSA          : 0x%X\n"
                 "TDRA           : 0x%X\n"
                 "---------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5], entry[6], entry[7]);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);
}

uint32_t getHDCRHPMN(uint32_t hdcr)
{
    return (hdcr & HDCR_HPMN_MASK) >> 0;
}

uint32_t getHDCRTPMCR(uint32_t hdcr)
{
    return (hdcr & HDCR_TPMCR_MASK) >> 5;
}

uint32_t getHDCRTPM(uint32_t hdcr)
{
    return (hdcr & HDCR_TPM_MASK) >> 6;
}

uint32_t getHDCRHPME(uint32_t hdcr)
{
    return (hdcr & HDCR_HPME_MASK) >> 7;
}

uint32_t getHDCRTDE(uint32_t hdcr)
{
    return (hdcr & HDCR_TDE_MASK) >> 8;
}

uint32_t getHDCRTDA(uint32_t hdcr)
{
    return (hdcr & HDCR_TDA_MASK) >> 9;
}

uint32_t getHDCRTDOSA(uint32_t hdcr)
{
    return (hdcr & HDCR_TDOSA_MASK) >> 10;
}

uint32_t getHDCRTDRA(uint32_t hdcr)
{
    return (hdcr & HDCR_TDRA_MASK) >> 11;
}

void enableAUXIRQ()
{
    getIRQREGISTERS()->Enable_IRQ_1 |= AUX_INT_EN;
}

void disableAUXIRQ()
{
    getIRQREGISTERS()->Disable_IRQs_1 |= AUX_INT_EN;
}

void displayInit(void)
{
    printf("Initializing TFT  Display ...\n");
    static int is_initialized = 0;
    if(is_initialized == 0)
    {
        //TFT_init_board();
        //TFT_hard_reset();
        //RAIO_init();
        is_initialized = 1;
        printf("C-Berry Display initialized\n");
    }
}

uint32_t getCPUState(uint32_t cpsr)
{
    return (cpsr & CPSR_CPU_STATE_MASK) >> 5;
}

void printCP10CP11Access(void)
{
    unsigned int cpacr = get_CPACR();
    entry[0] = getASEDIS(cpacr);
    entry[1] = getD32DIS(cpacr);
    entry[2] = getCP10AccessState(cpacr);
    entry[3] = getCP11AccessState(cpacr);
    memset(str, 0, 512);
    sprintf(str, "\nCP10 AND C11 ACCESS INFO :\n"
                 "----------------------\n"
                 "ASEDIS          : 0x%X\n"
                 "D32DIS          : 0x%X\n"
                 "CP10 ACCESS     : 0x%X\n"
                 "CP11 ACCESS     : 0x%X\n"
                 "----------------------\n",
            entry[0], entry[1], entry[2], entry[3]);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);
}

void printCPSRState(void)
{
    unsigned int cpsr = get_CPSR();
    entry[0] = getCPSRMode(cpsr);
    entry[1] = getIndianess(cpsr);
    entry[2] = getIRQState(cpsr);
    entry[3] = getFIRQState(cpsr);
    entry[4] = getIntState(cpsr);
    entry[5] = getCPUState(cpsr);
    memset(str, 0, 512);
    sprintf(str, "\nCPSR REGISTER  INFO :\n"
                 "-----------------------\n"
                 "CPU MODE         : 0x%X\n"
                 "Indianess        : 0x%X\n"
                 "IRQ state        : 0x%X\n"
                 "F IRQ State      : 0x%X\n"
                 "INTERRUPT State  : 0x%X\n"
                 "CPU State T      : 0x%X\n"
                 "-----------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
    mini_uart_stream(str);
    //RAIO_print(str);
}

void printFPSID(void)
{
    // FPSID
    uint32_t fpsid = get_FPSID();
    entry[0] = getFPSIDImplementer(fpsid);
    entry[1] = getFPSIDSW(fpsid);
    entry[2] = getFPSIDSubarchitecture(fpsid);
    entry[3] = getFPSIDPartNumber(fpsid);
    entry[4] = getFPSIDVariant(fpsid);
    entry[5] = getFPSIDRevision(fpsid);
    memset(str, 0, 512);
    sprintf(str, "\nFPSID Info : \n"
                 "-----------------------\n"
                 "Implementer      : 0x%X\n"
                 "SW               : 0x%X\n"
                 "Sub Arch         : 0x%X\n"
                 "Part Number      : 0x%X\n"
                 "Variant          : 0x%X\n"
                 "Revision         : 0x%X\n"
                 "-----------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);
}

void printLinkRegister(void)
{
    uint32_t *lr_ptr = getLinkRegister();
    memset(str, 0, 512);
    sprintf(str, "\nLink Register\n"
                 "Exception at %p 0x%08lX\n",
            lr_ptr, *lr_ptr);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str);
    //RAIO_print(str);

}

void printSPSRState()
{
    unsigned int cpsr = getSPSR();
    entry[0] = getCPSRMode(cpsr);
    entry[1] = getIndianess(cpsr);
    entry[2] = getIRQState(cpsr);
    entry[3] = getFIRQState(cpsr);
    entry[4] = getIntState(cpsr);
    entry[5] = getCPUState(cpsr);
    memset(str, 0, 512);
    sprintf(str, "\nSPSR REGISTER  INFO :\n"
                 "-----------------------\n"
                 "CPU MODE         : 0x%X\n"
                 "Indianess        : 0x%X\n"
                 "IRQ state        : 0x%X\n"
                 "F IRQ State      : 0x%X\n"
                 "INTERRUPT State  : 0x%X\n"
                 "CPU State T      : 0x%X\n"
                 "-----------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
    mini_uart_stream(str);
    //RAIO_print(str);
}

void confirm(void)
{
    mini_uart_stream(__PRETTY_FUNCTION__);
}

void enableIRQ1(uint32_t mask)
{
    getIRQREGISTERS()->Enable_IRQ_1 |= mask;
}

void enableIRQ2(uint32_t mask)
{
    getIRQREGISTERS()->Enable_IRQ_2 |= mask;
}

void enable_gpio_int()
{
    enableIRQ2(GPIO_0_INT | GPIO_1_INT | GPIO_2_INT | GPIO_3_INT);
}

void arm_sleep()
{
    __wfi();
}
