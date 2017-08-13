#ifndef SPI_H
#define SPI_H

#include "gpio.h"
#include "timer.h"
#define CLOCK_DIVIDER 2
#define CLOCK_DIVIDER_1                 (1 << 0)
#define CLOCK_DIVIDER_2                 (1 << 1) //125MHz
#define CLOCK_DIVIDER_4                 (1 << 2) //62.5MHz
#define CLOCK_DIVIDER_8                 (1 << 3)
#define CLOCK_DIVIDER_16                (1 << 4)
#define CLOCK_DIVIDER_32                (1 << 5)
#define CLOCK_DIVIDER_64                (1 << 6)
#define CLOCK_DIVIDER_128               (1 << 7)
#define CLOCK_DIVIDER_256               (1 << 8)
#define CLOCK_DIVIDER_512               (1 << 9)
#define CLOCK_DIVIDER_1024              (1 << 10)
#define CLOCK_DIVIDER_2048              (1 << 11)
#define CLOCK_DIVIDER_4096              (1 << 12)
#define CLOCK_DIVIDER_8192              (1 << 13)
#define CLOCK_DIVIDER_16384             (1 << 14)
#define CLOCK_DIVIDER_32768             (1 << 15)
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
void spi_quick_send(char data);
uint8_t spi_send(uint8_t data);
void spi_write(uint8_t *data,int len);

// SPI DEBUG
int spi_is_interface_en();
int spi_data_received();
int spi_data_sent();
int spi_is_transfert_active();

#endif //  SPI_H
