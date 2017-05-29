

#include "modules.h"
#include <stdio.h>
#include <system.h>

extern int _write(int, char*, int);
int main(void)
{
    int app_pin = PIN_33;
    int c  = 0;
    //float f = 2.1;
    int uart_rx_pin = PIN_35;
    int error_pin = PIN_37;
    //gpio_set_pin(ACK_LED);
    gpio_set_pin_OUT(app_pin);
    gpio_set_pin_OUT(uart_rx_pin);
    gpio_set_pin_OUT(error_pin);
    gpio_set_pin_OUT(ACK_LED);
    //core_blink(ACK_LED, 2);
    //gpio_set_pin(app_pin);
    //gpio_set_pin(app_pin);
    //spi_demo();
    mu_init(BAUD_115200);
    core_blink(ACK_LED, 3);
    //printf("Raspberry Pi 2 Initialized !");
    core_blink(ACK_LED, 3);
    irqEnableTimerIrq();
    ArmTimeInit();
    _interrupt_enable();
    //printProcessorInfo();
    //core_blink(uart_rx_pin, 3);
    while(1)
    {
       //mini_uart_rx_wait();
        //c = mini_uart_data();
       // mini_uart_send(c);
    }
    return 0;
}
