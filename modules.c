#include "modules.h"
#include <string.h>


#define BUFFER_SIZE 96

void spi_demo()
{
    char *text= "\nHere is the 2nd part of the text !";
    char *str = "Hello C-Berry !\n"
                "Bare Metal Programming on "
                "Raspberry PI 2.\n"
                "Computer Engineering ...\n"
                "Merry XMAS \n"
                "Copyright 2016 \n"
                "Author : Cyrille\n";
    core_blink(ACK_LED,5);
    TFT_init_board();
    TFT_hard_reset();
    RAIO_init2();
    RAIO_print(str);
}



void mini_uart_demo()
{
    //char *hello = "Hello Berry Core  !";
    char *str = "Hello C-Berry !\n"
                "Bare Metal Programming on "
                "Raspberry PI 2.\n"
                "Computer Engineering ...\n"
                "Merry XMAS \n"
                "Copyright 2016 \n"
                "Author : Cyrille\n";
    mini_uart_stream(str);
    mini_uart_tx_wait();
    //uart_close();

}
