

#include "modules.h"
#include <stdio.h>
#include <pwm.h>
//#include <ds1621.h>
#include <pca9685.h>
#include <system.h>
#include <task.h>

extern uint32_t run_demo;
int main(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);
    print_pcb_table();
    i2c_enable();
    //delayN(WAIT_150_MS);
    //i2c_init(DS1621_ADDRESS);
    //i2c_disable_clkt();
    //delayN(WAIT_150_MS);
    //ds1621_start_conversion();
    //i2c_debug(__PRETTY_FUNCTION__);
    //disableAUXIRQ();
    
    printf("enabling gpio irqs ...\n");
    gpio_set_pud(PIN_26, GPIO_PUD_UP);
    gpio_enable_low_detect(PIN_26);
    enable_gpio_int();

    //enableAUXIRQ();
    irqEnableTimerIrq();

    printf("\nEnabling interrupts\n");
    _interrupt_enable();
    printf("IRQs enabled\n");
    ArmTimeInit();
    printf("Entering sleep mode ...\n");

    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        if(run_demo){
            printf("ARM waiked up by an IRQ - Starting Demo\n");
            pca9685_led_demo();
            run_demo = 0;
            printf("ARM waiked up by an IRQ - Finished Demo\n");
        }
    }
    return 0;
}
