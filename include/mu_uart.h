#ifndef MU_UART_H
#define MU_UART_H

#include "gpio.h"
#include "timer.h"
#define UART_BASE 0x3F201000

/*******************************************************
 * UART INTERFACE
 * *****************************************************
*/
#define AUX_START_ADDRESS           0x3F215000
#define AUX_ENABLES_REG             0x3F215004
#define UART_ENABLE                 0x1
#define AUX_MU_IRQ_PENDING          (1 << 0)
#define AUX_MU_EN                   (1 << 0)

#define AUX_MU_IER_FIFO_CLEAR       (3 << 1)
#define AUX_MU_FIFO_RX_CLEAR        (1 << 1)
#define AUX_MU_FIFO_TX_CLEAR        (1 << 2)

#define AUX_MU_LCR_DLAB_ACCESS      (1 << 7)
#define AUX_MU_DLAB_ACCESS          (1 << 7)
#define AUX_MU_LCR_BREAK            (1 << 6)
#define AUX_MU_BREAK                (1 << 6)
#define AUX_MU_LCR_DATA_SIZE        (3 << 0)
#define AUX_MU_LCR_DATA_SIZE_8BIT   (3 << 0)
#define AUX_MU_DATA_SIZE            (3 << 0)
#define AUX_MU_DATA_SIZE_8BIT       (3 << 0)

#define AUX_MU_RTS                  (1 << 1)
#define AUX_MU_MCR_RTS              (1 << 1)

#define AUX_MU_TX_IDLE              (1 << 6)
#define AUX_MU_LSR_TX_IDLE          (1 << 6)
#define AUX_MU_TX_EMPTY             (1 << 5)
#define AUX_MU_LSR_TX_EMPTY         (1 << 5)
#define AUX_MU_LSR_RECEIVER_OVERRUN (1 << 1)
#define AUX_MU_RECEIVER_OVERRUN     (1 << 1)
#define AUX_MU_DATA_READY           (1 << 0)
#define AUX_MU_LSR_DATA_READY       (1 << 0)

#define AUX_MU_MSR_CTS              (1 << 5)
#define AUX_MU_CTS                  (1 << 5)

#define AUX_MU_CNTL_CTS_ASSERT      (1 << 7)
#define AUX_MU_CTS_ASSERT           (1 << 7)
#define AUX_MU_CNTL_RTS_ASSERT      (1 << 6)
#define AUX_MU_RTS_ASSERT           (1 << 6)
#define AUX_MU_CNTL_CTS_FLOW        (1 << 3)
#define AUX_MU_CTS_FLOW             (1 << 3)
#define AUX_MU_CNTL_RTS_FLOW        (1 << 2)
#define AUX_MU_RTS_FLOW             (1 << 2)
#define AUX_MU_CNTL_TX_EN           (1 << 1)
#define AUX_MU_TX_EN                (1 << 1)
#define AUX_MU_CNTL_RX_EN           (1 << 0)
#define AUX_MU_RX_EN                (1 << 0)
#define AUX_MU_CNTL_TX_RX_EN        (3 << 0)

#define AUX_MU_STAT_TX_FIFO_LEVEL   (0xF << 24)
#define AUX_MU_STAT_RX_FIFO_LEVEL   (0xF << 16)
#define AUX_MU_STAT_TX_DONE         (1 << 9)
#define AUX_MU_STAT_TX_FIFO_EMPTY   (1 << 8)
#define AUX_MU_STAT_CTS             (1 << 7)
#define AUX_MU_STAT_RTS             (1 << 6)
#define AUX_MU_STAT_TX_FIFO_FULL    (1 << 5)
#define AUX_MU_STAT_RECEIVER_OVERRUN (1 << 4)
#define AUX_MU_STAT_TX_IDLE         (1 << 3)
#define AUX_MU_STAT_RX_IDLE         (1 << 2)
#define AUX_MU_STAT_SPACE_AV        (1 << 1)
#define AUX_MU_STAT_SYMBOL_AV       (1 << 0)


#define AUX_INT_EN                  (1 << 29) // BCM2835 Peripheral  P.113

#define AUX_MU_IRQ_RX_EN            0x02
#define AUX_MU_IRQ_TX_EN            0x01

#define UART_DEFAULT_BAUDRATE ((CLK_FREQ / (8 * 115200)) - 1)
#define UART_TX_ENABLE 0x2
#define UART_RX_ENABLE 0x1
#define AUX_MU_BUSY 0x001
#define DLAB_ACCESS_MASK 0x8
#define UART_RX_FIFO_FULL 0x40
#define UART_TX_FIFO_FULL 0x20
#define UART_TX_FIFO_EMPTY 0x80
#define UART_RX_FIFO_EMPTY 0x10
#define UART_TX_IDLE 0x40
#define UART_TX_DONE 0x100
#define UART_BUSY 0x8
#define UART_INIT 0x31
#define UART_DATA_MODE_8BIT 0x3 // 8 BITS MODE
#define UART_DATA_MODE_7BIT 0x0 // 7 BITS

#define UART_FIFO_ENABLE 0x10
#define UART_FIFO_CLEAR 0x6
#define UART_REGISTER_VIEW 0x3F201000
#define UART_TX_PIN PIN_8
#define UART_RX_PIN PIN_10
#define UART0_TX_PIN PIN_8
#define UART0_RX_PIN PIN_10
//#define AUX_MU_STAT_REG 0x3F215064
//#define AUX_MU_BAUD_REG 0x3F215068
#define AUX_MU_IO_BASE 0x3F215040

#define BAUD_9600       9600
#define BAUD_19200      19200
#define BAUD_38400      38400
#define BAUD_57600      57600
#define BAUD_115200     270

/***********************************************
 * ********************************************/
#define AUX_M_ENABLES  0x3F215004
#define AUX_M_IO_REG   0x3F215040
#define AUX_M_IER_REG  0x3F215044
#define AUX_M_IIR_REG  0x3F215048
#define AUX_M_LCR_REG  0x3F21504C
#define AUX_M_MCR_REG  0x3F215050
#define AUX_M_LSR_REG  0x3F215054
#define AUX_M_MSR_REG  0x3F215058
#define AUX_M_SCRATCH  0x3F21505C
#define AUX_M_CNTL_REG 0x3F215060
#define AUX_M_STAT_REG 0x3F215064
#define AUX_M_BAUD_REG 0x3F215068
/***********************************************/
/**
 * @brief The AUX_IRQ_ENABLE_REG struct is used
 * to read IRQ status and enable AUX.
 * this struct is intended to be used by the MU
 * and the SPI
 */



typedef struct AUX_IRQ_ENABLE_REG
{
    volatile int32_t AUX_IRQ;
    volatile int32_t AUX_ENABLES;
}AUX_IRQ_ENABLE_REG;

/**
 * @brief The AUX_MU_REG struct This
 * struct regroup the register needed to
 * configure the MU.
 */
typedef struct AUX_MU_REG
{
    volatile uint32_t AUX_IRQ;       // AUX IRQ STATUS
    volatile uint32_t AUX_ENABLES;   // AUX ENABLES
    volatile uint32_t reserved[14];  // UNUSED
    volatile uint32_t AUX_MU_IO_REG; // AUX MU IO Data
    volatile uint32_t AUX_MU_IER_REG;// AUX MU IRQ ENABLE
    volatile uint32_t AUX_MU_IIR_REG;// AUX MU IRQ IDENTFY
    volatile uint32_t AUX_MU_LCR_REG;// AUX MU Line Control
    volatile uint32_t AUX_MU_MCR_REG;// AUX MU Modem Control
    volatile uint32_t AUX_MU_LSR_REG;// AUX MU Line Status
    volatile uint32_t AUX_MU_MSR_REG;// AUX MU Modem Status
    volatile uint32_t AUX_MU_SCRATCH;// AUX MU Scratch
    volatile uint32_t AUX_MU_CNTL_REG;// AUX MU Extra Control
    volatile uint32_t AUX_MU_STAT_REG;// AUX MU Extra Status
    volatile uint32_t AUX_MU_BAUD_REG; // AUX MU Baudrate
}AUX_MU_REG;


enum UART_OPERATION_MODE
{
    BITS_7 = 0,
    BITS_8 = 0x3
};


AUX_MU_REG* get_aux_mu_reg();
/**
 * @brief mini_uart_enable enable the mini uart
 * interface.
 */
void mini_uart_enable();
void mu_enable();
void mu_set_dlab(int value);
/**
 * @brief mu_init iniatialize the mini uart
 * @param baudrate The baud rate at which this
 * mini uart operates
 */
void mu_init(int baudrate);
/**
 * @brief mu_irq_enable enable interrupts for
 * this mini uart
 */
void mu_irq_enable();
void uart_disable();
/**
 * @brief mini_uart_stream sends a byte stream
 * @param str the buffer containing the stream to
 * be sent
 * @param len how many byte to send
 */
void mini_uart_stream(char *str);
/**
 * @brief mini_uart_send send 1 Byte
 * @param data the byte to be sent
 */
void mini_uart_send(uint32_t data);
/**
 * @brief mini_uart_data try to read one received
 * byte
 * @return  1 byte if this mini uart has received data
 *          0 if nothing was received
 */
uint8_t mini_uart_data();
/**
 * @brief mini_uart_has_pending_int queries the pending
 * interrupts
 * @return a non null value when we have any pending interrupts
 *         0 when no pending irq
 */
int mini_uart_has_pending_int();
/**
 * @brief mini_uart_set_operation_mode defines the mode
 * at which this mini uart operates
 * @param mode the mode at this mini uart should operate.
 * 8N1
 *
 */
void mini_uart_set_operation_mode(int8_t mode);
/**
 * @brief mini_uart_set_baudrate set the baud rate at
 * which this mini uart operates
 * @param rate the baud rate value
 */
void mini_uart_set_baudrate(int16_t rate);
/**
 * @brief mini_uart_get_baudrate
 * @return the current baud rate at which this mini
 * uart operates
 */
int16_t mini_uart_get_baudrate();
/**
 * @brief mini_uart_has_data
 * @return n > 0 --> there is at least 1 Byte to be read
 *         n = 0 --> there is nothing
 */
int mini_uart_has_data();
/**
 * @brief mini_uart_can_sent
 * @return n > 0 --> the mini uart can send at least 1 byte
 *         n = 0 --> the moni uart can't send data. the fifo is full
 */
int mini_uart_can_sent();
/**
 * @brief mini_uart_is_busy
 * @return n > 0 --> this mini uart is busy sending bytes
 *         n = 0 --> can send at least 1 byte
 */
int mini_uart_is_busy();
/**
 * @brief mini_uart_clear_fifo clears the mini uart fifo
 */
void mini_uart_clear_fifo();
void mini_uart_clear_rx_fifo();
void mini_uart_clear_tx_fifo();
/**
 * @brief mini_uart_transmiter_enable enable the transmiter
 * of this mini uart
 */
void mini_uart_transmiter_enable();
/**
 * @brief mini_uart_transmiter_disable disable this mini uart transmiter
 */
void mini_uart_transmiter_disable();
/**
 * @brief mini_uart_transmiter_done
 * @return n > 0 --> the mini uart has sent every bytes
 *         n = 0 --> the mini uart has not finished
 * sending bytes
 */
int mini_uart_transmiter_done();
/**
 * @brief mini_uart_receiver_enable enable this mini uart receiver
 */
void mini_uart_receiver_enable();
/**
 * @brief mini_uart_receiver_disable disable this mini uart receiver
 */
void mini_uart_receiver_disable();
void mini_uart_tx_wait();
void mini_uart_rx_wait();

#endif // MU_UART_H
