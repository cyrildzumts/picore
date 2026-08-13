#include <stdio.h>
#include <stdint.h>
#include "i2c.h"
#include "st7735.h"

#define AMG88XX_I2C_ADDR      0x69   // Adresse I2C par défaut (ou 0x68 si AD_SELECT à VCC)
#define AMG88XX_PIXEL_OFFSET  0x80   // Registre du premier pixel (Pixel 0)
#define AMG88XX_THERM_OFFSET  0x0E   // Registre de la thermistance interne

// Registres AMG88xx
#define AMG88XX_PCLT          0x00  // Power Control Register
#define AMG88XX_RST           0x01  // Reset Register
#define AMG88XX_FPSC          0x02  // Frame Rate Register (10fps / 1fps)
#define AMG88XX_T01L          0x80  // Début du registre des 64 pixels (Pixel 1 Low byte)


float* amg88xx_get_pixels(void);
/**
 * @brief Initialise le capteur AMG88xx et le contrôleur I2C
 * 
 * Configure la broche GPIO du capteur et initialise le bus I2C avec le bon diviseur de fréquence
 */
void amg88xx_init(void);

/**
 * @brief Lit les 64 pixels du capteur AMG88xx et les stocke dans un tableau de floats
 * 
 * @param pixel_buffer Pointeur vers un tableau de 64 floats pour stocker les températures
 */
void amg88xx_read_pixels(float *pixel_buffer);


/**
 * @brief Convertit une température en caractère ASCII correspondant.
 * 
 * @param temp Température en °C
 * @param min_temp Température minimale de l'échelle (ex: 20.0 °C)
 * @param max_temp Température maximale de l'échelle (ex: 35.0 °C)
 * @return char Caractère ASCII représentatif
 */
char temp_to_ascii(float temp, float min_temp, float max_temp);

/**
 * @brief Lit l'AMG8831 et affiche la matrice 8x8 sur l'UART sous forme de carte thermique
 */
void amg8831_display_ascii_heatmap(void);


/**
 * @brief Lit la température de la thermistance interne du capteur (Température ambiante de la puce)
 */
float amg8831_read_thermistor(void);

/**
 * @brief Envoie l'intégralité de la grille 8x8 sur l'UART avec mise en page
 */
void amg8831_dump_uart(void);

void amg88xx_print_uart(const float *pixels, float t_min, float t_max);

/**
 * @brief Convertit une température en couleur RGB565
 * 
 * @param temp Température en °C
 * @return uint16_t Couleur RGB565
 */
uint16_t temp_to_color(float temp);

/**
 * @brief Affiche la carte thermique sur l'ecran ST7735
 * 
 * @param pixels Pointeur vers un tableau de 64 floats representant la matrice 8x8 de temperatures
 */
void st7735_display_thermal_heatmap(const float *pixels_64);
 