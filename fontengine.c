#include "font5x7.h"
#include "st7735.h"


void st7735_draw_char_(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg, uint8_t size) {
    if (ch < 32 || ch > 126) {
        ch = '?';
    }

    uint8_t char_idx = ch - 32;

    for (uint8_t col = 0; col < FONT_WIDTH; col++) {
        uint8_t line = FONT5X7[char_idx][col];

        for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
            uint16_t pixel_color = (line & (1 << row)) ? color : bg;

            if (size == 1) {
                // 3 arguments : x, y, couleur
                st7735_draw_pixel(x + col, y + row, pixel_color);
            } else {
                // Pour size > 1 (ex: rect 2x2)
                st7735_fill_rect(x + (col * size), y + (row * size), size, size, pixel_color);
            }
        }
    }

    // Espacement entre les caractères (colonne de fond)
    for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
        if (size == 1) {
            st7735_draw_pixel(x + FONT_WIDTH, y + row, bg);
        } else {
            st7735_fill_rect(x + (FONT_WIDTH * size), y + (row * size), size, size, bg);
        }
    }
}


void st7735_draw_char(uint16_t x, uint16_t y, char ch, uint16_t color, uint16_t bg, uint8_t size) {
    // 1. Protection contre les caractères hors table ASCII 0-126
    if (ch > 126) {
        ch = '?';
    }

    // 2. Récupération directe grâce à notre tableau aligné ASCII
    // 5 colonnes par caractère
    for (uint8_t col = 0; col < FONT_WIDTH; col++) {
        uint8_t line = FONT5X7[(uint8_t)ch][col];

        for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
            // Test du bit courant (bit 0 = haut du caractère, bit 6 = bas)
            if (line & (1 << (6 - row))) {
                if (size == 1) {
                    st7735_draw_pixel(x + col, y + row, color);
                } else {
                    st7735_fill_rect(x + (col * size), y + (row * size), size, size, color);
                }
            } else if (bg != color) {
                // Remplissage du fond (si bg est différent de color)
                if (size == 1) {
                    st7735_draw_pixel(x + col, y + row, bg);
                } else {
                    st7735_fill_rect(x + (col * size), y + (row * size), size, size, bg);
                }
            }
        }
    }

    // 3. Espacement entre les caractères (1 colonne de fond)
    if (bg != color) {
        for (uint8_t row = 0; row < FONT_HEIGHT; row++) {
            if (size == 1) {
                st7735_draw_pixel(x + FONT_WIDTH, y + row, bg);
            } else {
                st7735_fill_rect(x + (FONT_WIDTH * size), y + (row * size), size, size, bg);
            }
        }
    }
}

/**
 * Dessine une CHAÎNE de caractères
 */
void st7735_draw_string(uint16_t x, uint16_t y, const char *str, uint16_t color, uint16_t bg, uint8_t size) {
    uint16_t cur_x = x;
    uint16_t cur_y = y;
    uint8_t char_step = (FONT_WIDTH + 1) * size;

    while (*str) {
        if (*str == '\n') {
            cur_x = x;
            cur_y += (FONT_HEIGHT + 2) * size;
        } else {
            // CORRECTION ICI : Appel de st7735_draw_char au lieu de st7735_draw_pixel !
            st7735_draw_char(cur_x, cur_y, *str, color, bg, size);
            cur_x += char_step;
        }
        str++;
    }
}