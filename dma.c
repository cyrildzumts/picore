#include <dma.h>

DMA_CB_REG *dma_get_reg(int channel){
    return (DMA_CB_REG*)(DMA_BASE + DMA_CHANNEL_SET(channel));
}

uint32_t *dma_get_int_status_reg(){
    return (uint32_t*)(DMA_INT_STATUS_BASE);
}

uint32_t *dma_get_enable_reg(){
    return (uint32_t*)(DMA_ENABLE_BASE);
}

void dma_reset(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    reg->CS |= DMA_CS_RESET;
}
void dma_abort(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    reg->CS |= DMA_CS_ABORT;
}
void dma_wait_for_outstanding_writes(int channel, int value){
    DMA_CB_REG* reg = dma_get_reg(channel);
    reg->CS |= BITS_L_SHITF(value,28);
}
void dma_panic_priority_level(int channel, int level){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->CS;
    content &= ~(DMA_CS_PANIC_PRIORITY);
    content |= BITS_L_SHITF(0x0F & level,20);
    reg->CS = content;
}
void dma_set_priority_level(int channel, int level){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->CS;
    content &= ~(DMA_CS_PRIORITY);
    content |= BITS_L_SHITF(0x0F & level,16);
    reg->CS = content;
}

void dma_clear_end(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->CS;
    content &= ~(DMA_CS_END);
    content |= BITS_L_SHITF(0x01,1);
    reg->CS = content;
}

void dma_activate(int channel, int active){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->CS;
    content &= ~(DMA_CS_ACTIVE);
    content |= BITS_L_SHITF(0x01 & active,0);
    reg->CS = content;
}

uint32_t dma_has_error(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_ERROR) >> 8;
}
uint32_t dma_is_waiting_for_outstanding_write(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_WAITING_FOR_OUTSTANDIG_WRITE) >> 6;
}
uint32_t dma_is_paused_by_dreq(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_DREQ_STOPS_DMA) >> 5;
}
uint32_t dma_is_paused(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_PAUSED) >> 4;
}
uint32_t dma_dreq(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_DREQ) >> 3;
}
uint32_t dma_has_interrupt_state(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_INT) >> 2;
}
uint32_t dma_end(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_END) >> 1;
}
uint32_t dma_is_active(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->CS & DMA_CS_ACTIVE) >> 0;
}
void dma_enable(int channel){
    if(channel > DMA_MAX_CHANNEL){
        return;
    }
    uint32_t *reg = dma_get_enable_reg();
    (*reg) |= BITS_L_SHITF(0x1, channel);

}
uint32_t dma_read_int_status(int channel){
    uint32_t *reg = dma_get_int_status_reg();
    return ((*reg) & BITS_L_SHITF(0x1, channel)) >> channel;
}
uint32_t dma_read_enable_state(int channel){
    uint32_t *reg = dma_get_enable_reg();
    return ((*reg) & BITS_L_SHITF(0x1, channel)) >> channel;
}


uint32_t dma_ti_read_permap(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->TI & DMA_TI_PERMAP) >> 16;
}
void dma_ti_set_permap(int channel, int permap){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->TI;
    content &= ~(DMA_TI_PERMAP);
    content |= BITS_L_SHITF(0x1F & permap,16);
    reg->TI = content;
}
uint32_t dma_ti_src_dreq(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    return (reg->TI & DMA_TI_SRC_DREQ) >> 10;

}
void dma_ti_set_src_dreq(int channel, int dreq){
    DMA_CB_REG* reg = dma_get_reg(channel);
    uint32_t content = reg->TI;
    content &= ~(DMA_TI_SRC_DREQ);
    content |= BITS_L_SHITF(0x1 & dreq,10);
    reg->TI = content;
}
void dma_info(int channel){
    DMA_CB_REG* reg = dma_get_reg(channel);
    printf("DMA Enable : %u\nDMA Error : %u\nDMA Waiting for writre : %u\nDMA DREQ Stop DMA : %u\nDMA Paused : %u\nDMA DREQ : %u\nDMA INT : %u\nDMA END : %u\nDMA Active : %u\n",
        dma_read_enable_state(channel),dma_has_error(channel), dma_is_waiting_for_outstanding_write(channel), dma_is_paused_by_dreq(channel), dma_is_paused(channel),dma_dreq(channel),dma_has_interrupt_state(channel), dma_end(channel), dma_is_active(channel));
}