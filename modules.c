#include "modules.h"
// draw sinus
// ---------------------------------------------

// defines the x-intercept and y-intercept on the TFT
#define window_left 	-M_PI
#define window_right	 M_PI
#define window_up		 2
#define window_down		-2

void example_DrawWithRAIO()
{
    // rectangle
        Draw_Square (0, 0, 319, 239);
        Text_Foreground_Color ( COLOR_BLACK );
        RAIO_StartDrawing ( SQUARE_FILL );
        Text_Foreground_Color ( COLOR_BLUE );
        Draw_Square (210, 150, 260, 200);

        // line
        Draw_Line ( 10, 230, 310, 10 );
        Text_Foreground_Color ( COLOR_GREEN );
        RAIO_StartDrawing ( LINE );

        // circle
        Draw_Circle ( 90, 65, 25 );
        Text_Foreground_Color ( COLOR_RED );
        RAIO_StartDrawing ( CIRCLE_FILL );
}

void example_WriteText(char *text)
{
    RAIO_clear_screen();

        RAIO_SetFontSizeFactor ( 0 );
        RAIO_print_text ( 10, 10, text, COLOR_BLACK , COLOR_WHITE );

        RAIO_SetFontSizeFactor ( 5 );
        RAIO_print_text( 30, 30, text, COLOR_BLACK, COLOR_RED );

        RAIO_SetFontSizeFactor ( 10 );
        RAIO_print_text ( 60, 70, text, COLOR_BLACK, COLOR_BLUE );

        RAIO_SetFontSizeFactor ( 15 );
        RAIO_print_text ( 80, 120, text, COLOR_BLACK, COLOR_GREEN );
}

void example_DrawFunction(int16_t function)
{
    float x_real, y_real;
        int16_t count = 0;

        RAIO_clear_screen();

        // draw x-axis
        draw_coords_line ( window_left, 0, window_right, 0 );
        for( count = (int16_t)window_left; count < (int16_t)window_right; count++ )
        {
            Draw_Line ( scale_x( count ), scale_y( window_up*0.01 ), scale_x( count ), scale_y( window_down*0.01 ) );
            Text_Foreground_Color ( COLOR_WHITE );
            RAIO_StartDrawing ( LINE );
        }

        // draw y-axis
        draw_coords_line ( 0, window_up, 0, window_down );
        for( count = (int16_t)window_down; count < (int16_t)window_up; count++ )
        {
            Draw_Line ( scale_x( window_left*0.01 ), scale_y( count ), scale_x( window_right*0.01 ), scale_y( count ) );
            Text_Foreground_Color ( COLOR_WHITE );
            RAIO_StartDrawing ( LINE );
        }


        // draw function
        for( x_real = window_left; x_real < window_right; x_real=x_real+0.02 )
        {
            switch (function) // -> see FUNCTIONS
            {
                case SIN:      y_real = sin(x_real);   break;
                case COS:      y_real = cos(x_real);   break;
                case TAN:      y_real = tan(x_real);   break;
                case PARABOLA: y_real = x_real * x_real; break;
                case EXPONENT: y_real = exp(x_real);   break;
                case LOGN    : y_real = log(x_real);   break;
                default: break;
            };
            set_point( x_real, y_real);
        }
}

uint16_t scale_x(float_t x)
{
    return ( (uint16_t) (( x - window_left ) * ( DISPLAY_WIDTH / (window_right-window_left))) );
}

uint16_t scale_y(float_t y)
{
    uint16_t temp_y = 0;

    temp_y = DISPLAY_HEIGHT - (uint16_t) (( y - window_down) *  (DISPLAY_HEIGHT / (window_up-window_down)));
    if( temp_y > 239 ) { temp_y--; }

    return ( temp_y );
}

void draw_coords_line(float_t x1, float_t y1, float_t x2, float_t y2)
{
    Text_Foreground_Color( COLOR_WHITE );
    Draw_Line( scale_x( x1 ), scale_y( y1 ), scale_x( x2 ), scale_y( y2 ) );
    RAIO_StartDrawing( LINE );
}

void set_point(float_t x_real, float_t y_real)
{
    if ((x_real> window_left) & (x_real < window_right) & (y_real > window_down) & (y_real <window_up))
        {
            Text_Foreground_Color( COLOR_WHITE );
            Draw_Line( scale_x( x_real ), scale_y( y_real ), scale_x( x_real ), scale_y( y_real ) );
            RAIO_StartDrawing( LINE );
        }
}
