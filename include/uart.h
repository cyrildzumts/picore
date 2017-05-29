#ifndef UART_H
#define UART_H

#include "gpio.h"
#define UART_BASE 0x3F201000


typedef struct
{
    int32_t DR; // Data register
    int32_t RSRECR; // Not in use
    int32_t FR; // Flag register
    int32_t ILPR; // Not in use
    int32_t IBRD; // Integer Baud rate divisor
    int32_t FBRD; // Fractional Baud rate divisor
    int32_t LCRH; // Line Control register
    int32_t CR ; // Control Register
    int32_t IFLS; // Interupt FIFO Level Register
    int32_t IMSC; // Interupt Mask Set Clear Register
    int32_t RIS; // Raw Interupt Status Register
    int32_t MIS; // Masked Interupt Status Register
    int32_t ICR; // Interupt Clear Register
    int32_t DMACR; // DMA Control Register
    int32_t ITCR; // Test Control Register
    int32_t ITIP; // Integration Test Input register
    int32_t ITOP; // Integration Test Output Register
    int32_t TDR; // Test Data Register
}UART_REG;


/**
 * @brief uart_interface_enable enable the uart
 * interface
 */
void uart_interface_enable();
void uart_init();
void uart_close();
#endif // UART_H
