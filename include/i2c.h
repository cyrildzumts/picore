#ifndef I2C_H
#define I2C_H
#include "gpio.h"
#include "mu_uart.h"
#include <stdio.h>

#define I2C_SDA1_PIN  PIN_3
#define I2C_SCL1_PIN  PIN_5
#define I2C_0_BASE_ADDRESS 0x3F205000
#define I2C_1_BASE_ADDRESS 0x3F804000
/*
 * BSC2/I2C_2* master is used dedicated with the HDMI interface
 * and should not be accessed by user programs.
 */
#define I2C_2_BASE_ADDRESS 0x3F805000 // Dedicated with the HDMI Interface

// Control Register Interface
#define I2C_IC2EN               (1 << 15) // I2C Enable
#define I2C_INTR                (1 << 10) // Generate Interrupt on RX
#define I2C_INTT                (1 << 9)  // Generate Interrupt on TX
#define I2C_INTD                (1 << 8)  // Generate Interrupt on DONE
#define I2C_START               (1 << 7)  // Start a new transfer. One shot operation
#define I2C_CLEAR               (3 << 4)  // Clear FIFO. One shot operation.
#define I2C_READ                (1 << 0)  // Read Transfer Type

// Status Register Interface
#define I2C_CLKT                (1 << 9)  // Clock Stretch Timeout
#define I2C_ERR                 (1 << 8)  // ACK Error
#define I2C_RXF                 (1 << 7)  // FIFO Full
#define I2C_TXE                 (1 << 6)  // FIFO Empty
#define I2C_RXD                 (1 << 5)    // FIFO contains Data
#define I2C_TXD                 (1 << 4)    // FIFO can accept Data
#define I2C_RXR                 (1 << 3)    // FIFO needs Reading(full)
#define I2C_TXW                 (1 << 2)    // FIFO needs Writing(full)
#define I2C_DONE                (1 << 1)    // Transfer Done
#define I2C_TA                  (1 << 0)    // Transfer Active

// DLEN Register Interface
#define I2C_DLEN                (0xFFFF << 0) // Data length register

// Slave Address Interface
#define I2C_A                   (0x7F << 0)    // Access the first 6:0 bits.
// FIFO Register Interface
#define I2C_FIFO                (0xFF << 0)    // Access the First Byte.
#define I2C_FIFO_SIZE           16
// Clock Divider Interface

#define I2C_CDIV                (0xFFFF << 0)
// Data Delay Interface
#define I2C_DEL_FEDL            (0xFFFF << 16)
#define I2C_DEL_REDL            (0xFFFF << 0)

// CLKT Register Interface
#define I2C_CLKT_TOUT           (0xFFFF << 0)

#define ACK_ERR                 (0x1)
#define DATA_ERR                (0x2)
#define CLKT_ERR                (0x4)

typedef union DATA_DELAY_UNION
{
    struct inner
    {
        uint16_t REDL;
        uint16_t FEDL;
    }innerData;
    uint32_t data;
}DDEL;

typedef struct I2C_REG
{
    volatile uint32_t C;        // Control register
    volatile uint32_t S;        // Status register
    volatile uint32_t DLEN;     // Data Length register
    volatile uint32_t A;        // Slave Address register
    volatile uint32_t FIFO;     // Data FIFO register
    volatile uint32_t DIV;      // Clock Divider register
    volatile uint32_t DEL;      // Data Delay register
    volatile uint32_t CLKT;     // Clock Stretch Timeout register
} I2C_REG;

typedef enum
{
    /****************************
     * CDIV :
     * SCL = Core CLK / CDIV;
     * *************************/
    I2C_DIVIDER_2500    = 2500, // 100KHz : default
    I2C_DIVIDER_625     = 625, // 400KHz
    I2C_DIVIDER_150     = 150, // 1.666 MHz
    I2C_DIVIDER_148     = 148 // 1.689MHz

}I2C_CLK_DIVIDER;

typedef struct Packet
{
    char *buffer;
    int len;
}Packet;

typedef enum
{
    I2C_XFER_WRITE = 0,
    I2C_XFER_READ = 1
}TRANSFER_TYPE;

struct i2c_device
{
    I2C_REG dev;
    int is_activated;
    int is_busy;
};

//extern int i2c_byte_wait_us;
I2C_REG* i2c_get_reg();

void i2c_enable();
void i2c_disable();
void i2c_disable_clkt();
/**
 * @brief i2c_is_enabled
 * @return 0 --> I2C is disabled
 *         1 --> I2C is enable
 */
uint32_t i2c_is_enabled();
void i2c_init(uint32_t address);
void i2c_debug(const char *str); // print the I2C-REG content.

/**
 * @brief i2c_set_intR
 * @param state
 * If state = 1 --> generate interrupt while I2C_RXR = 1
 * if state = 0 --> don't generate interrupt on I2C_RXR
 */
void i2c_set_intR(int state);
/**
 * @brief i2c_set_intT
 * @param state
 * If state = 1 --> generate interrupt while I2C_TXW = 1
 * if state = 0 --> don't generate interrupt on I2C_TXW
 */
void i2c_set_intT(int state);
/**
 * @brief i2c_set_intD
 * @param state
 * If state = 1 --> generate interrupt while I2C_DONE = 1
 * if state = 0 --> don't generate interrupt on I2C_DONE
 */
void i2c_set_intD(int state);

void i2c_start();
void i2c_start_read();
/**
 * @brief i2c_start_transfer
 * @param type : 0 --> Write Transfer
 *               1 --> Read Transfer
 */
void i2c_start_transfer(int type);
void i2c_clear_fifo();
void i2c_status_clear();

void i2c_write_reg(int reg, Packet *data);
int i2c_read_reg(int reg, Packet *data);

int i2c_write(const Packet* data);
int i2c_read(Packet *data);

uint32_t i2c_transfer_type();
/**
 * @brief i2c_is_ack_error
 * @return 0 --> No errors detected
 *         1 --> Slave has not acknoledged its address.
 */
uint32_t i2c_is_ack_error();

void i2c_clear_ack_err();

/**
 * @brief i2c_is_fifo_full
 * @return 0 --> FIFO is not full
 *         1 --> FIFO is full
 */
uint32_t i2c_is_fifo_full();
/**
 * @brief i2c_is_fifo_empty
 * @return 0 --> FIFO is not empty
 *         1 --> FIFO is empty
 */
uint32_t i2c_is_fifo_empty();
/**
 * @brief i2c_contains_data
 * @return 0 --> FIFO is empty
 *         1 --> FIFO contains at least 1 byte.
 */
uint32_t i2c_contains_data();
/**
 * @brief i2c_can_accept_accept_data
 * @return 0 --> FIFO is full. Can't accept more data.
 *         1 --> FIFO has space for at least 1 byte.
 */
uint32_t i2c_can_accept_accept_data();
uint32_t i2c_fifo_need_reading();
uint32_t i2c_need_writing();

/**
 * @brief i2c_is_transfer_active
 * @return 0 --> Transfer not active
 *         1 --> Transfer active
 */
uint32_t i2c_is_transfer_active();

/**
 * @brief i2c_is_transfer_done
 * @return 0 --> Transfer not completed
 *         1 --> Transfer completed
 */
uint32_t i2c_is_transfer_done();
void i2c_clear_transfer_done();

/**
 * @brief i2c_dlen
 * @return the data length register
 */
uint32_t  i2c_dlen();
void i2c_set_dlen(uint32_t dlen);

void i2c_set_slave_address(uint32_t address);
uint32_t i2c_slave_address();

void i2c_set_cdiv(uint32_t cdiv);
/**
 * @brief i2c_cdiv
 * @return the clock divider beeing used
 */
uint32_t i2c_cdiv();

uint32_t i2c_falling_edge_delay();
uint32_t i2c_rising_edge_delay();
void i2c_set_falling_edge_delay(uint32_t fedl);
void i2c_set_rising_edge_delay(uint32_t redl);

/**
 * @brief i2c_set_stretch_timeout
 * @param timeout The number of SCL clock cycles to wait
 * after the rising edge of SCL before deciding that the
 * slave is not responding.
 */
void i2c_set_stretch_timeout(uint32_t timeout);
/**
 * @brief i2c_stretch_timeout
 * @return The number of SCL clock cycles to wait
 * after the rising edge of SCL before deciding that the
 * slave is not responding.
 * By Default the return value is 0x40.
 */
uint32_t i2c_stretch_timeout();

uint32_t i2c_is_clkt_err();

#endif // I2C_H
