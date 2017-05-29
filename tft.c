


#include "raio8870.h"
#include "tft.h"




// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board ( void )
{
    // *************** set the pins to be an output and turn them on

    gpio_set_pin_OUT(RAIO_OE_PIN);
    gpio_set_pin(RAIO_OE_PIN);

    gpio_set_pin_OUT(RAIO_RST_PIN);
    gpio_set_pin(RAIO_RST_PIN);

    gpio_set_pin_OUT(RAIO_CS_PIN);
    gpio_set_pin(RAIO_CS_PIN);

    gpio_set_pin_OUT(RAIO_RS_PIN);
    //gpio_set_pin(RAIO_RS);


    gpio_set_pin_OUT(RAIO_WR_PIN);
    gpio_set_pin(RAIO_WR_PIN);

    gpio_set_pin_OUT(RAIO_RD_PIN);
    gpio_set_pin(RAIO_RD_PIN);

        // INPUTS
    gpio_set_pin_IN(RAIO_WAIT_PIN);
    gpio_set_pud(RAIO_WAIT_PIN,GPIO_PUD_UP);

    gpio_set_pin_IN(RAIO_INT_PIN);
    gpio_set_pud(RAIO_INT_PIN,GPIO_PUD_UP);


    // SPI INIT
    spi_init();

    gpio_set_pin(PIN_33);
}


// hard reset of the graphic controller and the tft
// ----------------------------------------------------------
void TFT_hard_reset( void )
{
    deassert(RAIO_RST_PIN);
    delayN(10);
    assert(RAIO_RST_PIN);
    delayN(1);
}


// wait during raio is busy
// ----------------------------------------------------------
void TFT_wait_for_raio ( void )
{
    while ( !gpio_read_pin_level( RAIO_WAIT_PIN ) );
}


// write data via SPI to tft
// ----------------------------------------------------------
void TFT_SPI_data_out ( uint16_t data )
{
    union my_union number;
    char buffer[2];

    number.value = data;
    buffer[0] = (char) number.split.high;
    buffer[1] = (char) number.split.low;
    spi_write((uint8_t*)&buffer[0], 2 );
}


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( uint16_t reg )
{

    assert( RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);
    deassert( RAIO_WR_PIN);
    deassert( RAIO_OE_PIN);

    TFT_SPI_data_out ( reg );

    assert( RAIO_WR_PIN );
    assert(RAIO_CS_PIN);
    assert(RAIO_OE_PIN);
}


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( uint16_t data )
{ 
    deassert( RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);
    deassert( RAIO_WR_PIN);
    deassert( RAIO_OE_PIN);
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

}

void TFT_print( char *data, uint32_t count )
{
    uint32_t i;
    deassert(RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);
    deassert(RAIO_OE_PIN);

    for(i = 0; i < count ; i++)
    {
        deassert(RAIO_WR_PIN);
        spi_send(data[i]);
        assert(RAIO_WR_PIN);
    }


    assert(RAIO_CS_PIN);
    assert(RAIO_OE_PIN);
}

void TFT_MemoryWrite()
{
    assert(RAIO_RS_PIN);
    deassert(RAIO_CS_PIN);

    assert(RAIO_RD_PIN);
    delayN(30);
    deassert(RAIO_WR_PIN);
    spi_send(MRWC);
    assert(RAIO_WR_PIN);
    assert(RAIO_CS_PIN);
    deassert(RAIO_RS_PIN);
    delayN(10);
    deassert(RAIO_CS_PIN);
}
void TFT_MemoryWriteEnd()
{
    assert(RAIO_RS_PIN);
    assert(RAIO_RD_PIN);
    assert(RAIO_WR_PIN);
    assert(RAIO_CS_PIN);
}

//void TFT_write_clear()
//{

//    gpio_set_pin( RAIO_RS);
//    gpio_clear_pin( RAIO_CS);
//    gpio_clear_pin( RAIO_WR);
//    gpio_clear_pin( OE);

//}

//void TFT_write_close()
//{
//    gpio_set_pin( RAIO_WR );
//    gpio_set_pin( RAIO_CS);
//    gpio_set_pin( OE);
//}

void TFT_StartWrite()
{
//    gpio_clear_pin( RAIO_RS);
//    gpio_clear_pin(RAIO_CS);
//    gpio_clear_pin( RAIO_WR);
//    gpio_clear_pin( OE);

}

void TFT_Stopwrite()
{
//    gpio_set_pin( RAIO_WR );
//    gpio_set_pin(RAIO_CS);
//    gpio_set_pin( OE);
}

uint8_t TFT_DataRead(int reg)
{
    /*
    uint8_t data = 0;
    TFT_RegWrite(reg);
    gpio_clear_pin( RAIO_RS);
    gpio_clear_pin(RAIO_CS);
    gpio_set_pin( RAIO_WR);
    gpio_clear_pin( RAIO_RD);
    gpio_clear_pin( OE);

    data = (uint8_t)spi_read();

    gpio_set_pin( RAIO_RD );
    gpio_set_pin(RAIO_CS);
    gpio_set_pin( OE);
    return data;
    */
    return 0;
}
