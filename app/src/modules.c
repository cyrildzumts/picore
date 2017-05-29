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
    //int year = 2016;
    //char *author = "Cyrille";
    //char buffer[BUFFER_SIZE];
    int len = strlen(str);
    //char *ptr = (char*)malloc(len);
    core_blink(ACK_LED,2);
    //memset(ptr, 0, len);
    //memcpy(ptr, str, len);
    //strcpy(ptr, str);
    //core_blink(ACK_LED,2);
    //sprintf(buffer,"Hello %s\n", author);
    core_blink(ACK_LED,5);
    TFT_init_board();
    TFT_hard_reset();
    RAIO_init2();
    RAIO_print(str);
    delayN(50000);
    //RAIO_print(text);
    //RAIO_newline();
    //RAIO_clear_screen();
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
    int count = strlen(str);

    mini_uart_stream(str, count);
    mini_uart_tx_wait();
    //uart_close();

}
