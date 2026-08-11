#ifndef CORE_H
#define CORE_H
#include <stdint.h>
#include <stdio.h>
// RASPBERRY PI 2 Base Addresses
#define PERI_BASE 0x3F000000
// GPIO Base address
#define GPIO_BASE   (PERI_BASE + 0x00200000)

// CLOCK Address
#define CLOCK_BASE  (PERI_BASE + 0x00101000)
#define PWM_CLOCK_BASE  (CLOCK_BASE + 0xA0)
#define CLOCK_MANAGER_CTL_BASE (CLOCK_BASE + 0x70)

// BSC0 Address
#define BSC0_BASE   (PERI_BASE + 0x00205000)
// BSC1 Address
#define BSC1_BASE   (PERI_BASE + 0x00804000)
//Base Address of the PWM registers
#define GPIO_PWM_BASE   (PERI_BASE + 0x0020C000)

#define DMA_BASE (PERI_BASE + 0x7000)
#define DMA15_BASE (PERI_BASE + 0xE0500)

#define BITS_SHITF(X,Y)    ((X) << (Y))
#define BITS_L_SHITF(X,Y)  ((X) << (Y))
#define BITS_R_SHITF(X,Y)  ((X) >> (Y))

void print_bits(uint32_t value);

#endif //CORE_H