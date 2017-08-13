#include "i2c.h"
///static int i2c_byte_wait_us = 0;

I2C_REG *i2c_reg = (I2C_REG *)(I2C_1_BASE_ADDRESS);


I2C_REG* i2c_get_reg()
{
    return i2c_reg;
}

void i2c_enable()
{

    i2c_reg->C |= I2C_IC2EN;
    printf("I2C enabled");
}

void i2c_disable()
{
    i2c_reg->C &= ~I2C_IC2EN;
    gpio_pin_mode(I2C_SCL1_PIN, INPUT );
    gpio_pin_mode(I2C_SDA1_PIN, INPUT );
}

void i2c_init(uint32_t address)
{
    //int cdiv = 0;
    // enable Alternate Function 0 for pin 3 & 5
    gpio_alt_func_pin(I2C_SCL1_PIN, GPIO_PIN_ALT0);
    gpio_alt_func_pin(I2C_SDA1_PIN, GPIO_PIN_ALT0);
    i2c_set_slave_address(address);
    //cdiv = i2c_cdiv();
    //i2c_byte_wait_us = ((float)cdiv / CLK_FREQ) * 1000000*9;
    core_blink(PIN_35, 2);
}

uint32_t i2c_is_enabled()
{
    return (i2c_reg->C & I2C_IC2EN) >> 15;
}

void i2c_debug(const char *str)
{
    printf("I2C Module Debug %s : \n", str);
    printf("-----------------------------------------\n"
           "I2C Enable State            : %d\n"
           "I2C FIFO Full State         : %d\n"
           "I2C Transfer Active State   : %d\n"
           "I2C CDIV                    : %d\n"
           "I2C CLKT                    : %d\n"
           "I2C READ Bit                : %d\n"
           "I2C ACK_ERR Bit             : %d\n"
           "I2C CLKT ERR Bit            : %d\n"
           "I2C TX FIFO EMPTY           : %d\n"
           "I2C RX FIFO FULL            : %d\n"
           "I2C RXD FIFO contains data  : %d\n"
           "I2C TX FIFO can accept data : %d\n"
           "I2C RX FIFO needs reading   : %d\n"
           "I2C TX FIFO needs wrting    : %d\n"
           "I2C TX FIFO transfer done   : %d\n"
           "I2C DLEN                    : %d\n"
           "I2C Slave Address           : %X\n"
           "I2C FEDL                    : %d\n"
           "I2C REDL                    : %d\n"
           "Debug Done. \n"
           "-----------------------------------------\n",
           ((i2c_reg->C & I2C_IC2EN) >> 15),
           ((i2c_reg->S & I2C_RXF) >> 7),
           ((i2c_reg->S & I2C_TA) >> 0),
           i2c_reg->DIV,
           i2c_reg->CLKT,
           i2c_transfer_type(),
           i2c_is_ack_error(),
           i2c_is_clkt_err(),
           i2c_is_fifo_empty(),
           i2c_is_fifo_full(),
           i2c_contains_data(),
           i2c_can_accept_accept_data(),
           i2c_fifo_need_reading(),
           i2c_need_writing(),
           i2c_is_transfer_done(),
           i2c_dlen(),
           i2c_slave_address(),
           i2c_falling_edge_delay(),
           i2c_rising_edge_delay()
           );
}

void i2c_set_intR(int state)
{
    if (state)
    {
        i2c_reg->C |= I2C_INTR;
    }
    else
    {
        i2c_reg->C &= ~I2C_INTR;
    }
}

void i2c_set_intT(int state)
{
    if (state)
    {
        i2c_reg->C |= I2C_INTT;
    }
    else
    {
        i2c_reg->C &= ~I2C_INTT;
    }
}

void i2c_set_intD(int state)
{
    if (state)
    {
        i2c_reg->C |= I2C_INTD;
    }
    else
    {
        i2c_reg->C &= ~I2C_INTD;
    }
}

void i2c_start()
{
    i2c_reg->C |= I2C_START;


}

void i2c_clear_fifo()
{
    i2c_reg->C |= I2C_CLEAR;
}


int i2c_write(const Packet* data)
{
    printf("%s : writing to i2c module \n", __PRETTY_FUNCTION__);
    int err = 0;
    if(data && data->buffer)
    {
        volatile int remaining = data->len;
        volatile int i = 0;

        // Clear FIFO
        i2c_clear_fifo();
        // Clear Status
        i2c_status_clear();
        // set data length
        i2c_set_dlen(data->len);

        // Prepopulate FIFO with max buffer
        while(remaining && ( i < I2C_FIFO_SIZE))
        {
            i2c_reg->FIFO = data->buffer[i];
            i++;
            remaining--;
        }
        printf("%s : FIFO prepopulated \n", __PRETTY_FUNCTION__);
        i2c_debug(__PRETTY_FUNCTION__);
        // Enable device and start transfer
        printf("%s Enabling device and start transfer\n", __PRETTY_FUNCTION__);
        i2c_reg->C |= I2C_START;
        i2c_debug(__PRETTY_FUNCTION__);
        printf("%s : Waiting for Transfer DONE \n", __PRETTY_FUNCTION__);
        while(!i2c_is_transfer_done())
        {
            i2c_debug(__PRETTY_FUNCTION__);
            while(remaining && i2c_can_accept_accept_data())
            {
                i2c_reg->FIFO = data->buffer[i];
                i++;
                remaining--;
            }
        }
        printf("%s : FIFO remaining byte written \n", __PRETTY_FUNCTION__);
        if(i2c_is_ack_error())
        {
            err = ACK_ERR;
            printf("%s : ACK_ERR \n", __PRETTY_FUNCTION__);
        }
        else if(i2c_is_clkt_err())
        {
            err = CLKT_ERR;
            printf("%s : CLKT_ERR \n", __PRETTY_FUNCTION__);
        }
        else if(remaining)
        {
            err = DATA_ERR;
            printf("%s : DATA_ERR \n", __PRETTY_FUNCTION__);
        }
        i2c_clear_transfer_done();
    }

    return err;
}

int i2c_read(Packet *data)
{
    int err = 0;
    if(data)
    {
        int remaining = data->len;
        int i = 0;
        // Clear FIFO
        i2c_clear_fifo();
        // Clear Status
        i2c_status_clear();
        // Set Data length
        i2c_set_dlen(data->len);
        // Start Read
        i2c_reg->C |= (I2C_START | I2C_READ);
        // wait for transfer to complete
        while(!i2c_is_transfer_done())
        {
            while(i2c_contains_data())
            {
                data->buffer[i] = i2c_reg->FIFO;
                i++;
                remaining--;
            }
        }
        // read remaining Data in FIFO
        while(remaining && i2c_contains_data())
        {
            data->buffer[i] = i2c_reg->FIFO;
            i++;
            remaining--;
        }
        if(i2c_is_ack_error())
        {
            err = ACK_ERR;
            printf("%s : ACK_ERR \n", __PRETTY_FUNCTION__);
        }
        else if(i2c_is_clkt_err())
        {
            err = CLKT_ERR;
            printf("%s : CLKT_ERR \n", __PRETTY_FUNCTION__);
        }
        else if(remaining)
        {
            err = DATA_ERR;
            printf("%s : DATA_ERR \n", __PRETTY_FUNCTION__);
        }
        i2c_clear_transfer_done();
    }
    return err;
}


uint32_t i2c_is_ack_error()
{
    return (i2c_reg->S & I2C_ERR) >> 8;
}

void i2c_clear_ack_err()
{
    i2c_reg->S |= I2C_ERR;
}

uint32_t i2c_is_fifo_full()
{
    return (i2c_reg->S & I2C_RXF) >> 7;
}

uint32_t i2c_is_fifo_empty()
{
    return (i2c_reg->S & I2C_TXE) >> 6;
}

uint32_t i2c_contains_data()
{
    return (i2c_reg->S & I2C_RXD) >> 5;
}

uint32_t i2c_can_accept_accept_data()
{
    return (i2c_reg->S & I2C_TXD) >> 4;
}

uint32_t i2c_fifo_need_reading()
{
    return (i2c_reg->S & I2C_RXR) >> 3;
}

uint32_t i2c_need_writing()
{
    return (i2c_reg->S & I2C_TXW) >> 2;
}

uint32_t i2c_is_transfer_active()
{
    return (i2c_reg->S & I2C_TA) >> 0;
}

uint32_t i2c_is_transfer_done()
{
    return (i2c_reg->S & I2C_DONE) >> 1;
}

void i2c_clear_transfer_done()
{
    i2c_reg->S |= I2C_DONE;
}

uint32_t i2c_dlen()
{
    return i2c_reg->DLEN;
}

void i2c_set_dlen(uint32_t dlen)
{

   i2c_reg->DLEN = dlen;
}

void i2c_set_slave_address(uint32_t address)
{
    i2c_reg->A = address;
}

uint32_t i2c_slave_address()
{
    return i2c_reg->A;
}

void i2c_set_cdiv(uint32_t cdiv)
{
    i2c_reg->DIV = cdiv;
    //i2c_byte_wait_us  = ((float)cdiv / CLK_FREQ) * 1000000*9;
}

uint32_t i2c_cdiv()
{
    return i2c_reg->DIV;
}

uint32_t i2c_falling_edge_delay()
{

    return ((i2c_reg->DEL & 0xFFFF0000) >> 16);
}

uint32_t i2c_rising_edge_delay()
{
    return (i2c_reg->DEL & 0x0000FFFF);
}

void i2c_set_falling_edge_delay(uint32_t fedl)
{
    i2c_reg->DEL |= ((fedl << 16) & 0xFFFF0000);
}

void i2c_set_rising_edge_delay(uint32_t redl)
{
    i2c_reg->DEL |= (redl & 0x0000FFFF);
}

void i2c_set_stretch_timeout(uint32_t timeout)
{
    i2c_reg->CLKT = timeout;
}

uint32_t i2c_stretch_timeout()
{
    return i2c_reg->CLKT;
}

void i2c_status_clear()
{
    i2c_reg->S |= I2C_CLKT | I2C_ERR | I2C_DONE;
}

uint32_t i2c_is_clkt_err()
{
    return (i2c_reg->S & I2C_CLKT) >> 9;
}

void i2c_write_reg(int reg, Packet *data)
{
    // clear FIFO
    int i = 0;
    i2c_clear_fifo();
    // clear STATUS
    i2c_status_clear();
    // set data length
    i2c_set_dlen(1 + data->len);
    // prepopulate the fifo with reg address :
    i2c_reg->FIFO = reg;
    // enable the device and start writing transfer
    i2c_start_transfer(I2C_XFER_WRITE);
    i2c_debug(__PRETTY_FUNCTION__);
    //wait for transfer to be done
    while(!i2c_is_transfer_done())
    {
        while (((i < data->len) && (i < I2C_FIFO_SIZE)) &&
               i2c_can_accept_accept_data())
        {
            i2c_reg->FIFO = data->buffer[i];
            i++;
        }
    }
    printf("%s : FIFO remaining byte written \n", __PRETTY_FUNCTION__);
    delayN(WAIT_1_MICROSECOND * 250);
    if(i2c_is_ack_error())
    {
        printf("%s : ACK_ERR \n", __PRETTY_FUNCTION__);
    }
    else if(i2c_is_clkt_err())
    {
        printf("%s : CLKT_ERR \n", __PRETTY_FUNCTION__);
    }

    i2c_clear_transfer_done();
    i2c_status_clear();
}

int i2c_read_reg(int reg, Packet *data)
{

    int i = 0;
    i2c_clear_fifo();
    i2c_status_clear();
    i2c_set_dlen(1);
    // prepopulate the fifo with reg address :
    i2c_reg->FIFO = reg;
    i2c_start_transfer(I2C_XFER_WRITE);

    //wait for transfer to start
    while(!i2c_is_transfer_active())
    {
        if(i2c_is_transfer_done())
            break;
    }
    while(!i2c_is_transfer_done())
    {
        //i2c_debug("Transfer writing not Done");
    }
    i2c_set_dlen(data->len);
    i2c_start_transfer(I2C_XFER_READ);
    delayN(WAIT_1_MICROSECOND * 250);
    while((i < I2C_FIFO_SIZE) && i2c_contains_data())
    {
        data->buffer[i] = i2c_reg->FIFO;
        i++;
    }
    //delayN(WAIT_150_MS);
    if(i2c_is_ack_error())
    {
        printf("%s : ACK_ERR \n", __PRETTY_FUNCTION__);
    }
    else if(i2c_is_clkt_err())
    {
        printf("%s : CLKT_ERR \n", __PRETTY_FUNCTION__);
    }

    i2c_clear_transfer_done();
    i2c_status_clear();
    return 0;
}

void i2c_start_transfer(int type)
{
    uint32_t C = i2c_reg->C;
    if(type == I2C_XFER_WRITE)
    {
        C &=~I2C_READ;
    }
    else if(type == I2C_XFER_READ)
    {
        C |=I2C_READ;
    }
    i2c_reg->C = C | I2C_IC2EN | I2C_START;
}


uint32_t i2c_transfer_type()
{
    return (i2c_reg->C & I2C_READ);
}

void i2c_disable_clkt()
{
    i2c_reg->CLKT = 0;
}
