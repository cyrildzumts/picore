#include <supervisor.h>

extern uint32_t _core_started;
volatile int sup_timer_ticks = 0;
volatile int pin_26_recent_triggerer = 0;
volatile int pin_26_tmp_triggerer = 0;
volatile uint32_t current_value = TIMER_DEFAULT_LOAD;
volatile uint32_t previous_value = TIMER_DEFAULT_LOAD;
volatile reg64_t sup_reg_content;
volatile reg64_t sup_gpio_reg;
volatile uint32_t run_demo = 0;

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
    printf("%s :instruction addr : %X - cores : %d\n",__PRETTY_FUNCTION__, iaddr, _core_started);
    print_cpu_core();
    printCPSRState();
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
    if(getArmTimer()->RAWIRQ){
        sup_timer_ticks++;
        run_demo = 1;
        getArmTimer()->IRQClear = 1;
    }
    printf("IRQ %d triggered - Cores : %d\n",sup_timer_ticks, _core_started);
}

void supervisor_irq_handler_old()
{

    previous_value = current_value;
    current_value = getArmTimer()->Value;
    if(getArmTimer()->RAWIRQ){
        sup_timer_ticks++;
        run_demo = 1;
        getArmTimer()->IRQClear = 1;
    }
    printf("\nIRQ %d - Timer value : %d  - Timer Previous Value : %d - Duration : %d\n", sup_timer_ticks, current_value, previous_value, previous_value -current_value);
    //printf("Timer Counter : %d\n", current_value);
    sup_reg_content = gpio_event_status_register();
    int event_on_pin_26 = (sup_reg_content.low & BITS_L_SHITF(0x1, PIN_26)) >> PIN_26;
    if(event_on_pin_26){
        gpio_clear_event_detect(PIN_26);
        if((pin_26_recent_triggerer != sup_timer_ticks) && (previous_value -current_value) > PUSH_BTN_BOUNCING_DELAY){
            pin_26_tmp_triggerer = pin_26_recent_triggerer;
            pin_26_recent_triggerer = sup_timer_ticks;
            printf("PIN_26 Triggered  : Recent %d -  TMP : %d - Duration : %d\n", pin_26_recent_triggerer, pin_26_tmp_triggerer, pin_26_recent_triggerer - pin_26_tmp_triggerer);
            //run_demo = 1;
            //gpio_cm_info("********** PWM Clock Manager Info **********\n", gpio_get_pwm_cm_registers()->GP0CTL, gpio_get_pwm_cm_registers()->GP0DIV);
            //pwm_transmit();
        }
    }
    
}

void supervisor_firq_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}

void supervisor_trap_handler()
{
    printf("%s\n",__PRETTY_FUNCTION__);
}