#ifndef SPI_H
#define SPI_H

#include "gpio.h"
#include "timer.h"
#include "aux_spi.h"
#define CLOCK_DIVIDER 2
//#define CLOCK_DIVIDER_1                 (1 << 0)
#define CLOCK_DIVIDER_2                 (1 << 1)    // 125MHz
#define CLOCK_DIVIDER_4                 (1 << 2)    // 62.5MHz
#define CLOCK_DIVIDER_8                 (1 << 3)    // 31.24MHz
#define CLOCK_DIVIDER_16                (1 << 4)    // 15.625MHz
#define CLOCK_DIVIDER_32                (1 << 5)    // 7.8125MHz
#define CLOCK_DIVIDER_64                (1 << 6)    // 3.90625MHz
#define CLOCK_DIVIDER_128               (1 << 7)    // 1.953125MHz
#define CLOCK_DIVIDER_256               (1 << 8)    // 976.5625KHz
#define CLOCK_DIVIDER_512               (1 << 9)    // 488.28125KHz
#define CLOCK_DIVIDER_1024              (1 << 10)   // 244.140625KHz
#define CLOCK_DIVIDER_2048              (1 << 11)   // 122.07KHz
#define CLOCK_DIVIDER_4096              (1 << 12)   // 61.0351KHz
#define CLOCK_DIVIDER_8192              (1 << 13)   // 30.5175KHz
//#define CLOCK_DIVIDER_16384             (1 << 14)
//#define CLOCK_DIVIDER_32768             (1 << 15)
#define SPI2_EN 0x04
#define SPI1_EN 0x02
#define SPI_EN 0x0800
#define SPI_BASE_ADDRESS                    0x3F204000

// SPI CS REG
#define SPI_CS_EN_LEN_LONG                     (1 << 25)
#define SPI_CS_DMA_LEN                         (1 << 24)
#define SPI_CS_CSPOL2                          (1 << 23)
#define SPI_CS_CSPOL1                          (1 << 22)
#define SPI_CS_CSPOL0                          (1 << 21)
#define SPI_CS_RXF                             (1 << 20) // RX FIFO FULL
#define SPI_CS_RXR                             (1 << 19) // RX FIFO need reading
#define SPI_CS_TXD                             (1 << 18)
#define SPI_CS_RXD                             (1 << 17)
#define SPI_CS_DONE                            (1 << 16)
#define SPI_CS_LEN                             (1 << 13)
#define SPI_CS_REN                             (1 << 12)
#define SPI_CS_ADCS                            (1 << 11)
#define SPI_CS_INTR                            (1 << 10)
#define SPI_CS_INTD                            (1 << 9)
#define SPI_CS_DMAEN                           (1 << 8)
#define SPI_CS_TA                              (1 << 7)
#define SPI_CS_CSPOL                           (1 << 6)
#define SPI_CS_CLEAR                           (3 << 4)
#define SPI_CS_TX_CLEAR                        (1 << 4)
#define SPI_CS_RX_CLEAR                        (2 << 4)
#define SPI_CS_CPOL                            (1 << 3)
#define SPI_CS_CPHA                            (1 << 2)
#define SPI_CS_CS0                             (0x00)
#define SPI_CS_CS1                             (1 << 0)
#define SPI_CS_CS2                             (2 << 0)
#define SPI_CS_CS                              (3 << 0) // default : no slave selected
// SPI PINS
/***
 * MOSI :   GPIO 10
 * MISO :   GPIO 09
 * CLK  :   GPIO 11
 * CE0  :   GPIO 08
 * CE1  :   GPIO 07
 * ***/
#define SPI_PINS_ALT                           (GPIO_PIN_ALT0)
#define SPI_MOSI_PIN                           (PIN_19)
#define SPI_MISO_PIN                           (PIN_21)
#define SPI_CLK_PIN                            (PIN_23)
#define SPI_CE0_N_PIN                          (PIN_24)
#define SPI_CE1_N_PIN                          (PIN_26)


#define SPI_DBG_INTERFACE_EN_PIN PIN_33
#define SPI_DBG_DATA_RCV_PIN PIN_35
#define SPI_DBG_DATA_SENT_PIN PIN_37
#define SPI_DBG_TA_PIN PIN_37

#define SPI_BUFFER_SIZE 16 // SPI has a 16 Byte buffer Size.
#define SPI_MAX_DEBUG_SESSION 2000

typedef enum SPI_MODE
{
    MODE0 = 0,
    MODE1,
    MODE2,
    MODE3
}SPI_MODE;

typedef enum SPI_CHIP_SELECT
{
    CS0 = 0,     /*00 */
    CS1 = 1,     /*01 */
    CS2 = 2,
    NONE = 3
}SPI_CHIP_SELECT;



typedef struct DC_REG
{
    volatile unsigned char TDREQ;
    volatile unsigned char TPANIC;
    volatile unsigned char RDREQ;
    volatile unsigned char RPANIC;

}DC_REG;



typedef struct SPI_REG
{
    volatile int CS;
    volatile int FIFO;
    volatile int CLK;
    volatile int DLEN;
    volatile int LTOH;
    DC_REG DC; // SPI DMA Controls
} SPI_REG;

typedef struct spi_device_t
{
    SPI_REG *dev;
    int connected_dev;
    int is_busy;
    int is_initialized;
    int buffer_size;
    int dev_status;
    uint32_t max_speed;
    uint32_t mode;
    uint32_t cs;

}spi_device_t;

typedef struct spi_session_t
{
    uint32_t pins_states[3];
    uint32_t data_sent;
    uint32_t cmd_sent;
    uint32_t rx_buffer[SPI_BUFFER_SIZE];
    volatile int rx_count;
}spi_session_t;

typedef struct spi_device_debug_t
{
    //static SPI_REG *dev = NULL;
    spi_session_t sessions[SPI_MAX_DEBUG_SESSION];
    volatile int session_count;
}spi_device_debug_t;

spi_device_debug_t *spi_get_debug_dev();
SPI_REG* spi_get_reg();
void spi_interface_enable();
void spi_init();
void spi_close();
int spi_read();
void spi_set_clk_divider(uint16_t divider);
void spi_set_datamode(uint8_t mode);
void spi_chip_select(uint8_t cs);
void spi_set_cs_pol(uint8_t cs, uint8_t active);

void spi_clear_fifos();
void spi_clear_rx_fifo();
void spi_clear_tx_fifo();
void spi_set_ta();
void spi_clear_ta();
void spi_quick_send(uint32_t *data, int len);
uint32_t spi_send(uint8_t data);
void spi_write_dbg(uint8_t *data, int len);
void spi_write(uint8_t *data,int len);
// write and read data from fifo
void spi_transfer(uint8_t *data, int len, uint8_t *rx_buffer, int rx_len);
// SPI DEBUG
/**
 * @brief spi_debug Prints the  SPI Registers
 */
void spi_debug();
uint32_t spi_cs_len_long();
void spi_set_len_long(int bit_mode);
uint32_t spi_cs_dma_len();
uint32_t spi_cs_cspol2();
uint32_t spi_cs_cspol1();
uint32_t spi_cs_cspol0();
uint32_t spi_cs_rxf();
uint32_t spi_cs_rxr();
uint32_t spi_cs_txd();
uint32_t spi_cs_rxd();
uint32_t spi_cs_done();
uint32_t spi_cs_len();
uint32_t spi_cs_ren();
uint32_t spi_cs_adcs();
uint32_t spi_cs_intr();
uint32_t spi_cs_intd();
uint32_t spi_cs_dmaen();
uint32_t spi_cs_ta();
uint32_t spi_cs_cspol();
void spi_cs_clear(uint32_t mask);
uint32_t spi_cs_cpol();
uint32_t spi_cs_cpha();
uint32_t spi_cs_cs();
uint32_t spi_aux_enable();
int spi_is_interface_en();
int spi_data_received();
int spi_data_sent();
int spi_is_transfert_active();
void spi_irq_enable(uint32_t mask);


void spi_write_cmd(int reg);
void spi_read_cmd();
void spi_protocol_debug(spi_device_debug_t *dev);
void spi_test();
#endif //  SPI_H
