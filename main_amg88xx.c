


#include <stdio.h>
#include <cpu.h>
#include <system.h>
#include "amg88xx.h"

extern void _interrupt_enable();

static void get_min_max_temp(const float *pixels, float *min_temp, float *max_temp) {
    float min_val = pixels[0];
    float max_val = pixels[0];

    for (int i = 1; i < 64; i++) {
        if (pixels[i] < min_val) min_val = pixels[i];
        if (pixels[i] > max_val) max_val = pixels[i];
    }

    // Assigner les valeurs locales aux pointeurs de sortie
    *min_temp = min_val;
    *max_temp = max_val; // <-- Correction ici (*max_temp au lieu de *max_val)
}


int main(void)
{ 
    int pin = PIN_29;
    float t_min = 0.0f;
    float t_max = 0.0f;
    float *pixels = amg88xx_get_pixels();
    printf("%s started\n", __PRETTY_FUNCTION__);
    i2c_enable();
    st7735_init();
    st7735_fill_screen(ST7735_BLACK);
    amg88xx_init();
    st7735_draw_string(2, 132, "CAM THERMIQUE", ST7735_WHITE, ST7735_BLACK, 1);
    irqEnableTimerIrq();
    printf("\nEnabling interrupts\n");
    _interrupt_enable();
    printf("IRQs enabled\n");
    ArmTimeInit();
    printf("Demarrage de la boucle d'affichage thermique dynamique...\n");
    printf("Entering sleep mode\n");
    while(1)
    {
        arm_sleep();
        printf("ARM waiked up by an IRQ\n");
        core_blink(pin, 1);
        //i2c_scan();
        //amg8831_dump_uart();
        amg88xx_read_pixels(pixels);
        get_min_max_temp(pixels, &t_min, &t_max);
        //st7735_display_thermal_heatmap(thermal_pixels);
        amg88xx_print_uart(pixels, t_min, t_max);
        st7735_render_thermal_dynamic(pixels, t_min, t_max);
        char text_buf[32];
        snprintf(text_buf, sizeof(text_buf), "Min:%.1fC Max:%.1fC", t_min, t_max);
        printf("Affichage du texte %s\n", text_buf);
        st7735_draw_string(2, 148, text_buf, ST7735_YELLOW, ST7735_BLACK, 1);
    }
    return 0;
}
