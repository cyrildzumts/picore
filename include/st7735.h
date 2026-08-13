#ifndef ST7735_H
#define ST7735_H

#include <stdint.h>
#include "gpio.h"
#include "spi.h"
#include "font5x7.h"

// Définition des broches GPIO (ajuste selon ton câblage)
#define GPIO_RST  17  // Reset matériel
#define GPIO_DC   27  // Data / Command
#define ST7735_PIN_DC   27  // Broche DC (Data/Command)
#define ST7735_PIN_RST  17

// Commandes ST7735
#define ST7735_NOP     0x00
#define ST7735_SWRESET 0x01
#define ST7735_SLPOUT  0x11
#define ST7735_NORON   0x13
#define ST7735_INVOFF  0x20
#define ST7735_DISPON  0x29
#define ST7735_CASET   0x2A
#define ST7735_RASET   0x2B
#define ST7735_RAMWR   0x2C
#define ST7735_MADCTL  0x36
#define ST7735_COLMOD  0x3A


// Configuration Frame Rate & Power
#define ST7735_FRMCTR1 0xB1
#define ST7735_FRMCTR2 0xB2
#define ST7735_FRMCTR3 0xB3
#define ST7735_INVCTR  0xB4
#define ST7735_PWCTR1  0xC0
#define ST7735_PWCTR2  0xC1
#define ST7735_PWCTR3  0xC2
#define ST7735_PWCTR4  0xC3
#define ST7735_PWCTR5  0xC4
#define ST7735_VMCTR1  0xC5
#define ST7735_GMCTRP1 0xE0
#define ST7735_GMCTRN1 0xE1





// --- Dimensions de l'écran ---
#define ST7735_WIDTH    128
#define ST7735_HEIGHT   160

// --- Couleurs de base RGB565 ---
#define ST7735_BLACK   0x0000
#define ST7735_BLUE    0x001F
#define ST7735_RED     0xF800
#define ST7735_GREEN   0x07E0
#define ST7735_CYAN    0x07FF
#define ST7735_MAGENTA 0xF81F
#define ST7735_YELLOW  0xFFE0
#define ST7735_WHITE   0xFFFF

// --- Prototypes ---
void st7735_init(void);
void st7735_write_cmd(uint8_t cmd);
void st7735_write_data(uint8_t data);
void st7735_set_address_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
void st7735_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color);
void st7735_fill_screen(uint16_t color);
void st7735_draw_pixel(uint8_t x, uint8_t y, uint16_t color);
void st7735_display_thermal_heatmap(const float *pixels_64);
uint16_t temp_to_color_dynamic(float temp, float min_temp, float max_temp);
void st7735_render_thermal_dynamic(const float *pixels, float t_min, float t_max);

#endif // ST7735_H