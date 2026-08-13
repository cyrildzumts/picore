


#include <stdio.h>
#include <cpu.h>
#include <system.h>

extern void _interrupt_enable();

int main(void)
{ 
    int pin = PIN_29;
    printf("%s started\n", __PRETTY_FUNCTION__);
    i2c_enable();

    printf("Initializing PCA9685 PWM driver\n");
    pca9685_init();
    irqEnableTimerIrq();
    printf("\nEnabling interrupts\n");
    _interrupt_enable();
    printf("IRQs enabled\n");
    ArmTimeInit();
    printf("Entering sleep mode\n");
    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        core_blink(pin, 1);
        pca9685_demo();
        //printCPSRState();
    }
    return 0;
}
