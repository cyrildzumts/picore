#ifndef AUX_SPI_H
#define AUX_SPI_H
/*******************************************************
     * AUX SPI INTERFACE
     * This header define the SPI main Interface found
     * on the Raspberry PI 2.
     * This SPI interface is know as the SPI0 on the
     * BCM2835 ARM peripherals doc.
     * This interface is the SPI Master
     * This interface can work with up to 3 Chip
     * CS0 --> 00
     * CS1 --> 01
     * CS2 --> 10
     * ****************************************************/

#include "gpio.h"
#define AUX_SPI_IRQ_REG                     0x3F215000
#define AUX_SPI_ENABLES_REG                 0x3F215004
#define AUX_SPI_BASE_ADDRESS                0x3F215080

#define AUX_SPI_2_IRQ                       (1 << 2)
#define AUX_SPI_1_IRQ                       (1 << 1)

// AUXENB REG
#define AUX_SPI_2_EN                        (1 << 2)
#define AUX_SPI_1_EN                        (1 << 1)

#define AUX_SPI_CNTL0_CS_PATTERN            (7 << 17)
#define AUX_SPI_CNTL0_POST_IO               (1 << 16)
#define AUX_SPI_CNTL0_VA_CS                 (1 << 15)
#define AUX_SPI_CNTL0_VA_WIDTH              (1 << 14)
#define AUX_SPI_CNTL0_DOUT_HOLD_TIME        (3 << 12)
#define AUX_SPI_CNTL0_ENABLE                (1 << 11)
#define AUX_SPI_IN_RISING                   (1 << 10)
#define AUX_SPI_CNTL0_CLEAR_FIFO            (1 << 9)
#define AUX_SPI_CNTL0_OUT_RISING            (1 << 8)
#define AUX_SPI_CNTL0_INV_CLK               (1 << 7)
#define AUX_SPI_STAT_TX_FULL                (1 << 9)
#define AUX_SPI_STAT_TX_EMPTY               (1 << 8)
#define AUX_SPI_STAT_RX_EMPTY               (1 << 7)
#define AUX_SPI_STAT_BUSY                   (1 << 6)


// AUX_SPI_N PINS
// AUX_SPI_1 :
#define AUX_SPI1_CE2_N_PIN                  PIN_36
#define AUX_SPI1_CE1_N_PIN                  PIN_11
#define AUX_SPI1_CE0_N_PIN                  PIN_12
#define AUX_SPI1_MISO_PIN                   PIN_35
#define AUX_SPI1_MOSI_PIN                   PIN_38
#define AUX_SPI1_SCLK_PIN                   PIN_40

// AUX_SPI_2 : should not be used
#define AUX_SPI2_CE2_N_PIN                  45
#define AUX_SPI2_CE1_N_PIN                  44
#define AUX_SPI2_CE0_N_PIN                  43
#define AUX_SPI2_MISO_PIN                   40
#define AUX_SPI2_MOSI_PIN                   41
#define AUX_SPI2_SCLK_PIN                   42
/* Register masks for SPI0_CS */




typedef struct AUX_SPI_EN_REG
{
    volatile int IRQ_REG;
    volatile int SPI_EN_REG;

}AUX_SPI_EN_REG;

typedef struct AUX_SPI_REG
{
    volatile int SPI0_CONTL0_REG;
    volatile int SPI0_CONTL1_REG;
    volatile int SPI0_STAT_REG;
    volatile int RESERVED_1;
    volatile int SPI0_IO_REG;
    volatile int SPI0_PEEK_REG;
    volatile int UNUSED[11];
    volatile int SPI1_CONTL0_REG;
    volatile int SPI1_CONTL1_REG;
    volatile int SPI1_STAT_REG;
    volatile int RESERVED_2;
    volatile int SPI1_IO_REG;
    volatile int SPI1_PEEK_REG;
}AUX_SPI_REG;

AUX_SPI_EN_REG *aux_spi_en_reg();
AUX_SPI_REG *aux_spi_get_reg();

void aux_spi_init(int speed);
void aux_spi_interface_enable();
void aux_spi_chi_select(int cs);
void aux_spi_set_io_mode(int io_mode);
void aux_spi_clear_fifos();
void aux_spi_enable();
void aux_spi_invert_clk();
void aux_spi_reset_clk();
int aux_spi_tx_is_full();
int aux_spi_tx_is_empty();
int aux_spi_rx_is_empty();
int aux_spi_is_busy();
int aux_spi_data();
void aux_spi_write(char *data, int size);
void aux_spi_send(char data);


#endif // AUX_SPI_H
