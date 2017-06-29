#include "init.h"

static char boot_str[256] = {0};
void boot_debug()
{

}


void boot_init()
{
    core_blink(ACK_LED, 5);
    sprintf(boot_str, "Raspberry Pi 2 Boot initialization started\n");
    mini_uart_stream(boot_str);
    //RAIO_print(boot_str);
    memset(boot_str, 0, strlen(boot_str));
    printCPSRState();
    printSPSRState();
    printHSRState();
}
