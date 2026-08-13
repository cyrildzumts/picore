#include "amg88xx.h"


// Symboles ASCII ordonnés du plus froid au plus chaud
static const char ASCII_GRADIENT[] = " .:-=+*#%@";
static float thermal_pixels[64];
#define GRADIENT_LEN (sizeof(ASCII_GRADIENT) - 1)


float* amg88xx_get_pixels(void) {
    return thermal_pixels;
}

void amg88xx_init(void) {

    i2c_init_without_addr();
    
    // Mode Normal (0x00)
    i2c_writebyte(AMG88XX_I2C_ADDR, AMG88XX_PCLT, 0x00);
    delayN(WAIT_10_US);

    // Flag Reset (0x3F)
    i2c_writebyte(AMG88XX_I2C_ADDR, AMG88XX_RST, 0x3F);
    delayN(WAIT_10_US);

    // Frame Rate à 10 FPS (0x00)
    i2c_writebyte(AMG88XX_I2C_ADDR, AMG88XX_FPSC, 0x00);
    delayN(WAIT_10_US);
    
    printf("AMG88xx (0x69) configure avec succes a 10 FPS.\n");
}

/**
 * @brief Convertit une température en caractère ASCII correspondant.
 * 
 * @param temp Température en °C
 * @param min_temp Température minimale de l'échelle (ex: 20.0 °C)
 * @param max_temp Température maximale de l'échelle (ex: 35.0 °C)
 * @return char Caractère ASCII représentatif
 */
char temp_to_ascii(float temp, float min_temp, float max_temp)
{
    if (temp <= min_temp) return ASCII_GRADIENT[0];
    if (temp >= max_temp) return ASCII_GRADIENT[GRADIENT_LEN - 1];

    int index = (int)(((temp - min_temp) / (max_temp - min_temp)) * GRADIENT_LEN);
    if (index >= GRADIENT_LEN) index = GRADIENT_LEN - 1;
    if (index < 0) index = 0;

    return ASCII_GRADIENT[index];
}


void amg8831_display_ascii_heatmap(void)
{
    float thermal_matrix[8][8];
    float max_temp = -100.0f;
    float min_temp = 200.0f;

    // 1. Lecture des 64 pixels un par un via i2c_readbytes(addr, reg, uint32_t*)
    for (int i = 0; i < 64; i++) 
    {
        uint32_t raw_reg = 0;
        uint32_t reg_addr = AMG88XX_PIXEL_OFFSET + (i * 2); // Chaque pixel occupe 2 octets

        i2c_readbytes(AMG88XX_I2C_ADDR, reg_addr, &raw_reg);

        // Extraction et extension de signe sur 12 bits signés
        int16_t raw_val = (int16_t)(raw_reg & 0xFFFF);
        if (raw_val & 0x800) {
            raw_val |= 0xF000;
        }

        // 1 LSB = 0.25 °C
        float temp_celsius = raw_val * 0.25f;

        int row = i / 8;
        int col = i % 8;
        thermal_matrix[row][col] = temp_celsius;

        if (temp_celsius > max_temp) max_temp = temp_celsius;
        if (temp_celsius < min_temp) min_temp = temp_celsius;
    }

    // 2. En-tête sur le terminal UART
    printf("\033[H\033[J"); // Séquence ANSI pour effacer l'écran
    printf("=== CARTE THERMIQUE AMG8831 (8x8) ===\n");
    printf("Min: %.1f deg C | Max: %.1f deg C\n", min_temp, max_temp);
    printf("-------------------------------------\n");

    // 3. Plage d'affichage réglable (ex: 22.0 °C à 32.0 °C)
    float display_min = 22.0f; 
    float display_max = 32.0f; 

    // 4. Rendu de la grille ASCII
    for (int row = 0; row < 8; row++) 
    {
        printf("| ");
        for (int col = 0; col < 8; col++) 
        {
            float temp = thermal_matrix[row][col];
            char symbol = temp_to_ascii(temp, display_min, display_max);
            
            // On double le caractère pour garder un aspect visuel carré
            printf("%c%c ", symbol, symbol); 
        }
        printf("|\n");
    }
    printf("-------------------------------------\n");
}



float amg8831_read_thermistor(void)
{
    uint32_t raw_reg = 0;
    // Appel conforme : 3 arguments, passage de l'adresse de uint32_t
    i2c_readbytes(AMG88XX_I2C_ADDR, AMG88XX_THERM_OFFSET, &raw_reg);

    // Extraction des 16 bits de poids faible (LSB d'abord)
    int16_t raw_val = (int16_t)(raw_reg & 0xFFFF);

    // Traitement du signe (complément à 2 sur 12 bits)
    if (raw_val & 0x800) {
        raw_val |= 0xF000;
    }

    return raw_val * 0.0625f; // 1 LSB = 0.0625 °C
}

void amg8831_dump_uart(void)
{
    float thermal_matrix[8][8];
    float max_temp = -100.0f;
    float min_temp = 200.0f;
    int max_row = 0, max_col = 0;

    // 1. Lecture de la thermistance
    float chip_temp = amg8831_read_thermistor();

    // 2. Lecture des 64 pixels
    // Si ta fonction i2c_readbytes lit 1 registre / 32-bit à la fois (ou un pixel de 16 bits par appel),
    // on boucle sur les registres de chaque pixel. 
    // Chaque pixel fait 2 octets (0x80, 0x82, 0x84, ..., 0xFE).
    for (int i = 0; i < 64; i++) 
    {
        uint32_t raw_reg = 0;
        uint32_t reg_addr = AMG88XX_PIXEL_OFFSET + (i * 2);

        i2c_readbytes(AMG88XX_I2C_ADDR, reg_addr, &raw_reg);

        // Assemblage/Masquage de la valeur 12-bit signée
        int16_t raw_val = (int16_t)(raw_reg & 0xFFFF);

        if (raw_val & 0x800) {
            raw_val |= 0xF000;
        }

        float temp_celsius = raw_val * 0.25f;

        int row = i / 8;
        int col = i % 8;
        thermal_matrix[row][col] = temp_celsius;

        if (temp_celsius > max_temp) {
            max_temp = temp_celsius;
            max_row = row;
            max_col = col;
        }
        if (temp_celsius < min_temp) {
            min_temp = temp_celsius;
        }
    }

    // 3. Transmission UART
    printf("\033[H\033[J"); // Effacement d'écran pour terminal UART
    printf("===================================================\n");
    printf("           LECTURE MATRICE AMG8831 (8x8)          \n");
    printf("===================================================\n");
    printf("Puce Ambiante : %.2f deg C\n", chip_temp);
    printf("Temp Min      : %.1f deg C\n", min_temp);
    printf("Temp Max      : %.1f deg C [Pixel (%d, %d)]\n", max_temp, max_row, max_col);
    printf("---------------------------------------------------\n\n");

    printf("      Col 0  Col 1  Col 2  Col 3  Col 4  Col 5  Col 6  Col 7\n");
    printf("     ------------------------------------------------------\n");

    for (int row = 0; row < 8; row++) 
    {
        printf("L%d | ", row);
        for (int col = 0; col < 8; col++) 
        {
            printf("%5.1f  ", thermal_matrix[row][col]);
        }
        printf("\n");
    }
    printf("===================================================\n");
}

// Convertit une température (°C) en couleur RGB565 (Palette thermique)
uint16_t temp_to_color(float temp) {
    if (temp < 20.0f) return ST7735_BLUE;    // Froid
    if (temp < 24.0f) return ST7735_GREEN;   
    if (temp < 28.0f) return ST7735_YELLOW;  
    if (temp < 32.0f) return 0xF860;         // Orange
    return ST7735_RED;                       // Chaud
}


void amg88xx_print_uart(const float *pixels, float t_min, float t_max) {
    // Effacement de l'écran terminal (séquence d'échappement ANSI)
    printf("\033[H\033[J");
    printf("===================================================\n");
    printf("        MATRICE THERMIQUE AMG8831 (8x8)           \n");
    printf("===================================================\n");
    printf(" Temp Min : %.2f deg C\n", t_min);
    printf(" Temp Max : %.2f deg C\n", t_max);
    printf("---------------------------------------------------\n\n");

    printf("       Col 0  Col 1  Col 2  Col 3  Col 4  Col 5  Col 6  Col 7\n");
    printf("     ------------------------------------------------------\n");

    for (int row = 0; row < 8; row++) {
        printf(" L%d | ", row);
        for (int col = 0; col < 8; col++) {
            float temp = pixels[row * 8 + col];
            printf("%5.1f ", temp);
        }
        printf("\n");
    }
    printf("===================================================\n\n");
}

// Reçoit un tableau de 64 valeurs float (AMG88xx) et dessine la matrice
void st7735_display_thermal_heatmap(const float *pixels_64) {
    const uint8_t scale = 16; // 8x16 = 128 pixels de largeur

    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            float temp = pixels_64[row * 8 + col];
            uint16_t color = temp_to_color(temp);

            uint8_t x = col * scale;
            uint8_t y = row * scale;

            st7735_fill_rect(x, y, scale, scale, color);
        }
    }
}

void amg88xx_read_pixels_(float *pixel_buffer) {
    uint32_t raw_buffer[128]; // Buffer temporaire pour stocker les octets lus

    // Lecture rapide des 128 registres successifs à partir de 0x80
    // (64 pixels * 2 octets : LSB et MSB)
    for (int i = 0; i < 128; i++) {
        raw_buffer[i] = i2c_readbytes(AMG88XX_I2C_ADDR, AMG88XX_T01L + i, &raw_buffer[i]);
    }

    // Conversion des octets LSB/MSB en valeurs float (°C)
    for (int i = 0; i < 64; i++) {
        uint8_t lsb = (uint8_t)raw_buffer[i * 2];
        uint8_t msb = (uint8_t)raw_buffer[i * 2 + 1];

        // Reconstitution de la valeur 12 bits signée
        int16_t raw_value = (uint16_t)(msb << 8) | lsb;

        // Extension de signe si négatif (bit 11 à 1)
        if (raw_value & 0x0800) {
            raw_value |= 0xF000;
        }

        // Conversion en °C (pas de 0.25 °C)
        pixel_buffer[i] = raw_value * 0.25f;
    }
}
void amg88xx_read_pixels(float *pixel_buffer) {
    for (int i = 0; i < 64; i++) {
        uint32_t reg_lsb = AMG88XX_T01L + (i * 2);
        uint32_t reg_msb = reg_lsb + 1;

        // Lecture des octets
        // 1. Lecture brute sur 32 bits
        uint32_t raw_lsb = i2c_readbyte(AMG88XX_I2C_ADDR, reg_lsb);
        uint32_t raw_msb = i2c_readbyte(AMG88XX_I2C_ADDR, reg_msb);

        uint16_t lsb = (uint16_t)(raw_lsb & 0xFF);
        uint16_t msb = (uint16_t)(raw_msb & 0xFF);

        // Assemblage 16 bits
        uint16_t raw16 = ((uint16_t)msb << 8) | lsb;

        // Extension de signe propre pour entier 12 bits signé (bits 0 à 11)
        int16_t val12;
        if (raw16 & 0x0800) { 
            // Si le bit 11 est à 1 (température négative)
            val12 = (int16_t)(raw16 | 0xF000);
        } else {
            // Température positive
            val12 = (int16_t)(raw16 & 0x0FFF);
        }

        // Conversion finale (1 LSB = 0.25 °C)
        pixel_buffer[i] = (float)val12 * 0.25f;
    }
}
