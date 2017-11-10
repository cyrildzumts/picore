

#include "modules.h"
#include <stdio.h>
//#include <ds1621.h>
#include <system.h>

int main(void)
{
    printf("%s\n", __PRETTY_FUNCTION__);
    //i2c_enable();
    //delayN(WAIT_150_MS);
    //i2c_init(DS1621_ADDRESS);
    //i2c_disable_clkt();
    //delayN(WAIT_150_MS);
    //ds1621_start_conversion();
    //i2c_debug(__PRETTY_FUNCTION__);
    //disableAUXIRQ();

    printf("enabling gpio irqs ...\n");
    //enable_gpio_int();

    //enableAUXIRQ();
    irqEnableTimerIrq();

    printf("\nEnabling interrupts\n");
    _interrupt_enable();
    printf("IRQs enabled\n");

    printCPSRState();
    //gpio_enable_low_detect(PIN_26);
    //gpio_enable_high_detect(PIN_26);
    //gpio_enable_rising_detect(PIN_26);
    //gpio_enable_falling_detect(PIN_26);
    ArmTimeInit();
    printf("Entering sleep mode ...\n");


    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        gpio_debug();
    }
    return 0;
}
