#ifndef PWM_H
#define PWM_H

#include <core.h>
#include <gpio.h>
#include <dma.h>
#include <stdio.h>

#define PWM_CHANNEL_1 1
#define PWM_CHANNEL_2 2
#define PWM_TRANSMISSION_DATA 0
#define PWM_TRANSMISSION_FIFO 2
#define PWM_0_PIN PIN_32
#define PWM_0_PIN_OFFSET 6
#define PWM_1_PIN_OFFSET 9
#define PWM_1_PIN PIN_33

#define PWM_0_ALT_PIN_OFFSET 24
#define PWM_1_ALT_PIN_OFFSET 27
#define PWM_0_PIN_ALT_F5 PIN_12
#define PWM_1_PIN_ALT_F5 PIN_35
#define PWM_DEFAULT_DATA 128
#define PWM_DEFAULT_RANGE 255
#define PWM_BASE_ADDRESS GPIO_PWM_BASE
#define PWM_BUFFER_SIZE 1024
#define PWM_DMA_CHANNEL 5
#define PWM_DMA_PERMAP 0x5

/**
 * Channel 2 M/S Enable.
 * 0 : PWM algorithm is used
 * 1 : M/S transmission is used
*/
#define PWM_CTL_MSEN2 (1 << 15) 
/**
 * Channel 1 Use FiFo
 * 0: Data register is transmitted
 * 1: FiFo is used for transmission
*/
#define PWM_CTL_USEF2 (1 << 13)
/**
 * Channel 1 Polarity
 * 0: 0 = low, 1 = high
 * 1: 1 = low, 0 = high
*/
#define PWM_CTL_POLA2 (1 << 12)
/**
 * Channel 1 Silence Bit
 * Define the state of the output when no transmission takes place.
*/
#define PWM_CTL_SBIT2 (1 << 11)
/**
 * Channel 1 Repeat Last Data
 * 0 : Transmission interrupts when FIFI is empty.
 * 1 : Last data in FiFo is transmitted repetedly until FiFo is is not empty.
*/
#define PWM_CTL_RPTL2 (1 << 10)
/**
 * Channel 1 Mode:
 * 0 : PWM mode
 * 1 : Serialiser mode
*/
#define PWM_CTL_MODE2 (1 << 9)

/**
 * Channel 1 Enable
 * 0 : Channel is disabled
 * 1 : Channel is enabled
*/
#define PWM_CTL_PWEN2 (1 << 8)

/**
 * Channel 1 M/S Enable.
 * 0 : PWM algorithm is used
 * 1 : M/S transmission is used
*/
#define PWM_CTL_MSEN1 (1 << 7) 

/**
 * Clear FiFo
 * 0 : Has no effect
 * 1 : Clear FiFo
 * Always reads 0.
*/
#define PWM_CTL_CLRF1 (1 << 6) 

/**
 * Channel 1 Use FiFo
 * 0: Data register is transmitted
 * 1: FiFo is used for transmission
*/
#define PWM_CTL_USEF1 (1 << 5)
/**
 * Channel 1 Polarity
 * 0: 0 = low, 1 = high
 * 1: 1 = low, 0 = high
*/
#define PWM_CTL_POLA1 (1 << 4)
/**
 * Channel 1 Silence Bit
 * Define the state of the output when no transmission takes place.
*/
#define PWM_CTL_SBIT1 (1 << 3)
/**
 * Channel 1 Repeat Last Data
 * 0 : Transmission interrupts when FIFI is empty.
 * 1 : Last data in FiFo is transmitted repetedly until FiFo is is not empty.
*/
#define PWM_CTL_RPTL1 (1 << 2)
/**
 * Channel 1 Mode:
 * 0 : PWM mode
 * 1 : Serialiser mode
*/
#define PWM_CTL_MODE1 (1 << 1)

/**
 * Channel 1 Enable
 * 0 : Channel is disabled
 * 1 : Channel is enabled
*/
#define PWM_CTL_PWEN1 (1 << 0)


// Channel 4 state
#define PWM_STA_STA4 (1 << 12)
// Channel 3 state
#define PWM_STA_STA3 (1 << 11)
// Channel 2 state
#define PWM_STA_STA2 (1 << 10)
// Channel 1 state
#define PWM_STA_STA1 (1 << 9)

// Bus Error Flag
#define PWM_STA_BERR (1 << 8)

// Channel 4 Gap Occured Flag
#define PWM_STA_GAPO4 (1 << 7)
// Channel 3 Gap Occured Flag
#define PWM_STA_GAPO3 (1 << 6)
// Channel 2 Gap Occured Flag
#define PWM_STA_GAPO2 (1 << 5)
// Channel 1 Gap Occured Flag
#define PWM_STA_GAPO1 (1 << 4)
//FiFo Read Error Flag
#define PWM_STA_RERR1 (1 << 3)
//FiFo Write Error Flag
#define PWM_STA_WERR1 (1 << 2)
//FiFo Empty Flag
#define PWM_STA_EMPT1 (1 << 1)
//FiFo Full Flag
#define PWM_STA_FULL1 (1 << 0)

//DMA Enable
#define PWM_DMAC_ENAB (1 << 31)
//DMA Threshold for PANIC signal
#define PWM_DMAC_PANIC (0xFF << 8)

//DMA Threshold for DREQ signal
#define PWM_DMAC_DREQ (0xFF << 0)


typedef struct PWM_REG {

   volatile uint32_t CTL; // PWM COntrol
   volatile uint32_t STA; // PWM Status
   volatile uint32_t DMAC; // PWM DMA Configuration
   volatile uint32_t RESERVED_1; // Reserved
   volatile uint32_t RNG1; // PWM Channel 1 Range
   volatile uint32_t DAT1; // PWM Channel 1 Data
   volatile uint32_t FIF1; // PWM FIFO Input
   volatile uint32_t RESERVED_2; // Reserved
   volatile uint32_t RNG2; // PWM Channel 2 Range
   volatile uint32_t DAT2; // PWM Channel 2 Data


}PWM_REG;

PWM_REG *get_pwm_register();
void pwm_enable(int channel);
void pwm_transmission_source(int channel, int source);
void pwm_set_polarity(int channel, int polarity);
void pwm_silence_bit(int channel, int bit);
void pwm_set_range(int channel, int range);
void pwm_set_repeat_last_data(int channel, int repeat);
void pwm_clear_fifo();
void pwm_clear_werror();
void pwm_clear_rerror();
void pwm_clear_berror();
int pwm_has_berror();
void pwm_clear_gap_flag(int channel);
void pwm_send(int channel, int data);
void pwm_transmit();
void pwm_init();
void pwm_info();
uint32_t pwm_ctl_get_pwen(int channel);
uint32_t pwm_ctl_get_mode(int channel);
uint32_t pwm_ctl_get_rptl(int channel);
uint32_t pwm_ctl_get_sbit(int channel);
uint32_t pwm_ctl_get_pola(int channel);
uint32_t pwm_ctl_get_usef(int channel);
uint32_t pwm_ctl_get_clrf();
uint32_t pwm_ctl_get_msen(int channel);

uint32_t pwm_sta_get_sta(int pos);
uint32_t pwm_sta_get_gap(int pos);
uint32_t pwm_sta_get_berr();
uint32_t pwm_sta_get_rerr1();
uint32_t pwm_sta_get_werr1();
uint32_t pwm_sta_get_empt1();
uint32_t pwm_sta_get_full();
uint32_t pwm_dmac_get_enab();
uint32_t pwm_dmac_get_panic();
uint32_t pwm_dmac_get_dreq();

void pwm_dmac_set_enab(int enab);
void pwm_dmac_set_panic_level(int level);
void pwm_dmac_set_dreq(int dreq);


#endif // PWM_H