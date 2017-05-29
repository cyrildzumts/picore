#include "spi.h"
#include "aux_spi.h"
volatile int *aux_en_ptr = (int*)AUX_SPI_ENABLES_REG;

static SPI_REG *spi_reg = (SPI_REG*)(SPI_BASE_ADDRESS);

SPI_REG* spi_get_reg()
{
    return spi_reg;
}

void spi_init()
{
    gpio_alt_func_pin(PIN_19,GPIO_PIN_ALT0);
    gpio_alt_func_pin(PIN_21,GPIO_PIN_ALT0);
    gpio_alt_func_pin(PIN_23,GPIO_PIN_ALT0);
    gpio_alt_func_pin(PIN_26,GPIO_PIN_ALT0);
    gpio_set_pin(SPI_DBG_INTERFACE_EN_PIN);
    spi_get_reg()->CS = 0;
    spi_clear_fifos();
    spi_set_datamode(MODE0);
    spi_set_clk_divider(CLOCK_DIVIDER);
    spi_chip_select(CS1);
    spi_set_cs_pol(CS1,LOW);

}

void spi_close()
{
    gpio_set_pin_IN(PIN_19);
    gpio_set_pin_IN(PIN_21);
    gpio_set_pin_IN(PIN_23);
    gpio_set_pin_IN(PIN_24);
    gpio_set_pin_IN(PIN_26);
}

void spi_interface_enable()
{
    *aux_en_ptr |= SPI1_EN;
    //*spi_cntl_reg_ptr |= SPI_EN;
}

void spi_set_clk_divider(uint16_t divider)
{
    spi_get_reg()->CLK = divider;
}

void spi_set_datamode(uint8_t mode)
{
    //int tmp = spi_get_reg()->CS;
    //tmp = tmp |  (SPI_CS_CSPOL | SPI_CS_CPHA);
    //spi_get_reg()->CS = tmp;
    core_set_bits(&spi_get_reg()->CS, mode << 2, SPI_CS_CPOL | SPI_CS_CPHA);
}

// write and read  byte to SPI
uint8_t spi_send(uint8_t data)
{
    int ret;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();

    // wait for TXD
    while(!(spi_get_reg()->CS & SPI_CS_TXD))
    {
        ;
    }
    // Write to FIFO
    spi_get_reg()->FIFO = data;
    // wait for DONE
    while(!(spi_get_reg()->CS & SPI_CS_DONE))
    {
        ;
    }
    // Read  any Byte from SPI
    ret = spi_get_reg()->FIFO;
    // set TA = 0
    spi_clear_ta();

    return ret;
}




// writes (and reads) len bytes to SPI
void spi_write(uint8_t *data, int len)
{
    int i;
    int rxd = 0;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();
    for(i = 0; i < len; i++)
    {
        // wait for TXD
        while(!(spi_get_reg()->CS & SPI_CS_TXD))
        {
            ;
        }
        // Write to FIFO
        spi_get_reg()->FIFO = data[i];
        while(((spi_get_reg()->CS & SPI_CS_RXD)))
        {
            //core_blink(PIN_37, 1);
            //spi_get_reg()->CS |= SPI_CS_RX_CLEAR;
            rxd = spi_get_reg()->FIFO;
        }
    }

    // wait for DONE
    while(!(spi_get_reg()->CS & SPI_CS_DONE))
    {
        while(((spi_get_reg()->CS & SPI_CS_RXD)))
        {
            //core_blink(PIN_37, 2);
            //spi_get_reg()->CS |= SPI_CS_RX_CLEAR;
            (void)spi_get_reg()->FIFO;
        }
    }
    // set TA = 0
    spi_clear_ta();
}

void spi_chip_select(uint8_t cs)
{
    spi_get_reg()->CS |= cs;
}
void spi_set_cs_pol(uint8_t cs, uint8_t active)
{
    uint8_t shift = 21 + cs;
    int CS = spi_get_reg()->CS;
    int tmp = 0;
    if(active)
    {
        tmp = CS | (1 << shift);
    }
    else
    {
        tmp = CS &( ~(1 << shift));
    }
    spi_get_reg()->CS = tmp;

}

int spi_read()
{
    return spi_get_reg()->FIFO;
}

void spi_clear_fifos()
{

    spi_get_reg()->CS |= SPI_CS_CLEAR;
}

void spi_set_ta()
{
    spi_get_reg()->CS |= SPI_CS_TA;
}

void spi_clear_ta()
{
    spi_get_reg()->CS &= ~SPI_CS_TA;
}

int spi_is_transfert_active()
{
    return spi_get_reg()->CS & SPI_CS_TA;
}

int spi_is_interface_en()
{
    return (*aux_en_ptr & SPI1_EN);
}

int spi_data_received()
{
    return spi_get_reg()->CS & SPI_CS_RXR;
}

int spi_data_sent()
{
    return spi_get_reg()->CS & SPI_CS_DONE ;
}

void spi_clear_rx_fifo()
{
    spi_get_reg()->CS |= SPI_CS_RX_CLEAR;
}

void spi_clear_tx_fifo()
{
    spi_get_reg()->CS |= SPI_CS_TX_CLEAR;
}

void spi_quick_send(char data)
{
    // Write to FIFO
    gpio_clear_pin(PIN_18);

    // activate SPI transfer
    spi_get_reg()->CS |= SPI_CS_TA;
    spi_get_reg()->FIFO = data;
    //spi_get_reg()->CS &= ~SPI_CS_TA;
    spi_get_reg()->CS |= SPI_CS_RX_CLEAR;

    // WR = 1
    gpio_set_pin( PIN_18 );

}
