#include <events.h>


ARM_IRQ_REGISTERS *event_arm_irq_reg = (ARM_IRQ_REGISTERS*) IRQ_BASIC_BASE;

ARM_IRQ_REGISTERS* event_get_arm_irq_registers(){
    return event_arm_irq_reg;
}


void irqARMTimerEnable(){
    event_arm_irq_reg->ENABLE_BASIC_IRQ = EVENT_ARM_TIMER_IRQ;
}
void irqARMTimerDisable(){
    event_arm_irq_reg->DISABLE_BASIC_IRQ = EVENT_ARM_TIMER_IRQ;
}

uint32_t irq_is_arm_timer_pending(){
    return (event_arm_irq_reg->IRQ_BASIC_PENDING & ARM_TIMER_PENDING);
}

uint32_t irq_is_arm_basic_pending(){
    return (event_arm_irq_reg->IRQ_BASIC_PENDING & BITS_L_SHITF(0x3, 8)) >> 8;
}

uint32_t irq_is_arm_basic_pending1(){
    return (event_arm_irq_reg->IRQ_BASIC_PENDING & PENDING_IRQ_1) >> 8;
}
uint32_t irq_is_arm_basic_pending2(){
    return (event_arm_irq_reg->IRQ_BASIC_PENDING & PENDING_IRQ_2) >> 9;
}
void irq_interrupt_enable(int source){
    if(source < 32){
        event_arm_irq_reg->ENABLE_IRQ1 |= BITS_L_SHITF(0x1, source);
    }else{
        event_arm_irq_reg->ENABLE_IRQ2 |= BITS_L_SHITF(0x1, (source - 32));
    }
}
void irq_interrupt_disable(int source){
    if(source < 32){
        event_arm_irq_reg->DISABLE_IRQ1 |= BITS_L_SHITF(0x1, source);
    }else{
        event_arm_irq_reg->DISABLE_IRQ2 |= BITS_L_SHITF(0x1, (source - 32));
    }
}
void irq_base_interrupt_enable(int source){
    event_arm_irq_reg->ENABLE_BASIC_IRQ |= BITS_L_SHITF(0x1, source);
}
void irq_base_interrupt_disable(int source){
    event_arm_irq_reg->DISABLE_BASIC_IRQ |= BITS_L_SHITF(0x1, source);
}