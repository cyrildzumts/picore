
#include "raio8870.h"
#include "tft.h"


// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board ( void )
{
    int rst = -1;
    printf("Entering %s\n", __PRETTY_FUNCTION__);
    // *************** set the pins to be an output and turn them on

    int pin_mask = ((1 << RAIO_OE_PIN)| (1 << RAIO_RST_PIN) | (1 << RAIO_CS_PIN)
                    | (1 << RAIO_WR_PIN));
    gpio_pin_mode(RAIO_OE_PIN,OUTPUT);
    //gpio_set_pin_OUT(RAIO_OE_PIN);

    gpio_pin_mode(RAIO_RST_PIN, OUTPUT);
    rst = (gpio_get_pin_level_register().low & (1 << RAIO_RST_PIN)) >> RAIO_RST_PIN;
    printf("RAIO_RST_PIN OUTPUT LEVEL : %d\n", rst);
    //gpio_set_pin_OUT(RAIO_RST_PIN);

    //gpio_set_pin_OUT(RAIO_CS_PIN);
    gpio_pin_mode(RAIO_CS_PIN, OUTPUT);


    gpio_pin_mode(RAIO_RS_PIN,OUTPUT);
    //gpio_set_pin_OUT(RAIO_RS_PIN);


    gpio_pin_mode(RAIO_WR_PIN, OUTPUT);
    //gpio_set_pin_OUT(RAIO_WR_PIN);


    gpio_pin_mode(RAIO_RD_PIN, OUTPUT);
    //gpio_set_pin_OUT(RAIO_RD_PIN);
    // first remove the current pud on the pins defined
    // in mask.
    gpio_set_pudclock(GPIO_GPPUDCLK0, pin_mask, GPIO_PUD_OFF);
    gpio_set_pudclock(GPIO_GPPUDCLK0, pin_mask, GPIO_PUD_UP);

//    assert(RAIO_OE_PIN);
//    assert(RAIO_RST_PIN);
//    assert(RAIO_CS_PIN);
//    assert(RAIO_RS_PIN);
//    assert(RAIO_WR_PIN);
//    assert(RAIO_RD_PIN);

    assert2(GPIO_GPSET0, RAIO_OE_PIN);
    assert2(GPIO_GPSET0, RAIO_RST_PIN);
    assert2(GPIO_GPSET0, RAIO_CS_PIN);
    assert2(GPIO_GPSET0, RAIO_RS_PIN);
    assert2(GPIO_GPSET0, RAIO_WR_PIN);
    assert2(GPIO_GPSET0, RAIO_RD_PIN);

        // INPUTS
    gpio_pin_mode(RAIO_WAIT_PIN, INPUT);
    //gpio_set_pin_OUT(RAIO_WAIT_PIN);
    gpio_set_pud(RAIO_WAIT_PIN,GPIO_PUD_UP);
    gpio_pin_mode(RAIO_INT_PIN, INPUT);
    //gpio_set_pin_OUT(RAIO_INT_PIN);
    //gpio_set_pudclock(GPIO_GPPUDCLK0,((1<< RAIO_WAIT_PIN)| (1 << RAIO_INT_PIN)),GPIO_PUD_UP);
    gpio_set_pud(RAIO_INT_PIN,GPIO_PUD_UP);


    // SPI INIT
    //printf("ready  to initialize spi interface\n");
    spi_init();
    printf("Leaving %s\n", __PRETTY_FUNCTION__);
}


// hard reset of the graphic controller and the tft
// ----------------------------------------------------------
void TFT_hard_reset( void )
{
    //deassert(RAIO_RST_PIN);
    deassert2(GPIO_GPCLR0,RAIO_RST_PIN);
    delayN(WAIT_1_MILLISECOND * 10);
    //assert(RAIO_RST_PIN);
    assert2(GPIO_GPSET0,RAIO_RST_PIN);
    delayN(WAIT_1_MILLISECOND);
    printf("Leaving %s\n", __PRETTY_FUNCTION__);
}


// wait during raio is busy
// ----------------------------------------------------------
void TFT_wait_for_raio ( void )
{
    while ( !gpio_read_pin_level(RAIO_WAIT_PIN) );
}


// write data via SPI to tft
// ----------------------------------------------------------
void TFT_SPI_data_out ( int data )
{
    union my_union number;
    uint8_t buffer[2];

    number.value = data;
    buffer[0] = number.split.high;
    buffer[1] = number.split.low;
    spi_write(buffer, 2 );
}


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( int reg )
{
    spi_device_debug_t *dev = spi_get_debug_dev();
    spi_session_t *current_session = dev->sessions + dev->session_count;
    //current_session = dev()->sessions + dev->session_count;
    current_session->pins_states[0] = gpio_get_pin_level_register().low;
    current_session->cmd_sent = reg;
    current_session->data_sent = 0x100;
    current_session->rx_count = 0;

    //assert( RAIO_RS_PIN);
    assert2(GPIO_GPSET0,RAIO_RS_PIN);
    //assert(PIN_37);
    //deassert(RAIO_CS_PIN);
    deassert2(GPIO_GPCLR0,RAIO_CS_PIN);
    //deassert(PIN_35);

    //deassert( RAIO_WR_PIN);
    deassert2(GPIO_GPCLR0,RAIO_WR_PIN);
    //deassert(PIN_33);
    //assert(RAIO_RD_PIN);
    //deassert( RAIO_OE_PIN);
    deassert2(GPIO_GPCLR0,RAIO_OE_PIN);
    //deassert(PIN_31);

    //deassert_mask(GPIO_GPCLR0,CS_WR_OE_MASK);
    current_session->pins_states[1] = gpio_get_pin_level_register().low;

    TFT_SPI_data_out ( reg );

    assert( RAIO_WR_PIN );
    assert2(GPIO_GPSET0,RAIO_WR_PIN);
    //assert(PIN_33);
    assert(RAIO_CS_PIN);
    assert2(GPIO_GPSET0,RAIO_CS_PIN);
    //assert(PIN_35);
    //deassert( RAIO_RS_PIN);
    //deassert(PIN_37);
    //assert(RAIO_OE_PIN);
    assert2(GPIO_GPSET0,RAIO_OE_PIN);
    //assert(PIN_31);
    current_session->pins_states[2] = gpio_get_pin_level_register().low;
    dev->session_count++;
    //assert_mask(GPIO_GPSET0, CS_WR_OE_MASK);
}


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( int data )
{ 
    spi_device_debug_t *dev = spi_get_debug_dev();
    spi_session_t *current_session = dev->sessions + dev->session_count;
    current_session->pins_states[0] = gpio_get_pin_level_register().low;
    current_session->data_sent = data;
    current_session->cmd_sent = 0x100;
    current_session->rx_count = 0;

    //deassert( RAIO_RS_PIN);
    deassert2(GPIO_GPCLR0,RAIO_RS_PIN);
    //deassert(PIN_37);
    delay();
    //deassert(RAIO_CS_PIN);
    deassert2(GPIO_GPCLR0,RAIO_CS_PIN);
    //deassert(PIN_35);
    delay();
    //deassert( RAIO_WR_PIN);
    deassert2(GPIO_GPCLR0,RAIO_WR_PIN);
    //deassert(PIN_33);

    //assert(RAIO_RD_PIN);
    //deassert( RAIO_OE_PIN);
    deassert2(GPIO_GPCLR0,RAIO_OE_PIN);
    //deassert(PIN_31);
    current_session->pins_states[1] = gpio_get_pin_level_register().low;

    TFT_SPI_data_out (data);
    //assert(RAIO_WR_PIN);
    assert2(GPIO_GPSET0,RAIO_WR_PIN);
    //assert(PIN_33);
    //assert(RAIO_CS_PIN);
    assert2(GPIO_GPSET0,RAIO_CS_PIN);
    //assert(PIN_35);
    //assert( RAIO_RS_PIN);
    //assert(PIN_37);
    //assert(RAIO_OE_PIN);
    assert2(GPIO_GPSET0,RAIO_OE_PIN);
    //assert(PIN_31);
    current_session->pins_states[2] = gpio_get_pin_level_register().low;
    dev->session_count++;
}


// write 'count'-bytes to tft
// ----------------------------------------------------------
void TFT_DataMultiWrite( uint16_t *data, uint32_t count )
{
    uint32_t i;

    deassert(RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);
    deassert(RAIO_OE_PIN);

    //deassert_mask(GPIO_GPCLR0, RS_CS_OE_MASK);
    for(i = 0; i < count ; i++)
    {
        deassert(RAIO_WR_PIN);
        spi_set_ta();
        spi_get_reg()->FIFO = (uint8_t)(data[i] >> 8);
        spi_get_reg()->FIFO = (uint8_t)(data[i] & 0xFF);
        while(!(spi_get_reg()->CS & SPI_CS_DONE))
        {
            spi_clear_rx_fifo();
        }
        spi_clear_ta();
        assert(RAIO_WR_PIN);
    }

    assert(RAIO_CS_PIN);
    assert(RAIO_OE_PIN);

    //assert_mask(GPIO_GPSET0, CS_OE_MASK);

}

//void TFT_print( char *data, uint32_t count )
//{
//    uint32_t i;
//    deassert(RAIO_RS_PIN);
//    deassert(RAIO_CS_PIN);
//    deassert(RAIO_OE_PIN);

//    for(i = 0; i < count ; i++)
//    {
//        deassert(RAIO_WR_PIN);
//        spi_send(data[i]);
//        assert(RAIO_WR_PIN);
//    }


//    assert(RAIO_CS_PIN);
//    assert(RAIO_OE_PIN);
//}

//void TFT_MemoryWrite()
//{
//    assert(RAIO_RS_PIN);
//    deassert(RAIO_CS_PIN);

//    assert(RAIO_RD_PIN);
//    delayN(30);
//    deassert(RAIO_WR_PIN);
//    spi_send(MRWC);
//    assert(RAIO_WR_PIN);
//    assert(RAIO_CS_PIN);
//    deassert(RAIO_RS_PIN);
//    delayN(10);
//    deassert(RAIO_CS_PIN);
//}
//void TFT_MemoryWriteEnd()
//{
//    assert(RAIO_RS_PIN);
//    assert(RAIO_RD_PIN);
//    assert(RAIO_WR_PIN);
//    assert(RAIO_CS_PIN);
//}

////void TFT_write_clear()
////{

////    gpio_set_pin( RAIO_RS);
////    gpio_clear_pin( RAIO_CS);
////    gpio_clear_pin( RAIO_WR);
////    gpio_clear_pin( OE);

////}

////void TFT_write_close()
////{
////    gpio_set_pin( RAIO_WR );
////    gpio_set_pin( RAIO_CS);
////    gpio_set_pin( OE);
////}

//void TFT_StartWrite()
//{
////    gpio_clear_pin( RAIO_RS);
////    gpio_clear_pin(RAIO_CS);
////    gpio_clear_pin( RAIO_WR);
////    gpio_clear_pin( OE);

//}

//void TFT_Stopwrite()
//{
////    gpio_set_pin( RAIO_WR );
////    gpio_set_pin(RAIO_CS);
////    gpio_set_pin( OE);
//}

void TFT_DataRead(int reg)
{
//    int data = 0;
//    int i = 0;
//    int rxd_count = 0;
//    uint32_t buffer[SPI_BUFFER_SIZE] = {-1};
//    /*
//    union my_union number;
//    uint8_t buffer[2];

//    number.value = reg;
//    buffer[0] = number.split.high;
//    buffer[1] = number.split.low;
//    */
//    printf("Reading Register : %X\n", reg);
//    assert( RAIO_RS_PIN);
//    deassert(RAIO_CS_PIN);
//    deassert( RAIO_WR_PIN);
//    assert(RAIO_RD_PIN);
//    deassert( RAIO_OE_PIN);
//    spi_clear_fifos();
//    spi_set_ta();
//    spi_get_reg()->FIFO = reg;
//    spi_get_reg()->FIFO = 0;
//    while (!spi_cs_done()) {
//        //printf("...\n");
//        ;
//    }
//    assert( RAIO_WR_PIN );
//    assert(RAIO_CS_PIN);
//    deassert(RAIO_RS_PIN);


//    deassert(RAIO_CS_PIN);
//    assert(RAIO_WR_PIN);
//    deassert(RAIO_RD_PIN);
//    while(spi_cs_rxd())
//    {

//        if(SPI_BUFFER_SIZE <= i)
//            break;
//        buffer[i] = spi_get_reg()->FIFO;
//        rxd_count++;
//        i++;
//    }
//    assert(RAIO_RS_PIN);
//    assert(RAIO_CS_PIN);
//    assert(RAIO_RD_PIN);
//    assert(RAIO_OE_PIN);

//    for (i = 0; i < rxd_count; i++)
//    {
//        printf(" %d | %X \n", i, buffer[i]);
//    }
//    //return buffer[rxd_count - 1];
//    printf("leaving %s\n", __PRETTY_FUNCTION__);
}

//uint32_t TFT_status_read_cmd()
//{
//    volatile uint32_t status = -1;
//    volatile int i = 0;
//    // status read cmd
//    spi_set_ta();
//    assert_mask(GPIO_GPSET0, (RAIO_RS_PIN | RAIO_WR_PIN));

//    deassert_mask(GPIO_GPCLR0, RAIO_CS_PIN | RAIO_RD_PIN);
//    //spi_debug();
//    //printf("%s loop starts: \n", __PRETTY_FUNCTION__);
//    //delayN(50);
//    spi_read_cmd();

//    /*
//    while(spi_cs_rxd())
//    {
//        spi_debug();
//        if(i > 10)
//        {
//            status = spi_read();
//            printf("%s data read into status %d : \n", __PRETTY_FUNCTION__, status);
//            //break;
//        }
//        i++;
//    }*/
//    printf("%s loop ends: \n", __PRETTY_FUNCTION__);
//    //deassert(RAIO_RD_PIN);
//    status = spi_read();
//    deassert(RAIO_RS_PIN);
//    assert_mask(GPIO_GPSET0, (RAIO_CS_PIN | RAIO_RD_PIN));
//    spi_clear_ta();
//    return status;
//}

//void TFT_write_cmd(int reg)
//{
//    assert_mask(GPIO_GPSET0 ,RAIO_RS_PIN | RAIO_RD_PIN);
//    deassert_mask( GPIO_GPCLR0, RAIO_WR_PIN | RAIO_CS_PIN);
//    //delayN(10);
//    TFT_SPI_data_out(reg);
//    assert_mask(GPIO_GPSET0,RAIO_WR_PIN | RAIO_CS_PIN);
//    deassert(RAIO_RS_PIN);
//}

//void TFT_read_cmd(int reg)
//{
//    uint32_t rxd = -1;
//    uint32_t rxd2 = -1;
//    uint32_t rxd3 = -1;
//    // Clear TX and RX FIFOs
//    spi_clear_fifos();
//    // Set TA = 1;
//    spi_set_ta();
//    assert_mask(GPIO_GPSET0 ,RAIO_RS_PIN | RAIO_RD_PIN);
//    deassert_mask( GPIO_GPCLR0, RAIO_WR_PIN | RAIO_CS_PIN);
//    deassert( RAIO_OE_PIN);
//    //delayN(10);
//    rxd = spi_send(reg);

//    //TFT_SPI_data_out(reg);
//    deassert(RAIO_RS_PIN);
//    assert_mask(GPIO_GPSET0,RAIO_WR_PIN | RAIO_CS_PIN);
//    deassert(RAIO_CS_PIN);
//    deassert(RAIO_RD_PIN);
//    rxd2 = spi_send(0);
//    rxd3 = spi_send(0);
//    spi_clear_ta();
//    //spi_read_cmd();
//    assert_mask(GPIO_GPSET0, RAIO_RS_PIN| RAIO_CS_PIN | RAIO_RD_PIN);
//    assert( RAIO_OE_PIN);

//    printf("Command Read results : \n"
//           "RXD = %X\n"
//           "RXD2= %X\n"
//           "RXD3= %X\n", rxd, rxd2, rxd3);

//}

void TFT_SPI_protocol_debug()
{
    printf("Entering %s \n", __PRETTY_FUNCTION__);
    spi_device_debug_t *dev = spi_get_debug_dev();
    spi_session_t *session = NULL;
    uint32_t pins_state = 0;

    int mask = (1 << PIN_37) | (1 << PIN_35)
            | (1 << PIN_33) | (1 << PIN_31);
    int i = 0;
    int j = 0;
    /*
    int cs , rs, oe,rst,wr,rd, ce1;
    cs = rs = oe = rst = wr = rd = ce1 = 0;
    */

    if(dev)
    {

        gpio_pin_mode(PIN_37, OUTPUT);
        gpio_pin_mode(PIN_35, OUTPUT);
        gpio_pin_mode(PIN_33, OUTPUT);
        gpio_pin_mode(PIN_31, OUTPUT);
        gpio_set_pudclock(GPIO_GPPUDCLK0, mask, GPIO_PUD_DOWN);

        deassert_mask(GPIO_GPCLR0, mask);
        TFT_init_board();
        TFT_hard_reset();
        RAIO_init();
        printf("----------------------------------------\n"
               "------- TFT SPI PROTOCOL DEBUG ---------\n"
               "----------------------------------------\n");
        for(i = 0; i < dev->session_count; i++)
        {
            session = &dev->sessions[i];
            printf("SESSION %d : --------------------\n", i);
            printf("COMMAND             : 0X%X\n"
                   "DATA                : 0X%X\n"
                   "Byte received       : %d\n", session->cmd_sent, session->data_sent, session->rx_count);
            for(j = 0; j < 3; j++)
            {
                printf("##################################\n"
                       "## Pin states at protocol Stage %d :\n", j);
                pins_state = session->pins_states[j];
                printf("* RS PIN     : %X\n"
                       "* CS PIN     : %X\n"
                       "* WR PIN     : %X\n"
                       "* RD PIN     : %X\n"
                       "* OE PIN     : %X\n"
                       "* RST PIN    : %X\n"
                       "* CE1 PIN    : %X\n",
                       GPIO_PIN_LEVEL(pins_state, RAIO_RS_PIN), GPIO_PIN_LEVEL(pins_state, RAIO_CS_PIN),
                       GPIO_PIN_LEVEL(pins_state, RAIO_WR_PIN), GPIO_PIN_LEVEL(pins_state, RAIO_RD_PIN),
                       GPIO_PIN_LEVEL(pins_state, RAIO_OE_PIN), GPIO_PIN_LEVEL(pins_state, RAIO_RST_PIN),
                       GPIO_PIN_LEVEL(pins_state, SPI_CE1_N_PIN));
                printf("##################################\n");
            }
            printf("------ End of SESSION %d : -------------\n"
                   "----------------------------------------\n", i);
        }
        printf("----------------------------------------\n");
    }
    else
    {
        printf("SPI device pointer is null ... \n");
    }
    printf("Leaving %s \n", __PRETTY_FUNCTION__);
    deassert_mask(GPIO_GPCLR0, (1 << PIN_37) | (1 << PIN_35)
                  | (1 << PIN_33) | (1 << PIN_31));
    delayN(WAIT_1_SECOND);
    assert_mask(GPIO_GPSET0, (1 << PIN_37) | (1 << PIN_35)
                  | (1 << PIN_33) | (1 << PIN_31));
}

void debug_popule_pin(uint32_t reg, int *pins)
{
    pins[0] = GPIO_PIN_LEVEL(reg, RAIO_CS_PIN);
    pins[1] = GPIO_PIN_LEVEL(reg, RAIO_RS_PIN);
    pins[2] = GPIO_PIN_LEVEL(reg, RAIO_OE_PIN);
    pins[3] = GPIO_PIN_LEVEL(reg, RAIO_RST_PIN);
    pins[4] = GPIO_PIN_LEVEL(reg, RAIO_WR_PIN);
    pins[5] = GPIO_PIN_LEVEL(reg, RAIO_RD_PIN);
    pins[6] = GPIO_PIN_LEVEL(reg, SPI_CE1_N_PIN);
}
