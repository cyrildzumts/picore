#ifndef DMA_H
#define DMA_H

#include <core.h>
#include <stdio.h>

#define DMA_CHANNEL_OFFSET 0x100
#define DMA_INT_STATUS_BASE (DMA_BASE + 0xFE0)
#define DMA_ENABLE_BASE (DMA_BASE + 0xFF0)

#define DMA_CS_RESET (0x1 << 31)
#define DMA_CS_ABORT (0x1 << 30)
#define DMA_CS_DISDEBUG (0x1 << 29)
#define DMA_CS_WAIT_FOR_OUTSTANDIG_WRITE (0x1 << 28)
#define DMA_CS_PANIC_PRIORITY (0xF << 20)
#define DMA_CS_PRIORITY (0xF << 16)
#define DMA_CS_ERROR (0x1 << 8)
#define DMA_CS_WAITING_FOR_OUTSTANDIG_WRITE (0x1 << 6)
#define DMA_CS_DREQ_STOPS_DMA (0x1 << 5)
#define DMA_CS_PAUSED (0x1 << 4)
#define DMA_CS_DREQ (0x1 << 3)
#define DMA_CS_INT (0x1 << 2)
#define DMA_CS_END (0x1 << 1)
#define DMA_CS_ACTIVE (0x1 << 0)

#define DMA_TI_NO_WIDE_BURTS (0x1 << 26)
#define DMA_TI_WAITS         (0x1F << 21)
#define DMA_TI_PERMAP        (0x1F << 16)
#define DMA_TI_BURTS_LENGTH  (0xF << 12)
#define DMA_TI_SRC_IGNORE    (0x1 << 11)
#define DMA_TI_SRC_DREQ      (0x1 << 10)
#define DMA_TI_SRC_WIDTH     (0x1 << 9)
#define DMA_TI_SRC_INC       (0x1 << 8)
#define DMA_TI_DEST_IGNORE   (0x1 << 7)
#define DMA_TI_DEST_DREQ     (0x1 << 6)
#define DMA_TI_DEST_WIDTH    (0x1 << 5)
#define DMA_TI_DEST_INC      (0x1 << 4)
#define DMA_TI_WAIT_RESP     (0x1 << 3)
#define DMA_TI_TDMODE        (0x1 << 1)
#define DMA_TI_INTEN         (0x1 << 0)

#define DMA_MAX_CHANNEL     14
#define DMA_CHANNEL_ACTIVATE 0x1

#define DMA_CHANNEL_SET(x) ((x) * DMA_CHANNEL_OFFSET)

typedef struct DMA_CB_REG{ 
    volatile uint32_t CS;
    volatile uint32_t CONBLK_AD;
    volatile uint32_t TI;
    volatile uint32_t SOURCE_AD;
    volatile uint32_t DEST_AD;
    volatile uint32_t TXFR_LEN;
    volatile uint32_t STRIDE;
    volatile uint32_t NEXTCONBK;
    volatile uint32_t DEBUG;

}DMA_CB_REG;

DMA_CB_REG *dma_get_reg(int channel);
uint32_t *dma_get_int_status_reg();
uint32_t *dma_get_enable_reg();

void dma_reset(int channel);
void dma_abort(int channel);
void dma_wait_for_outstanding_writes(int channel, int value);
void dma_panic_priority_level(int channel, int level);
void dma_set_priority_level(int channel, int level);
uint32_t dma_has_error(int channel);
uint32_t dma_is_waiting_for_outstanding_write(int channel);
uint32_t dma_is_paused_by_dreq(int channel);
uint32_t dma_is_paused(int channel);
uint32_t dma_dreq(int channel);
uint32_t dma_has_interrupt_state(int channel);
uint32_t dma_end(int channel);
uint32_t dma_is_active(int channel);
void dma_activate(int channel, int active);
void dma_clear_end(int channel);
void dma_enable(int channel);
uint32_t dma_read_int_status(int channel);
uint32_t dma_read_enable_state(int channel);

uint32_t dma_ti_read_permap(int channel);
void dma_ti_set_permap(int channel, int permap);
uint32_t dma_ti_src_dreq(int channel);
void dma_ti_set_src_dreq(int channel, int dreq);

void dma_info(int channel);



#endif //DMA_H