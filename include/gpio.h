
/************************************************
 * Author : Cyrille Ngassam Nkwenga
 * Platform : Raspberry PI 2
 * Description : This module implements differents core
 * functionalities to control a Raspberry PI 2.
 * This module implements a GPIO Controller.
 */


#ifndef GPIO_H
#define GPIO_H
#include <stdint.h>
#define CLK_FREQ  250000000 // 250MHz

/*
 * This Enum map the physical Pins Layout
 * the internal Pin configuration
 */
enum RPI_2_PIN
{
    PIN_3   = 2,
    PIN_5   = 3,
    PIN_7   = 4, // GPIO_GCLK
    PIN_8   = 14,// TXD0 ALT0
    PIN_10  = 15,// RXD0 ALT0
    PIN_11  = 17,// GPIO_GEN0
    PIN_12  = 18,// GPIO_GEN1
    PIN_13  = 27,// GPIO_GEN2
    PIN_15  = 22,// GPIO_GEN3
    PIN_16  = 23,// GPIO_GEN4
    PIN_18  = 24,// GPIO_GEN5
    PIN_19  = 10,// SPI_MOSI
    PIN_21  = 9, //SPI_MISO
    PIN_22  = 25,// GPIO_GEN6
    PIN_23  = 11,//SPI_CLK
    PIN_24  = 8, // SPI_CE0_N
    PIN_26  = 7, // SPI_CE1_N
    PIN_29  = 5,
    PIN_31  = 6,
    PIN_32  = 12,
    PIN_33  = 13,
    PIN_35  = 19,
    PIN_36  = 16,
    PIN_37  = 26,
    PIN_38  = 20,
    PIN_40  = 21,
    ACK_LED = 47  // ACK LED available on the Raspberry PI 2
};

// RASPBERRY PI 2 GPIO Base address
#define PERI_BASE 0x3F000000
// GPIO Base address
#define GPIO_BASE   (PERI_BASE + 0x00200000)

// CLOCK Address
#define CLOCK_BASE  (PERI_BASE + 0x00101000)

// BSC0 Address
#define BSC0_BASE   (PERI_BASE + 0x00205000)
// BSC1 Address
#define BSC1_BASE   (PERI_BASE + 0x00804000)
//Base Address of the PWM registers
#define GPIO_PWM_BASE   (PERI_BASE + 0x0020C000)



// GPIO FUNCTION SELECT REGISTERS
#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5

// GPIO SET REGISTERS
#define GPIO_GPSET0     7
#define GPIO_GPSET1     8
// GPIO CLEAR REGISTERS
#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11

// GPIO PIN LEVEL REGISTERS
#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14

// GPIO EVENT DETECT STATUS REGISTERS
#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17

//GPIO RISING EDGE DETECT ENABLE REGISTERS
#define GPIO_GPREN0     19
#define GPIO_GPREN1     20

//GPIO FALLING EDGE DETECT ENABLE REGISTERS
#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23

// GPIO High Detect Enable Registers (GPHENn)
#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26

// GPIO Low Detect Enable Registers (GPLENn)
#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29

// GPIO Asynchronous rising Edge Detect Enable Registers (GPARENn)
#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32

// GPIO Asynchronous Falling Edge Detect Enable Registers (GPAFENn)
#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35

//GPIO Pull-up/down Register (GPPUD)
#define GPIO_GPPUD      37

//GPIO Pull-up/down Clock Registers (GPPUDCLKn)
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39



/*******************************************************
 * GPIO INTERFACE
 * ****************************************************/
typedef enum BASE_POINTERS
{
    BASE = 0,
    GPIO,
    TIMER,
    CLOCK,
    SPI,
    BSC0,
    BSC1,
    PMW,
    UART
} BASE_POINTERS;

/********************************************************
 * GPIO ALTERNATE FUNCTIONS
 * *****************************************************/
#define GPIO_PIN_ALT0 0x4
#define GPIO_PIN_ALT1 0x5
#define GPIO_PIN_ALT2 0x6
#define GPIO_PIN_ALT3 0x7
#define GPIO_PIN_ALT4 0x3
#define GPIO_PIN_ALT5 0x2
#define GPIO_FSEL_MASK 0x07


/*******************************************
 * GPIO Pulldown Register
 * ****************************************/
typedef enum
{
    GPIO_PUD_OFF    = 0x00,
    GPIO_PUD_DOWN   = 0x01,
    GPIO_PUD_UP     = 0x02
}GPIO_PUD;


void assert(uint8_t pin);
void deassert(uint8_t pin);



/**
     * @brief gpio_set_pin_IN set the GPIO PIN pin as INPUT
     * @param pin The GPIO PIN to be set as INPUT
     */
    void gpio_set_pin_IN( int pin);
    /**
     * @brief gpio_set_pin_as_OUT set the GPIO PIN pin as OUTPUT
     * @param pin The GPIO PIN to be set as OUTPUT
     */
    void gpio_set_pin_OUT( int pin);

    /**
     * @brief gpio_set_pins_OUT defined the pins defined
     * in mask as out put.
     * This function operates only for the pins
     * controlled by the selected SEL_REG. So this
     * only work for the 10 pins controlled by sel_reg.
     * the extra pin will be ignored.
     * @param mask defines the pins to be set. it must
     * contains max 10 pins to be set.
     * @param sel_reg the SEL_REG to apply the mask
     */

    void gpio_pin_setup(int mask, int sel_reg);
    /**
     * @brief gpio_alt_func_pin Alternate the GPIO PIN pin Function.
     * @param pin The GPIO PIN to be alternated
     * @param GPIO_ALT_FUNC The Alternate Function activated.
     */
    /*
     * Each module which want to activate the Alternate
     * function should be responsible for doing so, not
     * the GPIO alternate these pins. So I will remove
     * this function in the future.
     * To activate the desired alternate function
     * simply call the appropriated module's init function.
     */
    void gpio_alt_func_pin(int pin, int mode);

    /**
     * @brief gpio_set_pin Set the GPIO PIN pin to 1
     * @param pin The GPIO PIN to be set
     */
    void gpio_set_pin(int pin);


        /**
         * @brief gpio_set_pins Set the Pins defined in mask.
         * defines the list of pins up
         * to 31 to be set
         * @param mask defines the pins to be set by
         * sel_reg.
         * @param set_reg the SEL_REG on which the pins
         * should be set.
         */
        void gpio_set_pins(int mask, int set_reg);
    /**
     * @brief gpio_clear_pin Clear the GPIO PIN pin
     * @param pin The GPIO PIN to be cleared
     */
    void gpio_clear_pin(int pin);

        /**
         * @brief gpio_clear_pins Clear all the pins up to
         * 31.
         * @param mask defines the list of pins
         * to be cleared.
         * clr_reg the CLEAR_REGISTER to be used.
         */
        void gpio_clear_pins(int mask, int clr_reg);

    /**
     * @brief gpio_read_pin_level This function read the GPIO PIN level
     * @param pin The PIN from which we want to read the level
     * @return return the GPIO PIN Level
     * @return -1 if the pin number is not valid.
     */
    int gpio_read_pin_level(int pin);


    /**
     * @brief gpio_reset_pins Resets all used GPIO PINS if pin = 0, if
     * pin != 0 then Resets only pin
     * @param pin
     */

    /**
     * @brief gpio_set_pud activates the GPIO Pin PUD Type
     * @param pin The pin we want to activate the PUD
     * @param pud_type the PUD TYPE : Pull Down / UP Resistor
     */
    void gpio_set_pud(int pin, int pud_type);
    /**
     * @brief gpio_pud Set the Pulldown Type : Pulldown or Pull Up
     * Resistor
     * @param pud The Pull Down type
     */
    void gpio_pud(int pud);
    /**
     * @brief gpio_pudclock_ON activate the Pull Down/UP
     * clock on the PIN pin
     * @param pin The PIN we want to activate the Clock on
     * This Function does nothing if pin is invalid
     */
    void gpio_pudclock(int pin, uint8_t value);

    /**
     * @brief core_set_bits Set the register referenced by
     * addr to the value "value" with the mask mask applied
     * @param addr The Register we  want to set the value
     * @param value the new Value to set the addr to.
     * @param mask
     */
    void core_set_bits(volatile int *addr, int value, int mask);
    /**
     * @brief core_blink Utility function that blinks the LED connected
     * to the PIN pin "n" times.
     * @param pin The PIN where the LED is connected.
     * @param n How many times we want to blinks the LED.
     */
    void core_blink(int pin,int n);

    void delayN(int N);
    uint32_t *gpio_get_base_ptr();

#endif // GPIO_H
