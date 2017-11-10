#include <supervisor.h>

volatile int sup_timer_ticks = 0;
volatile uint32_t current_value = 0;
volatile Event_Status_Reg sup_reg_content;
volatile Event_Status_Reg sup_gpio_reg;
void sec_supervisor_undefined_instr_handler(int *iaddr)
{
    printf("%s :instruction addr : %X\n",__PRETTY_FUNCTION__, iaddr);
}

void sec_svc_handler(unsigned int arg)
{
    printf("%s call id : %u\n",__PRETTY_FUNCTION__, arg);
}

void sec_supervisor_prefetch_abort_handler(unsigned int *op_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void sec_supervisor_data_abort_handler(unsigned int *data_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void sec_supervisor_irq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void sec_supervisor_firq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void sec_supervisor_trap_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void supervisor_undefined_instr_handler(int *iaddr)
{
    printf("%s :instruction addr : %X\n",__PRETTY_FUNCTION__, iaddr);
    print_cpu_core();
    printCpuMode();
}

void supervisor_svc_handler(unsigned int arg)
{
    printf("%s call id : %u\n",__PRETTY_FUNCTION__, arg);
}

void supervisor_prefetch_abort_handler(unsigned int *op_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void supervisor_data_abort_handler(unsigned int *data_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void supervisor_irq_handler()
{
    getArmTimer()->IRQClear = 1;
    current_value = getArmTimer()->Value;
    //static int ticks = 0;
    //ticks = ticks + 1;
    sup_timer_ticks++;
    printf("\nIRQ %d\n", sup_timer_ticks);
    printf("Timer Counter : %d\n", current_value);
    //sup_reg_content = gpio_event_status_register();
    //sup_gpio_reg = gpio_get_pin_level_register();
    //gpio_clear_event_detect(PIN_26);
    //printf("leaving %s\n", __PRETTY_FUNCTION__);
}

void supervisor_firq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void supervisor_trap_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void print_cpu_core()
{
    unsigned int core = 0;
    __asm("MRC P15, 0, %0, C0, C0, 5\n"
          "AND %0, %0, #0x03\n\t"
          : "=r"(core));
    printf("CPUCore running : %d\n", core);
}
