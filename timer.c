#include "timer.h"
/*******************************************************
 * BASIC TIMER INTERFACE
 * ****************************************************/
static FreeRunningTimer *timer = (FreeRunningTimer*)(FREE_RUNNING_TIMER_BASE);
static ArmTimer_t *armtimer = (ArmTimer_t *)(ARMTIMER_BASE);
void delay()
{
    volatile int ts = timer->CLO;
    while((timer->CLO - ts) < 1)
    {
        ;
    }
}

void delayN(int N)
{
    volatile int ts = timer->CLO;
    while((timer->CLO - ts) < N)
    {
        ;
    }
}

void core_blink(int pin, int n)
{
    volatile int i = 0;
    for(i = 0; i < n; i++)
    {
        assert(pin);
        delayN(WAIT_200_MS);
        deassert(pin);
        delayN(WAIT_200_MS);
    }
}

ArmTimer_t *getArmTimer()
{
    return armtimer;
}

void ArmTimeInit()
{
    armtimer->Load = 0x400;
    armtimer->Control = ARMTIMER_CTRL_23BIT |
                        ARMTIMER_CTRL_ENABLE |
                        ARMTIMER_CTRL_INT_ENABLE |
                        ARMTIMER_CTRL_PRESCALE_256;
}
