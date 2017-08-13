#ifndef DHT11_H
#define DHT11_H
#include <gpio.h>
#include <timer.h>
#include <mu_uart.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define MAXTIMINGS	85
#define BUFFER_OUT_SIZE 256
#define DHTPIN		PIN_37

void dht11_init();
void dht11_read();

#endif // DHT11_H
