#ifndef TIMER_H
#define TIMER_H

#include "gpio.h"

/*******************************************************
 * BASIC TIMER INTERFACE
 * ****************************************************/

// Local Timer Interface

// System Timer Adress
#define FREE_RUNNING_TIMER_BASE  (PERI_BASE + 0x00003000)

// ARM Timer :

#define ARMTIMER_BASE               ( PERI_BASE + 0xB400 )

#define ARMTIMER_CTRL_23BIT         ( 1 << 1 )

#define ARMTIMER_CTRL_PRESCALE_1    ( 0 << 2 )
#define ARMTIMER_CTRL_PRESCALE_16   ( 1 << 2 )
#define ARMTIMER_CTRL_PRESCALE_256  ( 2 << 2 )

#define ARMTIMER_CTRL_INT_ENABLE    ( 1 << 5 )
#define ARMTIMER_CTRL_INT_DISABLE   ( 0 << 5 )

/** @brief 0 : Timer disabled - 1 : Timer enabled */
#define ARMTIMER_CTRL_ENABLE        ( 1 << 7 )
#define ARMTIMER_CTRL_DISABLE       ( 0 << 7 )

// Default Wait Times
#define WAIT_1_MICROSECOND 1
#define WAIT_1_MILLISECOND 1000
#define WAIT_1_SECOND 1000000
#define WAIT_150_MS         (150 * WAIT_1_MILLISECOND)
#define WAIT_200_MS 200000
#define WAIT_10_US          (10 * WAIT_1_MICROSECOND)

#define MAX_TIMERS                  10
#define TIMER_RESOLUTION_MS         1000
#define TIMER_PERIODE_1S            0x100000
#define TIMER_PERIODE_4S            0x3D0900
#define TIMER_PERIODE_1MS           0x400
typedef struct {

    /** The timer load register sets the time for the timer to count down.
        This value is loaded into the timer value register after the load
        register has been written or if the timer-value register has counted
        down to 0. */
    volatile uint32_t Load;

    /** This register holds the current timer value and is counted down when
        the counter is running. It is counted down each timer clock until the
        value 0 is reached. Then the value register is re-loaded from the
        timer load register and the interrupt pending bit is set. The timer
        count down speed is set by the timer pre-divide register. */
    volatile uint32_t Value;

    /** The standard SP804 timer control register consist of 8 bits but in the
        BCM implementation there are more control bits for the extra features.
        Control bits 0-7 are identical to the SP804 bits, albeit some
        functionality of the SP804 is not implemented. All new control bits
        start from bit 8 upwards. */
    volatile uint32_t Control;

    /** The timer IRQ clear register is write only. When writing this register
        the interrupt-pending bit is cleared. When reading this register it
        returns 0x544D5241 which is the ASCII reversed value for "ARMT". */
    volatile uint32_t IRQClear;

    /** The raw IRQ register is a read-only register. It shows the status of
        the interrupt pending bit. 0 : The interrupt pending bits is clear.
        1 : The interrupt pending bit is set.

        The interrupt pending bits is set each time the value register is
        counted down to zero. The interrupt pending bit can not by itself
        generates interrupts. Interrupts can only be generated if the
        interrupt enable bit is set. */
    volatile uint32_t RAWIRQ;

    /** The masked IRQ register is a read-only register. It shows the status
        of the interrupt signal. It is simply a logical AND of the interrupt
        pending bit and the interrupt enable bit. 0 : Interrupt line not
        asserted. 1 :Interrupt line is asserted, (the interrupt pending and
        the interrupt enable bit are set.)  */
    volatile uint32_t MaskedIRQ;

    /** This register is a copy of the timer load register. The difference is
        that a write to this register does not trigger an immediate reload of
        the timer value register. Instead the timer load register value is
        only accessed if the value register has finished counting down to
        zero. */
    volatile uint32_t Reload;

    /** The Pre-divider register is not present in the SP804. The pre-divider
        register is 10 bits wide and can be written or read from. This
        register has been added as the SP804 expects a 1MHz clock which we do
        not have. Instead the pre-divider takes the APB clock and divides it
        down according to:

        timer_clock = apb_clock/(pre_divider+1)

        The reset value of this register is 0x7D so gives a divide by 126. */
    volatile uint32_t PreDivider;

    /** The free running counter is not present in the SP804. The free running
        counter is a 32 bits wide read only register. The register is enabled
        by setting bit 9 of the Timer control register. The free running
        counter is incremented immediately after it is enabled. The timer can
        not be reset but when enabled, will always increment and roll-over.

        The free running counter is also running from the APB clock and has
        its own clock pre-divider controlled by bits 16-23 of the timer
        control register.

        This register will be halted too if bit 8 of the control register is
        set and the ARM is in Debug Halt mode. */
    volatile uint32_t FreeRunningCounter;

    } ArmTimer_t;

ArmTimer_t *getArmTimer(void);
void ArmTimeInit(void);



// SYSTEM FREE TIMER
typedef struct System_Timer{
    volatile uint32_t CS;  // System Timer Control /Status
    volatile uint32_t CLO; // System Timer Counter Lower 32 Bits
    volatile uint32_t CHI; // System Timer Counter Higher 32 Bits
    volatile uint32_t CO;  // System Timer Compare 0
    volatile uint32_t C1;  // System Timer Compare 1
    volatile uint32_t C2;  // System Timer Compare 2
    volatile uint32_t C3;  // System Timer Compare 3
} FreeRunningTimer;

typedef struct {
    int expity;
    int periodic;
    void (*action)(void *);
    void *arg;
}virtual_timer;
extern volatile int _timer_ticks;
/**
 * @brief _timer a list of MAX_TIMERS timers
 */
/**
 * @brief delay implement a basic delay function
 * This method uses the Local Freerunning Timer
 * By default it wait for 1us (Raspberry PI 2)
 */
void delay();
/**
 * @brief delayN applies a delay of N microseconds
 * @param N How long to wait, in microseconds
 */
void delayN(int N);

void core_blink(int pin,int n);

/**
 * @brief timer_init initialize the timer
 */
void timer_init();
/**
 * @brief timer_create
 * @param timeout the timer timeout in microseconds
 * @param periodic defines whether it is a periodic timer
 *         periodic > 0 --> Periodic Timer
 *         periodic <= 0 --> one shot timer.
 * @param arg buffer to pass information to the callback.
 * @return a handle to the timer.
 */
int timer_create(int timeout, int periodic, void(*callback)(void*), void *arg);
/**
 * @brief timer_delete a timer. the deleted timer can then be assigned
 * assigned to anyone interrested
 * @param handle the timer id to be deleted
 */
int timer_delete(int handle);

#endif // TIMER_H
