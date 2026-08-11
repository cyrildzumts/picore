#include "pwm.h"
/******************************
 * PWM Interface
*******************************/

static PWM_REG *pwm_reg = (PWM_REG*) (PWM_BASE_ADDRESS);


PWM_REG *get_pwm_register(){
    return pwm_reg;
}

void pwm_enable(int channel){
    if(channel == PWM_CHANNEL_1){
        get_pwm_register()->CTL |= PWM_CTL_PWEN1;
    }else{
        get_pwm_register()->CTL |= PWM_CTL_PWEN2;
    }
    delayN(15);
}

void pwm_disable(int channel){
    uint32_t content = pwm_reg->CTL;
    if(channel == PWM_CHANNEL_1){
        content &= ~ (PWM_CTL_PWEN1);
    }else{
        content &= ~ (PWM_CTL_PWEN2);
    }
    pwm_reg->CTL = content;
    delayN(15);
}
void pwm_transmission_source(int channel, int source){
    uint32_t content = get_pwm_register()->CTL;
    content &= ~ (PWM_CTL_USEF1 | PWM_CTL_USEF2);
    content |= (source << 5) | (source << 13);
    get_pwm_register()->CTL = content;
}

void pwm_set_range(int channel, int range){
    printf("changing PWM range to %d\n", range);
    pwm_info();
    get_pwm_register()->RNG1 = range;
    get_pwm_register()->RNG2 = range;
    delayN(15);
    printf("changed PWM range to %d\n", range);
    pwm_info();
}

void pwm_set_mode(int channel, int mode){
    get_pwm_register()->CTL |= (PWM_CTL_MODE1 &(mode << 1));
    get_pwm_register()->CTL |= (PWM_CTL_MODE2 &(mode << 9));
    delayN(15);
}
void pwm_set_polarity(int channel, int polarity){
    if(channel == PWM_CHANNEL_1){
        get_pwm_register()->CTL |= (PWM_CTL_POLA1 &(polarity << 4));
    }else{
        get_pwm_register()->CTL |= (PWM_CTL_POLA2 &(polarity << 12));
    }
}
void pwm_silence_bit(int channel, int bit){

}
void pwm_set_repeat_last_data(int channel, int repeat){

}
void pwm_clear_fifo(){
    get_pwm_register()->CTL |= PWM_CTL_CLRF1;
}
void pwm_clear_werror(){
    get_pwm_register()->STA |= PWM_STA_WERR1;
}
void pwm_clear_rerror(){
    get_pwm_register()->STA |= PWM_STA_RERR1;
}
void pwm_clear_berror(){
    get_pwm_register()->STA |= PWM_STA_BERR;
}
int pwm_has_berror(){
    return get_pwm_register()->STA & PWM_STA_BERR;
}
void pwm_clear_gap_flag(int channel){

}
void pwm_send(int channel, int data){
    dma_activate(PWM_DMA_CHANNEL, DMA_CHANNEL_ACTIVATE);
    delayN(15);
    if(channel == PWM_CHANNEL_1){
        pwm_reg->DAT1 = data;
        //get_pwm_register()->FIF1 = data;
        pwm_reg->CTL |= PWM_CTL_PWEN1;
    }else{
        pwm_reg->DAT2 = data;
        //get_pwm_register()->FIF1 = data;
        pwm_reg->CTL |= PWM_CTL_PWEN2;
    }
    while(((pwm_reg->STA & (0x1 << 0)) >> 0)){
        delayN(10);
    }
    while(((pwm_reg->STA & (0x1 << 1)) >> 1) == 0){
        delayN(10);
    }
    if(((pwm_reg->STA & (0xF << 4)) >> 4)){
        pwm_reg->STA |= (0xF << 4);
    }
    uint32_t content = pwm_reg->CTL;
    if(channel == PWM_CHANNEL_1){
        content &= ~ (PWM_CTL_PWEN1);
    }else{
        content &= ~ (PWM_CTL_PWEN2);
    }
    pwm_reg->CTL = content;
    delayN(10);
}

void pwm_init(){
    gpio_reset_pin_setting(PIN_12);
    gpio_pin_mode(PIN_12, OUTPUT);
    gpio_clear_pin(PIN_12);
    gpio_pwm_cm_init();
    printf("Enabling PWM\n");
    pwm_set_range(PWM_CHANNEL_1, PWM_DEFAULT_RANGE);
    //pwm_enable(PWM_CHANNEL_1);
    //pwm_transmission_source(PWM_CHANNEL_1, 1);
    pwm_dmac_set_enab(1);
    delayN(15);
    printf("PWM enabled\n");
    pwm_info();
}

void pwm_info(){
    printf("PWM Info : \n");
    printf("CTL : %08X\nSTA : %08X\nDMAC : %08X\nRNG1 : %08X\nRNG2 : %08X\n", 
        pwm_reg->CTL, pwm_reg->STA, pwm_reg->DMAC, pwm_reg->RNG1, pwm_reg->RNG2);
    
    printf("PWN Channel Enable : %u %u\nMODE : %u %u\nRepeat Last Data : %u %u\nSilence Bit : %u %u\n"\
           "Use FIFO : %u %u\nM/S Enable : %u %u\n", 
           pwm_ctl_get_pwen(PWM_CHANNEL_1), pwm_ctl_get_pwen(PWM_CHANNEL_2), pwm_ctl_get_mode(PWM_CHANNEL_1), pwm_ctl_get_mode(PWM_CHANNEL_2),
           pwm_ctl_get_rptl(PWM_CHANNEL_1), pwm_ctl_get_rptl(PWM_CHANNEL_2), pwm_ctl_get_sbit(PWM_CHANNEL_1), pwm_ctl_get_sbit(PWM_CHANNEL_2),
           pwm_ctl_get_usef(PWM_CHANNEL_1), pwm_ctl_get_usef(PWM_CHANNEL_2), pwm_ctl_get_msen(PWM_CHANNEL_1), pwm_ctl_get_msen(PWM_CHANNEL_2));
    
    printf("PWM STA : %u %u %u %u\nPWM Bus Error %u\nPWM GAP : %u %u %u %u\nPWM FIFO Read Error : %u \n"\
           "PWM FIFO Write Error : %u\nPWM FIFO Empty : %u\nPWM FIFO FULL : %u\n", pwm_sta_get_sta(1), pwm_sta_get_sta(2),pwm_sta_get_sta(3),pwm_sta_get_sta(4),pwm_sta_get_berr(),
           pwm_sta_get_gap(1),pwm_sta_get_gap(2),pwm_sta_get_gap(3),pwm_sta_get_gap(4), pwm_sta_get_rerr1(),pwm_sta_get_werr1(), pwm_sta_get_empt1(),pwm_sta_get_full());


    printf("PWM DMAC Enable : %u\nPWM DMAC PANIC : 0x%02X\nPWM DMAC DREQ : 0x%02X\n", pwm_dmac_get_enab(), pwm_dmac_get_panic(), pwm_dmac_get_dreq());

    dma_info(PWM_DMA_CHANNEL);
}


uint32_t pwm_ctl_get_pwen(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & 0x1) >> 0;
    }
    return (pwm_reg->CTL & (0x1 << 8)) >> 8;
}
uint32_t pwm_ctl_get_mode(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 1)) >> 1;
    }
    return (pwm_reg->CTL & (0x1 << 9)) >> 9;
}
uint32_t pwm_ctl_get_rptl(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 2)) >> 2;
    }
    return (pwm_reg->CTL & (0x1 << 10)) >> 10;
}
uint32_t pwm_ctl_get_sbit(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 3)) >> 3;
    }
    return (pwm_reg->CTL & (0x1 << 11)) >> 11;
}
uint32_t pwm_ctl_get_pola(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 4)) >> 4;
    }
    return (pwm_reg->CTL & (0x1 << 12)) >> 12;
}
uint32_t pwm_ctl_get_usef(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 5)) >> 5;
    }
    return (pwm_reg->CTL & (0x1 << 13)) >> 13;
}
uint32_t pwm_ctl_get_clrf(){
    return (pwm_reg->CTL & (0x1 << 6)) >> 6;
}
uint32_t pwm_ctl_get_msen(int channel){
    if(channel == PWM_CHANNEL_1){
        return (pwm_reg->CTL & (0x1 << 7)) >> 7;
    }
    return (pwm_reg->CTL & (0x1 << 15)) >> 15;
}

uint32_t pwm_sta_get_sta(int pos){
    int offset = 8 + pos;
    if(pos > 4){
        return -1;
    }
    return (pwm_reg->STA & (0x1 << offset)) >> offset;
}
uint32_t pwm_sta_get_gap(int pos){
    if(pos > 4){
        return -1;
    }
    int offset = 3 + pos;
    return (pwm_reg->STA & (0x1 << offset)) >> offset;
}
uint32_t pwm_sta_get_berr(){
    return (pwm_reg->STA & (0x1 << 8)) >> 8;
}
uint32_t pwm_sta_get_rerr1(){
    return (pwm_reg->STA & (0x1 << 3)) >> 3;
}
uint32_t pwm_sta_get_werr1(){
    return (pwm_reg->STA & (0x1 << 2)) >> 2;
}
uint32_t pwm_sta_get_empt1(){
    return (pwm_reg->STA & (0x1 << 1)) >> 1;
}
uint32_t pwm_sta_get_full(){
    return (pwm_reg->STA & (0x1 << 0)) >> 0;
}
uint32_t pwm_dmac_get_enab(){
    return (pwm_reg->DMAC & (0x1 << 31)) >> 31;
}
uint32_t pwm_dmac_get_panic(){
    return (pwm_reg->DMAC & (0xFF << 8)) >> 8;
}
uint32_t pwm_dmac_get_dreq(){
    //return (pwm_reg->DMAC & (0xFF << 0)) >> 0;
    return pwm_reg->DMAC & 0xFF;
}

void pwm_dmac_set_enab(int enab){
    uint32_t content = pwm_reg->DMAC;
    content &= ~(BITS_L_SHITF(0x1, 31));
    content |= BITS_L_SHITF(0x01 & enab, 31);
    pwm_reg->DMAC = content;
}
void pwm_dmac_set_panic_level(int level){
    uint32_t content = pwm_reg->DMAC;
    content &= ~(BITS_L_SHITF(0xFF, 8));
    content |= BITS_L_SHITF(0x0FF & level, 8);
    pwm_reg->DMAC = content;
}
void pwm_dmac_set_dreq(int dreq){
    uint32_t content = pwm_reg->DMAC;
    content &= ~(BITS_L_SHITF(0xFF, 0));
    content |= BITS_L_SHITF(0x0FF & dreq, 0);
    pwm_reg->DMAC = content;
}

void pwm_transmit(){
    int i = 0;
        for(i = 0; i <= PWM_DEFAULT_RANGE; i++){
            pwm_send(PWM_CHANNEL_1, i);
            if(pwm_has_berror()){
                printf("PWM Write data %u: Bus Error\n", i);
                pwm_info();
                pwm_clear_berror();
            }
        }
        for(i = 0; i <= PWM_DEFAULT_RANGE; i++){
            pwm_send(PWM_CHANNEL_1, PWM_DEFAULT_RANGE - i);
            if(pwm_has_berror()){
                printf("PWM Write data %u : Bus Error\n", i);
                pwm_info();
                pwm_clear_berror();
            }
        }
}