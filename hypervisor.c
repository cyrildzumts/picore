#include <hypervisor.h>
void hyp_undefined_instr_handler(int *iaddr)
{
    printf("%s :instruction addr : %X\n",__PRETTY_FUNCTION__, iaddr);
}

void hyp_hvc_handler(unsigned int arg)
{
    printf("%s call id : %u\n",__PRETTY_FUNCTION__, arg);
}

void hyp_prefetch_abort_handler(unsigned int *op_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void hyp_data_abort_handler(unsigned int *data_addr)
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void hyp_irq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void hyp_firq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void hyp_trap_handler(){
    printf("%s\n",__PRETTY_FUNCTION__);
}
