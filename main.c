

#include "modules.h"
#include <stdio.h>
//#include <ds1621.h>
#include <system.h>

int main(void)
{

    gpio_debug();
    //gpio_set_pin_OUT(app_pin);
    //gpio_set_pin_OUT(uart_rx_pin);
    //gpio_set_pin_OUT(error_pin);
    //gpio_pin_mode(pin1, INPUT);
    //gpio_pin_mode(pin2, INPUT);

    // PIN_29 && PIN_31 are pull high, so there are always
    // HIGH.
    //gpio_enable_low_detect(pin1);
    //gpio_enable_low_detect(pin2);
    //gpio_enable_rising_detect(PIN_22);
    gpio_enable_falling_detect(PIN_22);
    //gpio_set_pin_OUT(ACK_LED);
    //i2c_enable();
    //delayN(WAIT_150_MS);
    //i2c_init(DS1621_ADDRESS);
    //i2c_disable_clkt();
    //delayN(WAIT_150_MS);
    //ds1621_start_conversion();
    //i2c_debug(__PRETTY_FUNCTION__);
    //disableAUXIRQ();
    _interrupt_enable();
    enable_gpio_int();
    //enableAUXIRQ();
    //irqEnableTimerIrq();
    //ArmTimeInit();
    printf("Start reading temperature : \n");
    //RAIO_turn_display(ON);
    //gpio_pin_mode(PIN_37, OUTPUT);
    //gpio_pin_mode(PIN_35, OUTPUT);
    //gpio_pin_mode(PIN_33, OUTPUT);
    //gpio_pin_mode(PIN_31, OUTPUT);
    //spi_init();
    //spi_test();
    TFT_SPI_protocol_debug();
    example_DrawWithRAIO();
    delayN(WAIT_1_SECOND * 60);
    example_DrawFunction(SIN);
    delayN(WAIT_1_MILLISECOND);
    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        gpio_debug();
    }
    return 0;
}
