#include "system.h"

static IRQ_REGISTERS *irq_controller = (IRQ_REGISTERS*)IRQ_REG_BASE;

//    static int lit = 0;
volatile int ticks = 0;
volatile int _timer_ticks = 0;
volatile Event_Status_Reg reg_content;
volatile Event_Status_Reg gpio_reg;
//    static int seconds = 0;
//static char str[512 + 1]  = {0};
static unsigned int entry[12] = {0};

extern unsigned int hyp_spsr_boot;
extern unsigned int hyp_elr_boot;
extern unsigned int hyp_elr_mod;
extern unsigned int modified_spsr;
/*
PageTable masterPT  = {0x00000000, 0x18000, 0x18000, MASTER, 3};
PageTable systemPT  = {0x00000000, 0x1C000, 0x18000, COARSE, 3};
PageTable task1PT   = {0x00400000, 0x1C400, 0x18000, COARSE, 3};
PageTable task2PT   = {0x00400000, 0x1C800, 0x18000, COARSE, 3};
PageTable task3PT   = {0x00400000, 0x1CC00, 0x18000, COARSE, 3};

Region kernelRegion     = {0x00000000, 4, 16, RWNA, WRITE_THROUGH_CACHE, 0x00000000, &systemPT};
Region sharedRegion     = {0x00010000, 4, 8,  RWRW, WRITE_THROUGH_CACHE, 0x00010000, &systemPT};
Region pageTableRegion  = {0x00018000, 4, 8,  RWNA, WRITE_THROUGH_CACHE, 0x00018000, &systemPT};
Region peripheralRegion = {0x10000000, 124, 256, RWNA, CACHE_NOT_CACHED, 0x10000000, &masterPT};

Region task1Region     = {0x00400000, 4, 8, RWRW, WRITE_THROUGH_CACHE, 0x00020000, &task1PT};
Region task2Region     = {0x00400000, 4, 8, RWRW, WRITE_THROUGH_CACHE, 0x00028000, &task2PT};
Region task3Region     = {0x00400000, 4, 8, RWRW, WRITE_THROUGH_CACHE, 0x00030000, &task3PT};
*/
void printProcessorInfo(void)
{
    unsigned int  midr = get_midr();
    entry[0] = getArchitecture(midr);
    entry[1] = getImplementer(midr);
    entry[2] = getVariant(midr);
    entry[3] = getPrimaryNumber(midr);
    entry[4] = getRevision(midr);
    printf("\nRaspberry Processor Info : \n"
                 "---------------------\n"
                 "Architecture   : 0x%X\n"
                 "Implementer    : 0x%X\n"
                 "Variant        : 0x%X\n"
                 "Pt Number      : 0x%X\n"
                 "Revision       : 0x%X\n"
                 "---------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    // actually needs to make sure the uart is enabled ...
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
    printf(__PRETTY_FUNCTION__);
}

void  undefined_instr_vector(void)
{
    printf("%s\n",__PRETTY_FUNCTION__);
    printCPSRState();
}

void  prefetch_abort_vector(void)
{
    printf("%s\n",__PRETTY_FUNCTION__);
    printCPSRState();
}
void  swi_handler(int callID)
{
    printf("%s call id : %d\n",__PRETTY_FUNCTION__, callID);
}

void hyp_handler(int callID){
    printf("%s call id : %d\n",__PRETTY_FUNCTION__, callID);
}

void data_abort_vector(void)
{
    printCPSRState();
    printf("%s\n",__PRETTY_FUNCTION__);
}


// TODO : Locate the source of the IRQ.
// POSIBLE SOURCE might be :
// GPIO PIN
// TIMER
// AUX ( mini UART, mini SPI)
// DMA
// SPI & UART
// I2C
// USB
// PWM
// SD CARD
void // __attribute__((interrupt("IRQ")))
interrupt_vector(void)
{
    getArmTimer()->IRQClear = 1;
    //static int ticks = 0;
    //ticks = ticks + 1;
    _timer_ticks++;
    printf("\nIRQ %d\n", _timer_ticks);
    reg_content = gpio_event_status_register();
    gpio_reg = gpio_get_pin_level_register();
    handleEvent();
    gpio_clear_event_detect(PIN_26);
    //gpio_clear_event_detect(PIN_29);
    /* TODO : update timer hier */
}

void fast_interrupt_vector(void)
{
    printf("%s\n",__PRETTY_FUNCTION__);
    printCPSRState();
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
    printf("\nHSR Register Info : \n"
                 "--------------------\n"
                 "EC            : 0x%X\n"
                 "IL            : 0x%X\n"
                 "CV            : 0x%X\n"
                 "COND          : 0x%X\n"
                 "ISS           : 0x%X\n"
                 "--------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    // actually needs to make sure the uart is enabled ...


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
    printf("\nHDCR Register Info : \n"
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
    mmu_control_set(0,0);
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
    printf("\nCP10 AND C11 ACCESS INFO :\n"
                 "----------------------\n"
                 "ASEDIS          : 0x%X\n"
                 "D32DIS          : 0x%X\n"
                 "CP10 ACCESS     : 0x%X\n"
                 "CP11 ACCESS     : 0x%X\n"
                 "----------------------\n",
            entry[0], entry[1], entry[2], entry[3]);
    // actually needs to make sure the uart is enabled ...
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
    printf("\nCPSR REGISTER  INFO :\n"
                 "-----------------------\n"
                 "CPU MODE         : 0x%X\n"
                 "Indianess        : 0x%X\n"
                 "IRQ state        : 0x%X\n"
                 "F IRQ State      : 0x%X\n"
                 "INTERRUPT State  : 0x%X\n"
                 "CPU State T      : 0x%X\n"
                 "-----------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
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
    printf("\nFPSID Info : \n"
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

}

void printLinkRegister(void)
{
    uint32_t *lr_ptr = getLinkRegister();
    printf("\nLink Register\n"
                 "Exception at %p 0x%08lX\n",
            lr_ptr, *lr_ptr);
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
    printf("\nSPSR REGISTER  INFO :\n"
                 "-----------------------\n"
                 "CPU MODE         : 0x%X\n"
                 "Indianess        : 0x%X\n"
                 "IRQ state        : 0x%X\n"
                 "F IRQ State      : 0x%X\n"
                 "INTERRUPT State  : 0x%X\n"
                 "CPU State T      : 0x%X\n"
                 "-----------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
}

void confirm(void)
{
    printf(__PRETTY_FUNCTION__);
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
    //enableIRQ2(GPIO_0_INT | GPIO_1_INT | GPIO_2_INT | GPIO_3_INT);
    irq_controller->Enable_IRQ_2 |= (GPIO_0_INT | GPIO_1_INT | GPIO_2_INT | GPIO_3_INT);
}

void arm_sleep()
{
    __wfi();
}

void mmu_control_set(uint32_t value, uint32_t mask)
{
    uint32_t format = 0;
    asm(
    "MRC P15, 4,%0, C1, C0, 0\n\t"
    : "=r" (format));
    printf("MMU REGISTER STATE : %X\n", format);
    format &=~mask;
    format |= value;
}

int write_through_support()
{
    uint32_t wt = 0;
    asm(
    "MRC P15, 1, %0, C0, C0, 0\n\t"
    :"=r" (wt));
    return (wt & WRITE_TRHOUGH_CACHE_SUPPORT) >> 31;
}

int write_back_support()
{
    uint32_t wb = 0;
    asm(
    "MRC P15, 1, %0, C0, C0, 0\n\t"
    :"=r" (wb));
    return (wb & WRITE_BACK_CACHE_SUPPORT) >> 30;
}

int cache_is_enabled()
{
    return -1;
}

void cache_enable()
{

}

int mmu_is_enabled()
{
return -1;
}

void mmu_enable()
{

}

void mmu_init(PageTable *pt)
{

}

void mmu_mapregion(Region *region)
{

}

void mmu_map_section_tableRegion(Region *region)
{

}

void mmu_map_coarse_tableRegion(Region *region)
{

}

void mmu_map_fine_tableRegion(Region *region)
{

}

int mmu_attachPageTable(PageTable *pt)
{
return -1;
}

void mmu_domain_accessSet(uint32_t value, uint32_t mask)
{

}

uint32_t cpu_mode()
{
return -1;
}

void printCpuMode()
{
    unsigned int cpsr = 0;
    asm("MRS %0, CPSR\n\t"
        : "=r"(cpsr));
    cpsr = getCPSRMode(cpsr);
    printf("CPU MODE : %X\n", cpsr);
    spi_send(cpsr);

}

void changeMode_debug()
{
    printCpuMode();
    change_mode(UND_MODE);
    printCpuMode();
    change_mode(IRQ_MODE);
    printCpuMode();
    change_mode(HYP_MODE);
    printCpuMode();
}


extern unsigned int *_cpu_registers;
extern unsigned int *_cpu_reg;
void print_cpu_registers()
{
    int i = 0;
    uint32_t *regs = _cpu_registers;
    printf("CPU Registers content : \n");
    for (i = 0; i < 17; i++)
    {
        printf("%d : %X\n",i, regs[i]);
    }
    printf("CPU stack addr : %X\n", _cpu_registers);
    printf("cpu registers finished \n");
}
