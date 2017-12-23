#ifndef BOARD_HEADER_H
#define BOARD_HEADER_H
#include <stdint.h>
#include <stdio.h>
// RASPBERRY PI 2 GPIO Base address
#define PERI_BASE 0x3F000000
// GPIO Base address
#define GPIO_BASE               (PERI_BASE + 0x00200000)

// CLOCK Address
#define CLOCK_BASE              (PERI_BASE + 0x00101000)

// BSC0 Address
#define BSC0_BASE               (PERI_BASE + 0x00205000)
// BSC1 Address
#define BSC1_BASE               (PERI_BASE + 0x00804000)
// SPI BASES ADDRESSES
#define SPI_BASE                (PERI_BASE + 0x00204000)
#define AUX_SPI_IRQ_REG         (PERI_BASE + 0x00215000)
#define AUX_SPI_ENABLES_REG     (PERI_BASE + 0x00215004)
#define AUX_SPI_BASE            (PERI_BASE + 0x00215080)

// System Timer Adress
#define FREE_RUNNING_TIMER_BASE (PERI_BASE + 0x00003000)

// ARM Timer :

#define ARMTIMER_BASE           (PERI_BASE + 0x0000B400)
//Base Address of the PWM registers
#define GPIO_PWM_BASE           (PERI_BASE + 0x0020C000)


// DMA ENTRY
// CHANNEL 0-14
#define DMA_BASE                (PERI_BASE + 0x00007000)
#define DMA15_BASE              (PERI_BASE + 0x00E05000)
#endif // BOARD_HEADER_H
