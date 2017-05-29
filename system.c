#include "system.h"

static IRQ_REGISTERS *irq_controller = (IRQ_REGISTERS*)IRQ_REG_BASE;
char *str_abort = "\nABORT IRQ \n";
char *str_fast = "\nFAST IRQ \n";
char *str_pref  = "\nPREFETCH ABORT\n";
char *str_undef = "\nUNDEF_IRQ\n";
char *str_svc   = "\nSupervsor Call IRQ\n";
char *str_data_abort = "\nData Abort IRQ\n";
char *str_hyp_trap = "\nHYP TRAP\n";
char str_irq[32] = "IRQ IRQ %d\n";
void printProcessorInfo()
{

    char str[512 + 1]  = {0};
    //str[512] = 0;
    int len = 0;
    //memset(str, 0, 512);
    unsigned int entry[6] = {0};
    unsigned int  midr = get_midr();
    unsigned int cpacr = get_CPACR();
    unsigned int cpsr = get_CPSR();
    uint32_t fpsid = get_FPSID();
    //entry[0] = entry[1] = entry[2] = entry[3] = entry[4] = 0;
    entry[0] = getArchitecture(midr);
    entry[1] = getImplementer(midr);
    entry[2] = getVariant(midr);
    entry[3] = getPrimaryNumber(midr);
    entry[4] = getRevision(midr);
    sprintf(str, "\nRaspberry Processor Info : \n"
                 "-----------------------------\n"
                 "Architecture      : 0x%X\n"
                 "Implementer       : 0x%X\n"
                 "Variant           : 0x%X\n"
                 "Pt Number         : 0x%X\n"
                 "Revision          : 0x%X\n"
                 "-------------------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    len = strlen(str);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str, len);
    entry[0] = getASEDIS(cpacr);
    entry[1] = getD32DIS(cpacr);
    entry[2] = getCP10AccessState(cpacr);
    entry[3] = getCP11AccessState(cpacr);
    memset(str, 0, 512);
    sprintf(str, "CP10 AND C11 ACCESS INFO :\n"
                 "---------------------------\n"
                 "ASEDIS            : 0x%X\n"
                 "D32DIS            : 0x%X\n"
                 "CP10 ACCESS       : 0x%X\n"
                 "CP11 ACCESS       : 0x%X\n"
                 "-------------------------------\n",
            entry[0], entry[1], entry[2], entry[3]);
    len = strlen(str);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str, len);

    entry[0] = getCPSRMode(cpsr);
    entry[1] = getIndianess(cpsr);
    entry[2] = getIRQState(cpsr);
    entry[3] = getFIRQState(cpsr);
    entry[4] = getIntState(cpsr);
    memset(str, 0, 512);
    sprintf(str, "CPSR REGISTER  INFO :\n"
                 "---------------------------\n"
                 "CPU MODE          : 0x%X\n"
                 "Indianess         : 0x%X\n"
                 "IRQ state         : 0x%X\n"
                 "F IRQ State       : 0x%X\n"
                 "INTERRUPT State   : 0x%X\n"
                 "-------------------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4]);
    len = strlen(str);
    mini_uart_stream(str, len);
    // FPSID
    entry[0] = getFPSIDImplementer(fpsid);
    entry[1] = getFPSIDSW(fpsid);
    entry[2] = getFPSIDSubarchitecture(fpsid);
    entry[3] = getFPSIDPartNumber(fpsid);
    entry[4] = getFPSIDVariant(fpsid);
    entry[5] = getFPSIDRevision(fpsid);
    memset(str, 0, 512);
    sprintf(str, "FPSID Info : \n"
                 "-----------------------------\n"
                 "Implementer       : 0x%X\n"
                 "SW                : 0x%X\n"
                 "Sub Arch          : 0x%X\n"
                 "Part Number       : 0x%X\n"
                 "Variant           : 0x%X\n"
                 "Revision          : 0x%X\n"
                 "-------------------------------\n",
            entry[0], entry[1], entry[2], entry[3], entry[4], entry[5]);
    len = strlen(str);
    // actually needs to make sure the uart is enabled ...
    mini_uart_stream(str, len);
    core_blink(ACK_LED, 2);
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



IRQ_REGISTERS *getIRQREGISTERS()
{
    return irq_controller;
}


void __attribute__((interrupt("ABORT"))) reset_vector(void)
{
    mini_uart_stream(str_abort, strlen(str_abort));
    //core_blink(PIN_35,1);
}

void __attribute__((interrupt("UNDEF"))) undefined_instr_vector(void)
{
    mini_uart_stream(str_undef, strlen(str_undef));
    //core_blink(PIN_35,2);
}

void __attribute__((interrupt("UNDEF"))) prefetch_abort_vector(void)
{
    mini_uart_stream(str_pref, strlen(str_pref));
    //core_blink(PIN_35,3);
}
void __attribute__((interrupt("SWI"))) swi_vector(void)
{
    mini_uart_stream(str_svc, strlen(str_svc));
    //core_blink(PIN_35,4);
}

void __attribute__((interrupt("ABORT"))) data_abort_vector(void)
{
    mini_uart_stream(str_data_abort, strlen(str_data_abort));
    //core_blink(PIN_35,5);
}

// Hyp Mode Trap
void trap_irq_hanlder(void)
{
    mini_uart_stream(str_hyp_trap, strlen(str_hyp_trap));
    //core_blink(ACK_LED,10);
}

void __attribute__((interrupt("IRQ")))
 interrupt_vector(void)
{
    //    static int lit = 0;
        static int ticks = 0;
    //    static int seconds = 0;
    getArmTimer()->IRQClear = 1;
    //core_blink(ACK_LED,1);
    //    ticks++;
    //    if( ticks > 1 )
    //        {
    //            ticks = 0;

    //            /* Calculate the FPS once a minute */
    //            seconds++;
    //            if( seconds > 59 )
    //            {
    //                seconds = 0;
    //            }
    //        }

    //        /* Flip the LED */
    //        if( lit )
    //        {
    //            deassert(PIN_35);
    //            lit = 0;
    //        }
    //        else
    //        {
    //            assert(PIN_35);
    //            lit = 1;
    //        }
    ticks++;
    //sprintf(str_irq, str_irq,ticks);
    mini_uart_send(ticks);
    //mini_uart_stream(str_irq, strlen(str_irq));
}

void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void)
{
    mini_uart_stream(str_fast, strlen(str_fast));
}

void irqEnableTimerIrq()
{
    getIRQREGISTERS()->Enable_Basic_IRQs = ARM_TMER_IRQ;
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
