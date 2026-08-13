


#include <stdio.h>
#include <cpu.h>
#include <system.h>
#include "st7735.h"
#include "i2c.h"
#include "amg88xx.h"

extern void _interrupt_enable();

int main(void)
{ 
    int pin = PIN_29;
    printf("%s started\n", __PRETTY_FUNCTION__);
    i2c_enable();
    //i2c_init_without_addr();
    //i2c_init(AMG88xx_I2C_ADDR);
    pca9685_init();
    st7735_init();
    st7735_fill_screen(ST7735_BLACK);
    float fake_amg88xx_data[64];
    for (int i = 0; i < 64; i++) {
        fake_amg88xx_data[i] = 18.0f + (i % 16); // Valeurs fictives entre 18°C et 33°C
    }
    printf("Affichage de la matrice thermique...\n");
    st7735_display_thermal_heatmap(fake_amg88xx_data);
    irqEnableTimerIrq();
    printf("\nEnabling interrupts\n");
    _interrupt_enable();
    printf("IRQs enabled\n");
    ArmTimeInit();
    printf("Entering sleep mode\n");
    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        core_blink(pin, 1);
        i2c_scan();
        amg8831_dump_uart();
    }
    return 0;
}
