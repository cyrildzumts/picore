#ifndef PCA9685_H
#define PCA9685_H

#include <core.h>
#include <events.h>
#include <i2c.h>

#define I2C_PCA9685_ADDR        0x40
#define I2C_PCA9685_BUS_ADDR    0x55
#define PCA9685_SOFT_RESET_ADDR 0x06
#define DEFAULT_FREQUENCY       1000
#define PCA9685_REG_MODE1       0x00
#define PCA9685_REG_MODE2       0x01
#define PCA9685_REG_SUBADR1     0x02
#define PCA9685_REG_SUBADR2     0x03
#define PCA9685_REG_SUBADR3     0x04
#define PCA9685_REG_ALLCALLADR  0x05
#define PCA9685_RESET_ADDR      0x06

#define PCA9685_PRESCALE_OFFSET 254
#define PCA9685_MODE1_RESTART   (0x1 << 7)
#define PCA9685_MODE1_AI        (0x1 << 5)
#define PCA9685_MODE1_SLEEP     (0x1 << 4)
#define PCA9685_MODE1_SUB1      (0x1 << 3)
#define PCA9685_MODE1_SUB2      (0x1 << 2)
#define PCA9685_MODE1_SUB3      (0x1 << 1)
#define PCA9685_MODE1_ALLCALL   (0x1 << 0)
#define PCA9685_LED0_ON_L       0x06
#define PCA9685_LED0_ON_H       0x07
#define PCA9685_LED0_OFF_L      0x08
#define PCA9685_LED0_OFF_H      0x09

#define PCA9685_ALL_LED_ON_L    0xFA
#define PCA9685_ALL_LED_ON_H    0xFB
#define PCA9685_ALL_LED_OFF_L   0xFC
#define PCA9685_ALL_LED_OFF_H   0xFD
#define PCA9685_PRESCALE_ADDR   0xFE
#define PCA9685_PRESCALE_MIN    4
#define PCA9685_PRESCALE_MAX    255
#define PCA9685_PRESCALE_DEFAULT 0x79 //121
#define PCA9685_TESTMODE        0xFF

#define PCA9685_LED_OFFSET      0x4
#define PCA9685_LED_MAX         16



#define PCA9685_RESTART_BIT     7
#define PCA9685_AI_BIT          5
#define PCA9685_SLEEP_BIT       4
#define PCA9685_SUB1_BIT        3
#define PCA9685_SUB2_BIT        2
#define PCA9685_SUB3_BIT        1
#define PCA9685_ALLCALL_BIT     0
#define PCA9685_WAIT            500

//#define I2C_DIVIDER_2500        1500
#define PCA9685_DEMO_LED_ON_L   0x99
#define PCA9685_DEMO_LED_ON_H   0x01
#define PCA9685_DEMO_LED_OFF_L  0xCC
#define PCA9685_DEMO_LED_OFF_H  0x04
#define PCA9685_I2C_CDIV I2C_DIVIDER_2500
#define PCA9685_MAX_COUNT       0x1000 // 4096
#define PCA9685_LED_L_MASK   (0xFF << 0)
#define PCA9685_LED_H_MASK   (0x0F << 8)



void pca9685_init();
void pca9685_restart();
void pca9685_mode1_set_restart(uint32_t restart);
void pca9685_mode1_set_autoincrement(uint32_t inc);
void pca9685_mode1_set_sleep(int sleep);
void pca9685_mode1_set_sub1(int sub);
void pca9685_mode1_set_sub2(int sub);
void pca9685_mode1_set_sub3(int sub);
void pca9685_mode1_set_allcall(int call);
void pca9685_send(uint32_t data);
void pca9685_debug();
void pca9685_led_demo();
void pca9685_clear_buffer();
void pca9685_set_duty_cycle(uint32_t duty);
void pca9685_set_pwm_frequency(uint32_t freq);
void pca9685_set_pwm(uint8_t channel, uint16_t on_count, uint16_t off_count);
void pca9685_set_servo_angle(uint8_t channel, float angle);


#endif //PCA9685_H