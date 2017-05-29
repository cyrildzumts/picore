
#ifndef TFT_H
#define TFT_H

#include <stdint.h>
#include "spi.h"
#define GPIO_MASK2_EN 0x01200000 // PIN 17,18  SEL_REG 1
#define GPIO_MASK3_EN 0x00009200 // PIN 22,23,24,25,27
// define pins on connector P1
#define RAIO_CS_PIN        PIN_24 // OUTPUT
#define RAIO_RS_PIN        PIN_12 // OUTPUT
#define RAIO_MOSI_PIN      PIN_19 // OUT
#define RAIO_MISO_PIN      PIN_21 // IN
#define RAIO_SCLK_PIN      PIN_23 // OUT
#define SPI_CE1            PIN_26 // OUT
#define RAIO_STCP_PIN      PIN_26 // OUT
#define RAIO_OE_PIN        PIN_11 // OUTPUT
#define RAIO_RST_PIN       PIN_22 // OUTPUT

#define RAIO_WR_PIN   PIN_18 // OUTPUT
#define RAIO_RD_PIN   PIN_16 // OUTPUT
#define RAIO_WAIT_PIN PIN_15 // INTPUT
#define RAIO_INT_PIN  PIN_13 // INTPUT



// declaration of a union (see RAIO8870.h)
// ----------------------------------------------------------
extern union my_union number;


// initialization of GPIO and SPI
// ----------------------------------------------------------
void TFT_init_board( void );


// hard reset of the graphic controller and the tft
// ----------------------------------------------------------
void TFT_hard_reset( void );	


// wait during raio is busy
// ----------------------------------------------------------
void TFT_wait_for_raio ( void );


// write byte to register
// ----------------------------------------------------------
void TFT_RegWrite( uint16_t reg );


// write byte to tft
// ----------------------------------------------------------
void TFT_DataWrite( uint16_t data );
uint8_t TFT_DataRead(int reg);

// write 'count'-bytes to tft
// ----------------------------------------------------------
void TFT_DataMultiWrite( uint16_t *data, uint32_t count );
void TFT_print(char *data, uint32_t count );
void TFT_MemoryWrite();
void TFT_MemoryWriteEnd();
void TFT_StartWrite();
void TFT_Stopwrite();

// setup before  writing
//void TFT_write_clear();

//void TFT_write_close();

#endif
