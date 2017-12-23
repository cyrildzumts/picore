#include "timer.h"
/*******************************************************
 * BASIC TIMER INTERFACE
 * ****************************************************/
static FreeRunningTimer *timer = (FreeRunningTimer*)(FREE_RUNNING_TIMER_BASE);
static ArmTimer_t *armtimer = (ArmTimer_t *)(ARMTIMER_BASE);
static virtual_timer _timer[MAX_TIMERS];

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
    armtimer->Load = (TIMER_PERIODE_1MS * 5);
    armtimer->Control = ARMTIMER_CTRL_23BIT |
                        ARMTIMER_CTRL_ENABLE |
                        ARMTIMER_CTRL_INT_ENABLE |
                        ARMTIMER_CTRL_PRESCALE_256;
}

void timer_init()
{
    armtimer->Load = TIMER_PERIODE_1S;
    armtimer->Control = ARMTIMER_CTRL_23BIT |
                        ARMTIMER_CTRL_ENABLE |
                        ARMTIMER_CTRL_INT_ENABLE |
                        ARMTIMER_CTRL_PRESCALE_256;
}

int timer_create(int timeout, int periodic, void (*callback)(void *), void *arg)
{
    int handle = -1;
    int i = 0;
    for(i = 0; i < MAX_TIMERS; i++)
    {
        if(_timer[i].action== NULL)
            break;
    }
    if(i < MAX_TIMERS)
    {
        if(periodic != 0)
        {
             _timer[i].periodic = (timeout < 100) ? 1 : (timeout / TIMER_RESOLUTION_MS);
        }
        else
        {
            _timer[i].periodic = 0;
        }
        _timer[i].action = callback;
        _timer[i].arg = arg;
        _timer[i].expity = _timer_ticks + _timer[i].periodic;
        handle = i;
    }

    return handle;
}

int timer_delete(int handle)
{
    int flag = -1;
    if((handle < MAX_TIMERS) && (handle >= 0))
    {
        _timer[handle].action = NULL;
        flag = 0;
    }
    return flag;
}
