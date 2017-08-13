

#include "modules.h"
#include <stdio.h>
#include <ds1621.h>
#include <system.h>

extern int _write(int, char*, int);
int main(void)
{
    int app_pin = PIN_33;
    //float f = 2.1;
    int uart_rx_pin = PIN_35;
    int error_pin = PIN_37;
    int pin2 = PIN_31;
    int pin1 = PIN_29;
    gpio_set_pin_OUT(app_pin);
    gpio_set_pin_OUT(uart_rx_pin);
    gpio_set_pin_OUT(error_pin);
    gpio_set_pin_IN(pin1);
    gpio_set_pin_IN(pin2);
    gpio_enable_low_detect(pin1);
    gpio_enable_low_detect(pin2);
    core_blink(error_pin, 3);
    //gpio_set_pin_OUT(ACK_LED);
    //i2c_enable();
    delayN(WAIT_150_MS);
    //i2c_init(DS1621_ADDRESS);
    //i2c_disable_clkt();
    delayN(WAIT_150_MS);
    //ds1621_start_conversion();
    //i2c_debug(__PRETTY_FUNCTION__);
    printf("Start reading temperature : \n");
    //disableAUXIRQ();
    _interrupt_enable();
    enable_gpio_int();
    //enableAUXIRQ();
    //irqEnableTimerIrq();
    //ArmTimeInit();
    while(1)
    {
        delayN(WAIT_1_SECOND * 30);
        //ds1621_read_temp();
    }
    return 0;
}
