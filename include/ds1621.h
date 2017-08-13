#include "i2c.h"
#define DS1621_ADDRESS              0x4F
#define DS1621_TEMP_REG             0xAA
#define DS1621_TH_REG               0xA1
#define DS1621_TL_REG               0xA2
#define DS1621_CONF_REG             0xAC
#define DS1621_COUNTER_REG          0xA8
#define DS1621_SLOPE_REG            0xA9
#define DS1621_START_CONV_CMD       0xEE
#define DS1621_STOP_CONV_CMD        0x22


void ds1621_config(uint32_t config);
void ds1621_init();
uint32_t ds1621_read_hi_temp();
uint32_t ds1621_read_lo_temp();
int ds1621_read_temp();
void ds1621_start_conversion();
void ds1621_print_register(int reg, int len);
/**
 * @brief ds1621_print print all the DS1621 register content
 */
void ds1621_print();
