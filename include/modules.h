#ifndef MODULES_H_
#define MODULES_H_

#include "display.h"
#include <math.h>
extern double log(double x);

void example_DrawWithRAIO( void );


// delete screen and write text
// ---------------------------------------------
void example_WriteText(char *text );


// draw function // -> see FUNCTIONS
// ---------------------------------------------
void example_DrawFunction( int16_t function );

// enumeration of drawing modes
enum FUNCTIONS { SIN, COS, TAN, PARABOLA, EXPONENT, LOGN };


// transform real world data in TFT data
// ---------------------------------------------
uint16_t scale_x( float_t x );

uint16_t scale_y( float_t y );


// draw coordinate system and function points
// ---------------------------------------------
void draw_coords_line(float_t x1, float_t y1, float_t x2, float_t y2);

void set_point( float_t x_real, float_t y_real );



#endif // MODULES_H_
