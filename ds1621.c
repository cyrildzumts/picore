#include "ds1621.h"
#include <display.h>
void ds1621_config(uint32_t config)
{
    printf("%s started...\n", __PRETTY_FUNCTION__);
    //i2c_send(DS1621_ADDRESS_WRITE);
    //i2c_send(DS1621_ACCESS_CONF);
    //i2c_send(config);
    printf("%s leaving ...\n", __PRETTY_FUNCTION__);
}

void ds1621_init()
{
    printf("%s started...\n", __PRETTY_FUNCTION__);
    //i2c_send(DS1621_ADDRESS_WRITE);
    //i2c_send(DS1621_START_CONV);
    //ds1621_config(8);
    printf("%s leaving ...\n", __PRETTY_FUNCTION__);
}

uint32_t ds1621_read_hi_temp()
{
    return 0;
}

uint32_t ds1621_read_lo_temp()
{
    return 0;
}

int ds1621_read_temp()
{
    Packet data;
    char buffer[2] = {0};
    char str[128] = {0};
    data.buffer = buffer;
    data.len = 2;
    i2c_read_reg(DS1621_TEMP_REG, &data);
    sprintf(str, "Current Temperature:  %d : \n", buffer[0]);
    RAIO_print(str);
    //printf("-----------------------------------\n");
    printf("Current Temperature:  %d : \n", buffer[0]);
    return 0;

}

void ds1621_print_register(int reg, int len)
{
    Packet data;
    int i = 0;
    char buffer[I2C_FIFO_SIZE] = {0};
    data.buffer = buffer;
    data.len = len;
    i2c_read_reg(reg, &data);
    printf("-----------------------------------\n");
    printf("Register %X : ",  reg);
    for(i = 0; i < len; i++)
    {
        printf("%X | %d, ", buffer[i], buffer[i]);
    }
    printf("\n-----------------------------------\n");
}

void ds1621_print()
{
    ds1621_print_register(DS1621_TL_REG, 1);
    ds1621_print_register(DS1621_TH_REG, 1);
    ds1621_print_register(DS1621_CONF_REG, 1);
    ds1621_print_register(DS1621_COUNTER_REG, 1);
    ds1621_print_register(DS1621_SLOPE_REG, 1);
    ds1621_print_register(DS1621_TEMP_REG, 2);
}

void ds1621_start_conversion()
{
    Packet data;
    data.len = 0;
    i2c_write_reg(DS1621_START_CONV_CMD, &data);

}
