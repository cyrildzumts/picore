#include "init.h"
#include <pca9685.h>

static char boot_str[256] = {0};
void boot_init()
{
    //ACK_LED
    extern uint32_t _core_started;
    gpio_pin_mode(PIN_37, OUTPUT);
    gpio_pin_mode(PIN_35, OUTPUT);
    gpio_pin_mode(PIN_33, OUTPUT);
    //gpio_cm_init();
    delayN(10);
    core_blink(PIN_37, 5);
    core_blink(ACK_LED, 5);
    core_blink(PIN_32, 5);
    printf("Raspberry Pi 2 Boot initialization started with core = %d\n", _core_started);
    //RAIO_print(boot_str);
    dma_info(PWM_DMA_CHANNEL);
    //pwm_init();

    printProcessorInfo();
    printCPSRState();
    pca9685_init();
}


void minimal_init(){
    extern uint32_t _core_started;
    int mask = BITS_L_SHITF(1, PIN_37) | BITS_L_SHITF(1, PIN_35) | BITS_L_SHITF(1, PIN_33) | BITS_L_SHITF(1, PIN_29) | BITS_L_SHITF(1, PIN_31);
    gpio_pin_mode(PIN_37, OUTPUT);
    gpio_pin_mode(PIN_35, OUTPUT);
    gpio_pin_mode(PIN_33, OUTPUT);
    gpio_pin_mode(PIN_29, OUTPUT);
    gpio_pin_mode(PIN_31, OUTPUT);
    gpio_set_pud(PIN_37, GPIO_PUD_OFF);
    gpio_set_pud(PIN_33, GPIO_PUD_OFF);
    gpio_set_pud(PIN_35, GPIO_PUD_OFF);
    gpio_set_pud(PIN_31, GPIO_PUD_OFF);
    gpio_set_pud(PIN_29, GPIO_PUD_DOWN);
    gpio_clear_pins(mask, 0);
    delayN(10);
    printf("Raspberry Pi 2 Boot initialization started with core = %d\n", _core_started);
    printCPSRState();
}
