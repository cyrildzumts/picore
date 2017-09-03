


#include "raio8870.h"
#include "tft.h"




// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board ( void )
{
    printf("Entering %s\n", __PRETTY_FUNCTION__);
    // *************** set the pins to be an output and turn them on

    //gpio_pin_mode(RAIO_OE_PIN,OUTPUT);
    gpio_set_pin_OUT(RAIO_OE_PIN);
    assert(RAIO_OE_PIN);

    //gpio_pin_mode(RAIO_RST_PIN, OUTPUT);
    gpio_set_pin_OUT(RAIO_RST_PIN);
    assert(RAIO_RST_PIN);

    gpio_set_pin_OUT(RAIO_CS_PIN);
    assert(RAIO_CS_PIN);

    //gpio_pin_mode(RAIO_RS_PIN,OUTPUT);
    gpio_set_pin_OUT(RAIO_RS_PIN);
    assert(RAIO_RS_PIN);


    //gpio_pin_mode(RAIO_WR_PIN, OUTPUT);
    gpio_set_pin_OUT(RAIO_WR_PIN);
    assert(RAIO_WR_PIN);

    //gpio_pin_mode(RAIO_RD_PIN, OUTPUT);
    gpio_set_pin_OUT(RAIO_RD_PIN);
    assert(RAIO_RD_PIN);

        // INPUTS
    //gpio_pin_mode(RAIO_WAIT_PIN, INPUT);
    gpio_set_pin_OUT(RAIO_WAIT_PIN);
    gpio_set_pud(RAIO_WAIT_PIN,GPIO_PUD_UP);
    //gpio_pin_mode(RAIO_INT_PIN, INPUT);
    gpio_set_pin_OUT(RAIO_INT_PIN);
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
    deassert(RAIO_RST_PIN);
    delayN(WAIT_1_MILLISECOND * 100);
    assert(RAIO_RST_PIN);
    delayN(WAIT_1_MILLISECOND);
    //printf("TFT hard reset : Status Reg : %X\n",TFT_status_read_cmd());
}


// wait during raio is busy
// ----------------------------------------------------------
void TFT_wait_for_raio ( void )
{
    while ( !gpio_read_pin_level( RAIO_WAIT_PIN ) );
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

    assert( RAIO_RS_PIN);

    deassert(RAIO_CS_PIN);
    deassert( RAIO_WR_PIN);
    //assert(RAIO_RD_PIN);
    deassert( RAIO_OE_PIN);

    //deassert_mask(GPIO_GPCLR0,CS_WR_OE_MASK);

    TFT_SPI_data_out ( reg );


    assert( RAIO_WR_PIN );
    assert(RAIO_CS_PIN);
    assert(RAIO_OE_PIN);

    //assert_mask(GPIO_GPSET0, CS_WR_OE_MASK);
}


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( int data )
{ 
    deassert(RAIO_CS_PIN);
    deassert( RAIO_RS_PIN);

    deassert( RAIO_WR_PIN);
    deassert( RAIO_OE_PIN);
    assert(RAIO_RD_PIN);
    TFT_SPI_data_out ( data);
    assert( RAIO_WR_PIN );
    assert(RAIO_CS_PIN);
    assert(RAIO_OE_PIN);
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
    int data = 0;
    int i = 0;
    int rxd_count = 0;
    uint32_t buffer[SPI_BUFFER_SIZE] = {-1};
    /*
    union my_union number;
    uint8_t buffer[2];

    number.value = reg;
    buffer[0] = number.split.high;
    buffer[1] = number.split.low;
    */
    printf("Reading Register : %X\n", reg);
    assert( RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);
    deassert( RAIO_WR_PIN);
    assert(RAIO_RD_PIN);
    deassert( RAIO_OE_PIN);
    spi_clear_fifos();
    spi_set_ta();
    spi_get_reg()->FIFO = reg;
    spi_get_reg()->FIFO = 0;
    while (!spi_cs_done()) {
        //printf("...\n");
        ;
    }
    assert( RAIO_WR_PIN );
    assert(RAIO_CS_PIN);
    deassert(RAIO_RS_PIN);


    deassert(RAIO_CS_PIN);
    assert(RAIO_WR_PIN);
    deassert(RAIO_RD_PIN);
    while(spi_cs_rxd())
    {

        if(SPI_BUFFER_SIZE <= i)
            break;
        buffer[i] = spi_get_reg()->FIFO;
        rxd_count++;
        i++;
    }
    assert(RAIO_RS_PIN);
    assert(RAIO_CS_PIN);
    assert(RAIO_RD_PIN);
    assert(RAIO_OE_PIN);

    for (i = 0; i < rxd_count; i++)
    {
        printf(" %d | %X \n", i, buffer[i]);
    }
    //return buffer[rxd_count - 1];
    printf("leaving %s\n", __PRETTY_FUNCTION__);
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
    uint32_t pin_levels = 0;
    pin_levels = (gpio_get_base_ptr())[GPIO_GPLEV0];
    pin_levels = gpio_get_pin_level_register().low;


}
