
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
#include <stdio.h>
#include <core.h>

#define CLK_FREQ  250000000 // 250MHz

/*
 * This Enum map the physical Pins Layout
 * the internal Pin configuration
 */
enum RPI_2_PIN
{
    PIN_3   = 2, // I2C_SDA1
    PIN_5   = 3, // I2C_SCL1
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
    PIN_32  = 12, // PWM0
    PIN_33  = 13, // PWM1
    PIN_35  = 19,
    PIN_36  = 16,
    PIN_37  = 26,
    PIN_38  = 20,
    PIN_40  = 21,
    ACK_LED = 47  // ACK LED available on the Raspberry PI 2
};

#define HIGH 1
#define LOW 0

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


#define CM_DEFAULT_SRC 0x01
#define CM_DEFAULT_MASH 0x0
#define CM_DEFAULT_DIVI 1
#define CM_PASSWORD     (0x5A << 24)
#define CM_PASSWORD_BITS  24
#define CM_MASH         (0x3 << 9)
#define CM_MASK_MASH    (0x3 << 9)
#define CM_MASK_PASSWORD (0xFF << 24)
#define CM_MASK_BUSY    (0x1 << 7)
#define CM_MASK_ENAB    (0x1 << 4)
#define CM_MASK_SRC     (0xF << 0)
#define CM_MASK_DIVI    (0xFFF << 12)
#define CM_MASK_DIVF    (0xFFF << 0)

#define CM_FLIP         (1 << 8)
#define CM_BUSY         (1 << 7)
#define CM_KILL         (1 << 5)
#define CM_ENAB         (1 << 4)
#define CM_SRC          (0xF << 0)
#define CM_DIVI         (0x0FFF << 12)
#define CM_DIVF         (0x0FFF << 10)

/*******************************************************
 * MACROS D'ACCÈS RAPIDE AUX REGISTRES GPFSEL
 ******************************************************/
#define GPIO_REG(offset) (*(volatile uint32_t *)(GPIO_BASE + (offset)))

// 1. INP_GPIO(pin) : Force les 3 bits de la broche à 000 (Mode Entrée)
#define INP_GPIO(pin)  GPIO_REG((pin)/10 * 4) &= ~(7 << (((pin)%10)*3))

// 2. OUT_GPIO(pin) : Configure la broche en Sortie (001)
// Note : Toujours exécuter INP_GPIO(pin) avant OUT_GPIO(pin) !
#define OUT_GPIO(pin)  GPIO_REG((pin)/10 * 4) |=  (1 << (((pin)%10)*3))

// 3. SET_GPIO_ALT(pin, alt) : Nettoie les bits et applique le mode ALT (0 à 5)
#define SET_GPIO_ALT(pin, alt) do { \
    INP_GPIO(pin); \
    GPIO_REG((pin)/10 * 4) |= ((alt) << (((pin)%10)*3)); \
} while(0)

extern void delayN(int);

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

#define GPIO_PIN_LEVEL(REG, PIN)   ((REG & (1<< PIN)) >> PIN)

/*******************************************
 * GPIO Pulldown Register
 * ****************************************/
typedef enum
{
    GPIO_PUD_OFF    = 0x00,
    GPIO_PUD_DOWN   = 0x01,
    GPIO_PUD_UP     = 0x02
}GPIO_PUD;

typedef enum
{
    INPUT = 1,
    OUTPUT = 0
}PINMODE;

typedef struct Event_Status_Reg
{
    volatile uint32_t low;
    volatile uint32_t high;
}Event_Status_Reg;

typedef struct reg64_t{
    uint32_t low;
    uint32_t high;
} reg64_t;

typedef Event_Status_Reg Reg_64BIT_t;

union GPIO_REG_64BIT
{
    uint64_t content;
    reg64_t reg;
};

typedef struct GPIO_REGISTERS {
    volatile uint32_t GPFSEL0;
    volatile uint32_t GPFSEL1;
    volatile uint32_t GPFSEL2;
    volatile uint32_t GPFSEL3;
    volatile uint32_t GPFSEL4;
    volatile uint32_t GPFSEL5;
    volatile uint32_t RESERVED_1;
    volatile uint32_t GPSET0;
    volatile uint32_t GPSET1;
    volatile uint32_t RESERVED_2;
    volatile uint32_t GPCLR0;
    volatile uint32_t GPCLR1;
    volatile uint32_t RESERVED_3;
    volatile uint32_t GPLEV0;
    volatile uint32_t GPLEV1;
    volatile uint32_t RESERVED_4;
    volatile uint32_t GPEDS0;
    volatile uint32_t GPEDS1;
    volatile uint32_t RESERVED_5;
    volatile uint32_t GPREN0;
    volatile uint32_t GPREN1;
    volatile uint32_t RESERVED_6;
    volatile uint32_t GPFEN0;
    volatile uint32_t GPFEN1;
    volatile uint32_t RESERVED_7;
    volatile uint32_t GPHEN0;
    volatile uint32_t GPHEN1;
    volatile uint32_t RESERVED_8;
    volatile uint32_t GPLEN0;
    volatile uint32_t GPLEN1;
    volatile uint32_t RESERVED_9;
    volatile uint32_t GPAREN0;
    volatile uint32_t GPAREN1;
    volatile uint32_t RESERVED_10;
    volatile uint32_t GPAFEN0;
    volatile uint32_t GPAFEN1;
    volatile uint32_t RESERVED_11;
    volatile uint32_t GPPUD;
    volatile uint32_t GPPUDCLK0;
    volatile uint32_t GPPUDCLK1;
    volatile uint32_t RESERVED_12;
    volatile uint32_t TEST;
} GPIO_REGISTERS;

typedef struct CLOCK_MANAGER_CTL_REGISTERS{
    volatile uint32_t GP0CTL;
    volatile uint32_t GP0DIV;
    volatile uint32_t GP1CTL;
    volatile uint32_t GP1DIV;
    volatile uint32_t GP2CTL;
    volatile uint32_t GP2DIV;
} CLOCK_MANAGER_CTL_REGISTERS;

typedef struct PWM_CLOCK_MANAGER_CTL_REGISTERS{
    volatile uint32_t GP0CTL;
    volatile uint32_t GP0DIV;
} PWM_CLOCK_MANAGER_CTL_REGISTERS;


void assert(int pin);
void assert2(int index_reg, int pin);
void deassert2(int index_reg, int pin);
void deassert(int pin);
void assert_mask(uint32_t reg_index, uint32_t mask);
void deassert_mask(uint32_t reg_index, uint32_t mask);

void gpio_enable_high_detect(uint32_t pin);
void gpio_disable_high_detect(uint32_t pin);

void gpio_enable_low_detect(uint32_t pin);
void gpio_disable_low_detect(uint32_t pin);

void gpio_enable_falling_detect(uint32_t pin);
void gpio_disable_falling_detect(uint32_t pin);

void gpio_enable_rising_detect(uint32_t pin);
void gpio_disable_rising_detect(uint32_t pin);

uint32_t gpio_event_detect(uint32_t pin);
reg64_t gpio_event_status_register();
void gpio_clear_event_detect(uint32_t pin);
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
     * @brief gpio_pin_mode changes the pin mode
     * @param pin the pin to be changed
     * @param mode the new mode to be set :
     *             INPUT or OUTPUT
     */
    void gpio_pin_mode(int pin, PINMODE mode);

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
         * @param group the SEL_REG on which the pins
         * should be set.
         */
        void gpio_set_pins(int mask, int group);
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
        void gpio_clear_pins(int mask, int group);

        void gpio_write_pin(int pin, int level);
    /**
     * @brief gpio_read_pin_level This function read the GPIO PIN level
     * @param pin The PIN from which we want to read the level
     * @return return the GPIO PIN Level
     * @return -1 if the pin number is not valid.
     */
    int gpio_read_pin_level(int pin);
    int gpio_read_pin(int pin);
    reg64_t gpio_get_pin_level_register();

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
    void gpio_pudclock(int pin, int value);
    void gpio_set_pudclock(int reg_index, uint32_t mask, int pud_type);

    void gpio_reset_pin_setting(int pin);


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
    uint32_t *gpio_get_base_ptr();

    void gpio_cm_set_password();
    void gpio_cm_set_mash(int mash);
    int gpio_cm_is_busy();
    void gpio_cm_kill();
    void gpio_cm_enable(int enable);
    void gpio_cm_set_src(int src);
    void gpio_cm_set_divi(int divi);
    void gpio_cm_set_divf(int divf);
    void gpio_cm_init();


    uint32_t gpio_cm_read_pwd(uint32_t reg);

    uint32_t gpio_cm_read_mash(uint32_t reg);

    uint32_t gpio_cm_read_busy(uint32_t reg);

    uint32_t gpio_cm_read_enab(uint32_t reg);

    uint32_t gpio_cm_read_src(uint32_t reg);

    uint32_t gpio_cm_read_divi(uint32_t reg);

    uint32_t gpio_cm_read_divf(uint32_t reg);
    void gpio_cm_info(char *str, uint32_t reg1, uint32_t reg2);
    //void pwm_cm_info();

    /*
    * convert a number to bits
    */
    unsigned to_bits(uint32_t n);

    void gpio_debug();
    GPIO_REGISTERS *gpio_get_gpio_registers();
    CLOCK_MANAGER_CTL_REGISTERS *gpio_get_cm_registers();
    PWM_CLOCK_MANAGER_CTL_REGISTERS *gpio_get_pwm_cm_registers();

#endif // GPIO_H