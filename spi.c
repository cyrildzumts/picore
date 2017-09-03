#include "spi.h"
#include "aux_spi.h"
volatile int *aux_en_ptr = (int*)AUX_SPI_ENABLES_REG;

volatile static SPI_REG *spi_reg = (SPI_REG*)(SPI_BASE_ADDRESS);

SPI_REG* spi_get_reg()
{
    return spi_reg;
}

void spi_init()
{
    //printf("Entering %s\n", __PRETTY_FUNCTION__);

    //printf("spi: Interface enabled.\n");

    gpio_alt_func_pin(SPI_MOSI_PIN,GPIO_PIN_ALT0);
    gpio_alt_func_pin(SPI_MISO_PIN,GPIO_PIN_ALT0);
    gpio_alt_func_pin(SPI_CLK_PIN,GPIO_PIN_ALT0);
    gpio_alt_func_pin(SPI_CE0_N_PIN,GPIO_PIN_ALT0);
    gpio_alt_func_pin(SPI_CE1_N_PIN,GPIO_PIN_ALT0);

    spi_interface_enable();
    //gpio_set_pud(SPI_MISO_PIN, GPIO_PUD_DOWN);

    /*
    gpio_pin_mode(SPI_MOSI_PIN, OUTPUT);
    gpio_pin_mode(SPI_CE1_N_PIN, OUTPUT);
    gpio_pin_mode(SPI_CE0_N_PIN, OUTPUT);
    */

    //printf("spi: pins alternate function 0 activated.\n");
    spi_reg->CS = 0;
    //printf("spi: clearing  fifos ...\n");
    spi_reg->CS |= SPI_CS_CLEAR;
    //spi_clear_fifos();
    //printf("spi: setting data mode to %d ...\n", MODE0);
    spi_set_datamode(MODE0);
    //printf("spi: setting clock divider to %d  ...\n", CLOCK_DIVIDER);
    spi_set_clk_divider(CLOCK_DIVIDER_8);
    spi_chip_select(CS1);
    //printf("spi: setting chip select to %d ...\n", CS1);
    spi_set_cs_pol(CS1,LOW);
    //spi_irq_enable(SPI_CS_INTR);
    //printf("spi: IRQ INTR enabled ...\n");
    //printf("spi: spi module is now ready ...\n");
    //printf("Leaving %s\n", __PRETTY_FUNCTION__);

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
    spi_reg->CLK = divider;
}

void spi_set_datamode(uint8_t mode)
{
    //int tmp = spi_get_reg()->CS;
    //tmp = tmp |  (SPI_CS_CSPOL | SPI_CS_CPHA);
    //spi_get_reg()->CS = tmp;
    core_set_bits(&spi_reg->CS, mode << 2, SPI_CS_CPOL | SPI_CS_CPHA);
}

// write and read  one byte to SPI
uint32_t spi_send(uint8_t data)
{
    int rxd = -1;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();

    // wait for TXD
    while(!spi_cs_txd())
    {
        ;
    }
    // Write to FIFO
    spi_reg->FIFO = data;

    //spi_debug();
    while(!spi_cs_done())
    {
        ;
    }
    while(!spi_cs_rxd())
    {
        ;
    }
    while(spi_cs_rxd())
    {
        rxd = spi_reg->FIFO;
    }
    //printf("%s data2 received : %X\n",__PRETTY_FUNCTION__, rxd);

    // set TA = 0
    spi_clear_ta();

    return rxd;
}

// writes (and reads) len bytes to SPI
void spi_write(uint8_t *data, int len)
{
    volatile int i;
    volatile int rxd = -1;
    // Clear TX and RX FIFOs
    spi_clear_fifos();
    // Set TA = 1;
    spi_set_ta();
    for(i = 0; i < len; i++)
    {
        // wait for TXD
        while(!spi_cs_txd())
        {
            ;
        }
        // Write to FIFO
        spi_reg->FIFO = data[i];
        //printf("%s data sent \n",__PRETTY_FUNCTION__);
        //delayN(10);
        //spi_debug();
        while(spi_cs_rxd())
        {
            rxd = spi_reg->FIFO;
            //printf("%s data received : %X\n",__PRETTY_FUNCTION__, rxd);
        }
    }

    // wait for DONE

    while(!spi_cs_done())
    {
        while(spi_cs_rxd())
        {

            rxd = spi_reg->FIFO;
            //printf("%s data2 received : %X\n",__PRETTY_FUNCTION__, rxd);
            //spi_get_reg()->FIFO;
        }
    }
    rxd = 0;
    (void)rxd;
    // set TA = 0
    spi_clear_ta();
}

void spi_chip_select(uint8_t cs)
{
    spi_reg->CS |= cs;
}
void spi_set_cs_pol(uint8_t cs, uint8_t active)
{
    uint8_t shift = 21 + cs;
    /*
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
    */
    core_set_bits(&spi_reg->CS,active << shift,1 <<shift);

}

int spi_read()
{
    volatile int data = -1;

    if(spi_cs_rxd())
    {
        data = spi_reg->FIFO;
    }


    return data;
}

void spi_clear_fifos()
{

    spi_reg->CS |= SPI_CS_CLEAR;
}

void spi_set_ta()
{
    spi_reg->CS |= SPI_CS_TA;
}

void spi_clear_ta()
{
    spi_reg->CS &= ~SPI_CS_TA;
}

int spi_is_transfert_active()
{
    return spi_reg->CS & SPI_CS_TA;
}

int spi_is_interface_en()
{
    return (*aux_en_ptr & SPI1_EN);
}

int spi_data_received()
{
    return spi_reg->CS & SPI_CS_RXR;
}

int spi_data_sent()
{
    return spi_reg->CS & SPI_CS_DONE ;
}

void spi_clear_rx_fifo()
{
    spi_reg->CS |= SPI_CS_RX_CLEAR;
}

void spi_clear_tx_fifo()
{
    spi_reg->CS |= SPI_CS_TX_CLEAR;
}

void spi_quick_send(uint32_t *data, int len)
{
    // Write to FIFO
    int i = 0;
    int rxd = -1;
    spi_clear_fifos();
    spi_set_ta();
    printf("%s started\n", __PRETTY_FUNCTION__);
    for(i = 0; i < len; i++)
    {
        if(i >= SPI_BUFFER_SIZE)
        {
            break;
        }
        spi_reg->FIFO = data[i];
    }

    // activate SPI transfer

    while (!spi_cs_done()) {
        printf("...\n");
    }
    printf("finished buffering data into tx buffer\n");
    while(spi_cs_rxd())
    {
        rxd = spi_reg->FIFO;
        printf(" %d | %X \n", rxd, rxd);
    }
    spi_clear_ta();
    printf("%s terminated\n", __PRETTY_FUNCTION__);
}

void spi_irq_enable(uint32_t mask)
{
    //if(aux_spi_get_reg()->)
    spi_reg->CS |= mask;
}

void spi_debug()
{
    printf("---------------------\n"
           "SPI Modules Debug : \n");
    printf("CS          : %X\n"
           "CPHA        : %X\n"
           "CPOL        : %X\n"
           "CSPOL       : %X\n"
           "TA          : %X\n"
           "DONE        : %X\n"
           "LEN         : %X\n"
           "RXD         : %X\n"
           "TXD         : %X\n"
           "RXR         : %X\n"
           "RXF         : %X\n"
           "AUX-EN      : %X\n"
           "DIVIDER     : %d\n",
           spi_cs_cs(), spi_cs_cpha(),spi_cs_cpol(),
           spi_cs_cspol(),spi_cs_ta(),spi_cs_done(),
           spi_cs_len(),
           spi_cs_rxd(), spi_cs_txd(), spi_cs_rxr(),
           spi_cs_rxf(), spi_aux_enable(), spi_reg->CLK);
    printf("---------------------\n");
}

uint32_t spi_cs_len_long()
{
    return ((spi_reg->CS & SPI_CS_EN_LEN_LONG) >> 25);
}

void spi_set_len_long(int bit_mode)
{
    spi_reg->CS |= bit_mode;
}

uint32_t spi_cs_dma_len()
{
    return ((spi_reg->CS & SPI_CS_DMA_LEN) >> 24);
}

uint32_t spi_cs_cspol2()
{
    return ((spi_reg->CS & SPI_CS_CSPOL2) >> 23);
}

uint32_t spi_cs_cspol1()
{
    return ((spi_reg->CS & SPI_CS_CSPOL1) >> 22);
}

uint32_t spi_cs_cspol0()
{
    return ((spi_reg->CS & SPI_CS_CSPOL0) >> 21);
}

uint32_t spi_cs_rxf()
{
    return ((spi_reg->CS & SPI_CS_RXF) >> 20);
}

uint32_t spi_cs_rxr()
{
    return ((spi_reg->CS & SPI_CS_RXR) >> 19);
}

uint32_t spi_cs_txd()
{
    return ((spi_reg->CS & SPI_CS_TXD) >> 18);
}

uint32_t spi_cs_rxd()
{
    return ((spi_reg->CS & SPI_CS_RXD) >> 17);
}

uint32_t spi_cs_done()
{
    return ((spi_reg->CS & SPI_CS_DONE) >> 16);
}

uint32_t spi_cs_len()
{
    return ((spi_reg->CS & SPI_CS_LEN) >> 13);
}

uint32_t spi_cs_ren()
{
    return ((spi_reg->CS & SPI_CS_REN) >> 12);
}

uint32_t spi_cs_adcs()
{
    return ((spi_reg->CS & SPI_CS_ADCS) >> 11);
}

uint32_t spi_cs_intr()
{
    return ((spi_reg->CS & SPI_CS_INTR) >> 10);
}

uint32_t spi_cs_intd()
{
    return ((spi_reg->CS & SPI_CS_INTD) >> 9);
}

uint32_t spi_cs_dmaen()
{
    return ((spi_reg->CS & SPI_CS_DMAEN) >> 8);
}

uint32_t spi_cs_ta()
{
    return ((spi_reg->CS & SPI_CS_TA) >> 7);
}

uint32_t spi_cs_cspol()
{
    return ((spi_reg->CS & SPI_CS_CSPOL) >> 6);
}

void spi_cs_clear(uint32_t mask)
{
    spi_reg->CS |= mask;
}

uint32_t spi_cs_cpol()
{
    return ((spi_reg->CS & SPI_CS_CPOL) >> 3);
}

uint32_t spi_cs_cpha()
{
    return ((spi_reg->CS & SPI_CS_CPHA) >> 2);
}

uint32_t spi_cs_cs()
{
    return ((spi_reg->CS & SPI_CS_CS) >> 0);
}

uint32_t spi_aux_enable()
{
    return *aux_en_ptr;
}

void spi_write_cmd(int reg)
{

    // activate SPI transfer
    spi_set_ta();
    // wait for TXD
    while(!spi_cs_done())
    {
        ;
    }
    // Write to FIFO
    spi_reg->FIFO = reg & 0xFF;
    delayN(30);
    printf("Data sent : %X\n", reg);
    spi_debug();
    while (!spi_cs_done())
    {
        ;
    }
    spi_clear_ta();
}

void spi_read_cmd()
{
    printf("entering %s\n",__PRETTY_FUNCTION__);
    volatile int i= 0;
    int data = -1;
    // wait for TXD
    spi_debug();
    printf("waiting for tx to be done \n");
    // Write to FIFO
    //spi_get_reg()->FIFO = reg & 0xFF;
    //delayN(30);
    //printf("Data sent : %X\n", reg);
    spi_debug();
    while (!spi_cs_done() && spi_cs_ta())
    {
        if(i > 100)
        {
            printf("No data received : i = %d\n",i);
            return;
        }
        i++;
    }
    printf("waiting for data to come ...\n");
    while(!spi_cs_rxd())
    {
        printf("...\n");
    }
    printf("new data received : ");
    data = spi_reg->FIFO;
    printf("%X\n",data);
}

void spi_test()
{
    uint32_t tx_buffer[SPI_BUFFER_SIZE]={0};
    uint32_t rx_buffer[SPI_BUFFER_SIZE]={-1};
    volatile uint32_t level = -1, level2 = -1;
    volatile int i = 0;
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        tx_buffer[i] = i + 240;
    }
    printf("TX Buffer content : \n");
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        printf(" %d : %d    | %X \n", i, tx_buffer[i], tx_buffer[i]);
    }
    deassert(SPI_CE1_N_PIN);

    /*
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        rx_buffer[i] = spi_send(tx_buffer[i]);
    }

    printf("SPI Send Test finished :  here are the data received :\n");
    for(i = 0; i < SPI_BUFFER_SIZE; i++)
    {
        printf(" %d : %d    | %X \n", i, rx_buffer[i], rx_buffer[i]);
    }
    spi_quick_send(tx_buffer, SPI_BUFFER_SIZE);
    */
    for(i = 0; i < 256; i++)
    {
        delayN(WAIT_1_MILLISECOND*100);
        spi_send(i);
    }
    assert(SPI_CE1_N_PIN);
    printf("Leaving %s\n", __PRETTY_FUNCTION__);

}
