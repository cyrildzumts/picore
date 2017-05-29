#include "mu_uart.h"
#include "timer.h"
#include <string.h>

/************************************************
 * UART INTERFACE
 * ************************************************/


static AUX_MU_REG *mini_uart_reg = (AUX_MU_REG*)(AUX_START_ADDRESS);

AUX_MU_REG* get_aux_mu_reg()
{
    return mini_uart_reg;
}


void mu_enable()
{
    get_aux_mu_reg()->AUX_ENABLES |= AUX_MU_EN;
}

void mu_init(int baudrate)
{

    mini_uart_enable();
    //uint32_t *gpio;
    uint32_t *FSEL1 = gpio_get_base_ptr() + GPIO_GPFSEL1;
    uint32_t *PUDCLOCK0 = gpio_get_base_ptr() + GPIO_GPPUDCLK0;

    // disable irq
    //mu_set_dlab(0);
    mini_uart_reg->AUX_MU_IER_REG = 0;
    //*((uint32_t*)(AUX_M_IER_REG)) = 0;
    // disable flow control
    mini_uart_reg->AUX_MU_CNTL_REG = 0;
    //*((uint32_t*)(AUX_M_CNTL_REG)) = 0;
    // set bit mode
    //mini_uart_set_operation_mode(UART_DATA_MODE_8BIT);
    mini_uart_reg->AUX_MU_LCR_REG |= AUX_MU_LCR_DATA_SIZE_8BIT;
    //*((uint32_t*)(AUX_M_LCR_REG)) = AUX_MU_LCR_DATA_SIZE_8BIT;

    mini_uart_reg->AUX_MU_MCR_REG  |= AUX_MU_MCR_RTS;
    //*((uint32_t*)(AUX_M_MCR_REG)) = 0;
    // disable all irq and clear fifos
    mini_uart_reg->AUX_MU_IER_REG = 0;
    //*((uint32_t*)(AUX_M_IER_REG)) = 0;
    mini_uart_reg->AUX_MU_IIR_REG |= 0xC6;
    //*((uint32_t*)(AUX_M_IIR_REG)) = 0xC6;


    // set baud rate
    mini_uart_set_baudrate(baudrate);
    uint32_t content = *FSEL1;
    int alt_14 = 2 << 12; // GPIO14 - ALT5
    int alt_15 = 2 << 15; // GPIO15 - ALT5
    // set GPIO14 als INPUT
    content &= ~(7 << 12);
    // ALT FUNC 5 on GPIO14
    content |= alt_14;
    // Set GPIO15 als Input
    content &= ~(7 << 15);
    // ALT FUNC 5 on GPIO15
    content |= alt_15;
    *FSEL1 = content;
    //gpio_alt_func_pin(UART_TX_PIN,GPIO_PIN_ALT5);
    //gpio_alt_func_pin(UART_RX_PIN,GPIO_PIN_ALT5);
    gpio_pud(0);
    delayN(150);
    //pio_set_pud(UART_TX_PIN, GPIO_PUD_OFF);
    *PUDCLOCK0 |= ((1<<14) | (1<<15));
    delayN(150);
    //gpio_set_pud(UART_RX_PIN, GPIO_PUD_OFF);
    *PUDCLOCK0 = 0;
    // set UART 1 TX_RX pin
    //mini_uart_reg->AUX_MU_CNTL_REG |= AUX_MU_CNTL_TX_RX_EN;
    *((uint32_t*)(AUX_M_CNTL_REG)) |= AUX_MU_CNTL_TX_RX_EN;
    core_blink(PIN_37, 6);

}

void mu_irq_enable()
{
    mini_uart_reg->AUX_MU_IIR_REG |= AUX_MU_IRQ_RX_EN | AUX_MU_IRQ_TX_EN;
}

void mu_set_dlab(int value)
{
    if(value)
    {
        mini_uart_reg->AUX_MU_LCR_REG |= AUX_MU_LCR_DLAB_ACCESS;
    }
    else
    {
        mini_uart_reg->AUX_MU_LCR_REG &= ~AUX_MU_LCR_DLAB_ACCESS;
    }
}

void uart_disable()
{
    mini_uart_receiver_disable();
    mini_uart_transmiter_disable();
    mini_uart_reg->AUX_ENABLES &= ~UART_ENABLE;
}

void uart_close()
{
    uart_disable();
    gpio_set_pin_IN(UART_RX_PIN);
    gpio_set_pin_IN(UART_TX_PIN);
}

void mini_uart_enable()
{

    mini_uart_reg->AUX_ENABLES |= AUX_MU_EN;
    //*((uint32_t*)(AUX_M_ENABLES)) |= AUX_MU_EN;

}
void mini_uart_stream(char *str, int len)
{
    int i = 0;
    if(str)
    {
        while(i < len)
        {
            mini_uart_send(str[i]);
            i++;
        }
    }
}

void mini_uart_send(uint32_t data)
{
    mini_uart_tx_wait();
    mini_uart_reg->AUX_MU_IO_REG = (uint8_t)data;
}

int mini_uart_is_busy()
{
    return (mini_uart_reg->AUX_MU_STAT_REG & AUX_MU_BUSY);
}



void mini_uart_clear_fifo()
{
    mini_uart_reg->AUX_MU_IER_REG |= AUX_MU_IER_FIFO_CLEAR;
}

void mini_uart_clear_rx_fifo()
{
    mini_uart_reg->AUX_MU_IER_REG |= AUX_MU_FIFO_RX_CLEAR;
}

void mini_uart_clear_tx_fifo()
{
    mini_uart_reg->AUX_MU_IER_REG |= AUX_MU_FIFO_TX_CLEAR;
}

uint8_t mini_uart_data()
{
    return mini_uart_reg->AUX_MU_IO_REG;
}

int mini_uart_has_pending_int()
{
    return 0;
}

void mini_uart_set_operation_mode(int8_t mode)
{
    mini_uart_reg->AUX_MU_LCR_REG |= mode;
}

void mini_uart_set_baudrate(int16_t rate)
{
    //mini_uart_reg->AUX_MU_BAUD_REG = (CLK_FREQ/(8*rate))-1;
    mini_uart_reg->AUX_MU_BAUD_REG = 270;
    //*((uint32_t*)(AUX_M_BAUD_REG)) = rate;
}

int16_t mini_uart_get_baudrate()
{
    return mini_uart_reg->AUX_MU_BAUD_REG;
}


int mini_uart_has_data()
{
    return (get_aux_mu_reg()->AUX_MU_LSR_REG & AUX_MU_LSR_DATA_READY) ;
}

int mini_uart_can_sent()
{
    return (get_aux_mu_reg()->AUX_MU_LSR_REG & AUX_MU_LSR_TX_IDLE);
}

void mini_uart_transmiter_disable()
{
    mini_uart_reg->AUX_MU_CNTL_REG &=~ AUX_MU_CNTL_TX_EN;
}

void mini_uart_transmiter_enable()
{
    mini_uart_reg->AUX_MU_CNTL_REG |= AUX_MU_CNTL_TX_EN;
}
int mini_uart_transmiter_done()
{
    return (mini_uart_reg->AUX_MU_STAT_REG & AUX_MU_STAT_TX_DONE);
}

void mini_uart_receiver_enable()
{
    mini_uart_reg->AUX_MU_CNTL_REG |= AUX_MU_CNTL_RX_EN;
}

void mini_uart_receiver_disable()
{
    mini_uart_reg->AUX_MU_CNTL_REG &= ~AUX_MU_CNTL_RX_EN;
}



void mini_uart_tx_wait()
{
    while((mini_uart_reg->AUX_MU_LSR_REG & AUX_MU_LSR_TX_IDLE) != AUX_MU_LSR_TX_IDLE )
    {
        ;
        //core_blink(PIN_37, 1);
    }
}

void mini_uart_rx_wait()
{
    while((mini_uart_reg->AUX_MU_LSR_REG & AUX_MU_LSR_DATA_READY) != AUX_MU_LSR_DATA_READY )
    {
        ;
        //core_blink(PIN_37, 1);
    }
}
