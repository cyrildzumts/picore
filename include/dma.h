#ifndef DMA_H
#define DMA_H
#include <gpio.h>
#include <stdint.h>
// DMA Controls and Status
#define DMA_CS_RESET              (1 << 31)
#define DMA_CS_ABORT              (1 << 30)
#define DMA_CS_DISDEBUG           (1 << 29)
#define DMA_CS_WAIT               (1 << 28)
#define DMA_CS_PANIC_PRIORITY(n)  ((n)&0xF << 20)
#define DMA_CS_PRIORITY(n)        ((n)&0xF << 16)
#define DMA_CS_ERROR              (1 << 8)
#define DMA_CS_WAITING            (1 << 6)
#define DMA_CS_DREQ_STOPS_DMA     (1 << 5)
#define DMA_CS_PAUSED             (1 << 4)
#define DMA_CS_DREQ               (1 << 3)
#define DMA_CS_INT                (1 << 2)
#define DMA_CS_END                (1 << 1)
#define DMA_CS_ACTIVE             (1 << 0)

// DMA Transfer Information
#define DMA_TI_NO_WIDE_BURSTS     (1 << 26)
#define DMA_TI_WAITS              (0x1F << 21)
#define DMA_TI_PERMAP             (0x1F << 16)
#define DMA_TI_BURST_LENGTH       (0xF << 12)
#define DMA_TI_SRC_IGNORE         (1 << 11)
#define DMA_TI_SRC_DREQ           (1 << 10)
#define DMA_TI_SRC_WIDTH          (1 << 9)
#define DMA_TI_SRC_INC            (1 << 8)
#define DMA_TI_DEST_IGNORE        (1 << 7)
#define DMA_TI_DEST_DREQ          (1 << 6)
#define DMA_TI_DEST_WIDTH         (1 << 5)
#define DMA_TI_DEST_INC           (1 << 4)
#define DMA_TI_WAIT_RESP          (1 << 3)
#define DMA_TI_TDMODE             (1 << 1)
#define DMA_TI_INTEN              (1 << 0)

#define DMA_TI_TXFR_YLENGTH       (0x3FFF << 16)
#define DMA_TI_TXFR_XLENGTH       (0xFFFF << 0)

// DMA Interrupts status
//  0 --- n < 16
#define DMA_INT_STATUS(n)       (1 << n)

// DMA Global Enable bits
// 0 --- n < 15
#define DMA_ENGINE_EN(n)        (1 << n)

// DMA DREQ
#define DMA_DREQ_DSI            1
#define DMA_DREQ_PCM_TX         2
#define DMA_DREQ_PCM_RX         3
#define DMA_DREQ_SMI            4
#define DMA_DREQ_PWM            5
#define DMA_DREQ_SPI_TX         6
#define DMA_DREQ_SPI_RX         7
#define DMA_DREQ_BSC_TX         8
#define DMA_DREQ_BSC_RX         9
#define DMA_DREQ_eMMC           11
#define DMA_DREQ_UART_TX        12
#define DMA_DREQ_SD_HOST        13
#define DMA_DREQ_UART_RX        14
#define DMA_DREQ_DSI2           15
#define DMA_DREQ_HDMI           17

#define DMA_ALIGN               32
typedef struct dma_cb_t{
    uint32_t transfer_info;
    uint32_t *source;
    uint32_t *destination;
    int length;
    uint32_t stride;
    struct dma_cb_t *next;
    uint32_t reserved[2];
} dma_cb_t;

typedef  struct dma_cb_list_t{
    dma_cb_t *head;
    dma_cb_t *tail;
    int initialized;
    int size;
} dma_cb_list_t;

dma_cb_t *dma_get_root();
void dma_add_cb(dma_cb_t *cb);
void dma_remove(dma_cb_t *cb);
void dma_init(dma_cb_t *cb);
void dma_start();
int dma_is_empty();
int dma_size();
void dma_copy(dma_cb_t *src, dma_cb_t *dst);

#endif // DMA_H
