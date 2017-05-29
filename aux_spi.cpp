#include "aux_spi.h"



/*******************************************************
 * AUX SPI INTERFACE
 * ****************************************************/

volatile static AUX_SPI_REG *aux_spi_reg= (AUX_SPI_REG*)(AUX_SPI_BASE_ADDRESS);
volatile static AUX_SPI_EN_REG *aux_en_reg = (AUX_SPI_EN_REG*)(AUX_SPI_IRQ_REG);
AUX_SPI_REG *aux_spi_get_reg()
{
    return aux_spi_reg;
}

AUX_SPI_EN_REG *aux_spi_en_reg()
{
    return aux_en_reg;
}


void aux_spi_init(int speed)
{

    aux_spi_interface_enable();
    aux_spi_get_reg()->SPI0_CONTL0_REG |= ((0xFFF & speed) << 20);
    aux_spi_clear_fifos();
    gpio_alt_func_pin(AUX_SPI1_CE2_N_PIN,GPIO_PIN_ALT4);
    gpio_alt_func_pin(AUX_SPI1_CE1_N_PIN,GPIO_PIN_ALT4);
    gpio_alt_func_pin(AUX_SPI1_CE0_N_PIN,GPIO_PIN_ALT4);
    gpio_alt_func_pin(AUX_SPI1_MISO_PIN,GPIO_PIN_ALT4);
    gpio_alt_func_pin(AUX_SPI1_MOSI_PIN,GPIO_PIN_ALT4);
    gpio_alt_func_pin(AUX_SPI1_SCLK_PIN,GPIO_PIN_ALT4);
    aux_spi_enable();
}

void aux_spi_interface_enable()
{
    aux_spi_en_reg()->SPI_EN_REG = AUX_SPI_1_EN;
}

void aux_spi_chi_select(int cs)
{
    aux_spi_get_reg()->SPI0_CONTL0_REG |= (AUX_SPI_CNTL0_CS_PATTERN  & (cs << 17));
}

void aux_spi_set_io_mode(int io_mode)
{
    aux_spi_get_reg()->SPI0_CONTL0_REG |= AUX_SPI_CNTL0_POST_IO;
}

void aux_spi_clear_fifos()
{
    aux_spi_get_reg()->SPI0_CONTL0_REG |= AUX_SPI_CNTL0_CLEAR_FIFO;
}

void aux_spi_enable()
{
    aux_spi_get_reg()->SPI0_CONTL0_REG |= AUX_SPI_CNTL0_ENABLE;
}

void aux_spi_invert_clk()
{
    aux_spi_get_reg()->SPI0_CONTL0_REG |= AUX_SPI_CNTL0_INV_CLK;
}

void aux_spi_reset_clk()
{
    aux_spi_get_reg()->SPI0_CONTL0_REG &= ~AUX_SPI_CNTL0_INV_CLK;
}

int aux_spi_tx_is_full()
{
    return aux_spi_get_reg()->SPI0_STAT_REG & AUX_SPI_STAT_TX_FULL;
}

int aux_spi_tx_is_empty()
{
    return aux_spi_get_reg()->SPI0_STAT_REG & AUX_SPI_STAT_TX_EMPTY;
}

int aux_spi_rx_is_empty()
{
    return aux_spi_get_reg()->SPI0_STAT_REG & AUX_SPI_STAT_RX_EMPTY;
}

int aux_spi_is_busy()
{
    return aux_spi_get_reg()->SPI0_STAT_REG & AUX_SPI_STAT_BUSY;
}

int aux_spi_data()
{
    return aux_spi_get_reg()->SPI0_IO_REG;
}

void aux_spi_write(char *data, int size)
{

    volatile int i = 0;
    for (i = 0; i < size; i++)
    {
        aux_spi_clear_fifos();
        aux_spi_send(data[i]);
    }
}

void aux_spi_send(char data)
{
    while(aux_spi_tx_is_full())
    {

    }
    aux_spi_get_reg()->SPI0_IO_REG = data;
}
