#include "monitor.h"
void monitor_undefined_instr_handler(int *iaddr)
{
    printf("%s :instruction addr : %X\n",__PRETTY_FUNCTION__, iaddr);
}

void monitor_smc_handler(unsigned int arg)
{
    printf("%s call id : %X\n",__PRETTY_FUNCTION__, arg);
}

void monitor_prefetch_abort_handler(unsigned int *op_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void monitor_data_abort_handler(unsigned int *data_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void monitor_irq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void monitor_firq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}
