#include "st7735.h"
#include <stdio.h>


// Délai d'attente sommaire en boucles NOP
static inline void delay_ms(uint32_t ms) {
    for (volatile uint32_t i = 0; i < ms * 5000; i++) {
        __asm__("nop");
    }
}

// Écrit une commande vers le ST7735
void st7735_write_cmd(uint8_t cmd) {
    gpio_write_pin(ST7735_PIN_DC, LOW);  // DC = 0 (Mode Commande)
    spi_send(cmd);                        // Utilise ton spi_send
}

// Écrit un octet de donnée vers le ST7735
void st7735_write_data(uint8_t data) {
    gpio_write_pin(ST7735_PIN_DC, HIGH); // DC = 1 (Mode Donnée)
    spi_send(data);                       // Utilise ton spi_send
}

// Séquence d'initialisation matérielle et logicielle
void st7735_init(void) {
    printf("Initialisation des GPIOs pour l'ecran ST7735...\n");

    // 1. Configuration des pins de contrôle GPIO
    gpio_pin_mode(ST7735_PIN_DC, OUTPUT);
    gpio_pin_mode(ST7735_PIN_RST, OUTPUT);

    // 2. Initialisation du contrôleur SPI (SPI0, CE0, DIVIDER=16 pour ~15-25MHz)
    spi_init();                             
    spi_chip_select(CS0);                  
    spi_set_clk_divider(CLOCK_DIVIDER_16); 
    spi_set_datamode(MODE0);               

    // 3. Reset matériel du ST7735
    gpio_write_pin(ST7735_PIN_RST, HIGH); 
    delay_ms(10);
    gpio_write_pin(ST7735_PIN_RST, LOW);  
    delay_ms(10);
    gpio_write_pin(ST7735_PIN_RST, HIGH);  
    delay_ms(120);

    // 4. Séquence de démarrage du contrôleur
    st7735_write_cmd(ST7735_SWRESET);
    delay_ms(150);

    st7735_write_cmd(ST7735_SLPOUT);
    delay_ms(250);

    // Configuration Frame Rate & Alimentation
    st7735_write_cmd(ST7735_FRMCTR1);
    st7735_write_data(0x01); st7735_write_data(0x2C); st7735_write_data(0x2D);

    st7735_write_cmd(ST7735_PWCTR1);
    st7735_write_data(0xA2); st7735_write_data(0x02); st7735_write_data(0x84);

    st7735_write_cmd(ST7735_PWCTR2);
    st7735_write_data(0xC5);

    st7735_write_cmd(ST7735_PWCTR3);
    st7735_write_data(0x0A); st7735_write_data(0x00);

    st7735_write_cmd(ST7735_PWCTR4);
    st7735_write_data(0x8A); st7735_write_data(0x2A);

    st7735_write_cmd(ST7735_PWCTR5);
    st7735_write_data(0x8A); st7735_write_data(0xEE);

    st7735_write_cmd(ST7735_VMCTR1);
    st7735_write_data(0x0E);

    st7735_write_cmd(ST7735_INVOFF);

    // Dynamic Address & Color Mode (RGB565 = 16 bit/pixel)
    st7735_write_cmd(ST7735_MADCTL);
    st7735_write_data(0xC8); // Orientation verticale (Portrait)

    st7735_write_cmd(ST7735_COLMOD);
    st7735_write_data(0x05); // Format 16-bit RGB565

    delay_ms(10);

    st7735_write_cmd(ST7735_NORON);
    delay_ms(10);

    st7735_write_cmd(ST7735_DISPON);
    delay_ms(100);

    printf("Ecran ST7735 initialise avec succes.\n");
}

// Définit la zone rectangulaire d'écriture dans la VRAM
void st7735_set_address_window(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1) {
    st7735_write_cmd(ST7735_CASET);
    st7735_write_data(0x00); st7735_write_data(x0);
    st7735_write_data(0x00); st7735_write_data(x1);

    st7735_write_cmd(ST7735_RASET);
    st7735_write_data(0x00); st7735_write_data(y0);
    st7735_write_data(0x00); st7735_write_data(y1);

    st7735_write_cmd(ST7735_RAMWR);
}

// Dessine un rectangle rempli (rapide avec spi_transfer)
void st7735_fill_rect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, uint16_t color) {
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) return;
    if ((x + w) > ST7735_WIDTH)  w = ST7735_WIDTH - x;
    if ((y + h) > ST7735_HEIGHT) h = ST7735_HEIGHT - y;

    st7735_set_address_window(x, y, x + w - 1, y + h - 1);

    gpio_write_pin(ST7735_PIN_DC, HIGH); // Passage en mode Donnée

    // Préparation d'un tampon de transfert (MSB en premier)
    uint8_t hi = (color >> 8) & 0xFF;
    uint8_t lo = color & 0xFF;
    
    // Transfert par blocs via ton spi_transfer
    uint32_t total_pixels = w * h;
    uint8_t buffer[128];
    for (int i = 0; i < 128; i += 2) {
        buffer[i]     = hi;
        buffer[i + 1] = lo;
    }

    while (total_pixels > 0) {
        uint32_t batch_pixels = (total_pixels > 64) ? 64 : total_pixels;
        spi_transfer(buffer, NULL, batch_pixels * 2);
        total_pixels -= batch_pixels;
    }
}

// Remplissage complet de l'écran
void st7735_fill_screen(uint16_t color) {
    st7735_fill_rect(0, 0, ST7735_WIDTH, ST7735_HEIGHT, color);
}

// Dessin d'un pixel unique
void st7735_draw_pixel(uint8_t x, uint8_t y, uint16_t color) {
    if (x >= ST7735_WIDTH || y >= ST7735_HEIGHT) return;
    st7735_set_address_window(x, y, x, y);
    st7735_write_data((color >> 8) & 0xFF);
    st7735_write_data(color & 0xFF);
}

// Convertit une température en RGB565 via une échelle dynamique (Heatmap Gradient)
uint16_t temp_to_color_dynamic(float temp, float min_temp, float max_temp) {
    // Sécurité division par zéro
    if (max_temp <= min_temp) {
        max_temp = min_temp + 1.0f;
    }

    // Normalisation de la température entre 0.0f et 1.0f
    float norm = (temp - min_temp) / (max_temp - min_temp);
    if (norm < 0.0f) norm = 0.0f;
    if (norm > 1.0f) norm = 1.0f;

    // Palette : Bleu -> Cyan -> Vert -> Jaune -> Rouge
    uint8_t r = 0, g = 0, b = 0;

    if (norm < 0.25f) { // Bleu -> Cyan
        b = 255;
        g = (uint8_t)(norm * 4.0f * 255.0f);
    } else if (norm < 0.50f) { // Cyan -> Vert
        g = 255;
        b = (uint8_t)((0.50f - norm) * 4.0f * 255.0f);
    } else if (norm < 0.75f) { // Vert -> Jaune
        g = 255;
        r = (uint8_t)((norm - 0.50f) * 4.0f * 255.0f);
    } else { // Jaune -> Rouge
        r = 255;
        g = (uint8_t)((1.0f - norm) * 4.0f * 255.0f);
    }

    // Conversion RGB888 vers RGB565 pour le ST7735
    return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}

// Fonction de rendu mise à jour avec l'échelle dynamique
void st7735_render_thermal_dynamic(const float *pixels, float t_min, float t_max) {
    const uint8_t scale = 16; // 8 x 16 = 128px de largeur

    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            float temp = pixels[row * 8 + col];
            uint16_t color = temp_to_color_dynamic(temp, t_min, t_max);

            uint8_t x = col * scale;
            uint8_t y = row * scale;

            st7735_fill_rect(x, y, scale, scale, color);
        }
    }
}