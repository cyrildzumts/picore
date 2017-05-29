#include "gpio.h"

/*******************************************************
 * GPIO INTERFACE
 * ****************************************************/
volatile uint32_t* gpio_ptr = (uint32_t*)GPIO_BASE;
/*
 * By Default All pins are set as INPUT */
void gpio_set_pin_IN( int pin)
{
  if(pin > 53)
    {
      // displays an unvalid error message
      return;
    }

  int sel_reg = pin / 10;
  gpio_ptr[sel_reg] &= ~(7 << ((pin % 10) * 3));
}


void gpio_set_pin_OUT( int pin)
{
  if(pin > 53)
    {
      // displays an unvalid error message
      return;
    }

  int sel_reg = pin / 10;
  gpio_ptr[sel_reg] |= (1 << ((pin % 10) * 3));
}



void gpio_pin_setup(int mask, int sel_reg)
{
   gpio_ptr[sel_reg] = mask;
}

void gpio_alt_func_pin( int pin, int mode)
{
    int sel_reg;
    if(pin > 53)
      {
        // displays an unvalid error message
        return;
      }
    sel_reg = pin / 10;
    gpio_ptr[sel_reg] |= (mode << ((pin % 10) * 3));
}

void gpio_set_pin(int pin)
{
  if(pin > 53)
    return;
  if((pin / 32) == 0)
    {
      gpio_ptr[GPIO_GPSET0] = (1 << pin);
    }
  else
    {
      gpio_ptr[GPIO_GPSET1]= (1 << (pin % 32) );
    }
}

void gpio_set_pins(int mask, int set_reg)
{
    gpio_ptr[set_reg] = mask;
}

void gpio_clear_pin(int pin)
{
  if(pin > 53)
    return;
  if((pin / 32) == 0)
    {
        gpio_ptr[GPIO_GPCLR0] = (1 << pin);
    }
  else
    {
      gpio_ptr[GPIO_GPCLR1] = (1 << (pin % 32) );
    }

}


void gpio_clear_pins(int mask, int clr_reg)
{
   gpio_ptr[clr_reg] = mask;
}

int gpio_is_pin_set(int pin)
{
    if(pin < 54)
        return (gpio_ptr[GPIO_GPLEV0 + pin/32] & (1 << pin % 32) )? 1:0;
    return -1;
}


int gpio_read_pin_level(int pin)
{
    if(pin < 54)
        return  (gpio_ptr[GPIO_GPLEV0 + pin/32] &  (1 << pin % 32)) ? 1: 0 ;
    return -1;
}


void gpio_pud(int pud)
{
    gpio_ptr[GPIO_GPPUD] = pud;
}

void gpio_set_pud(int pin, int pud_type)
{
    gpio_pud(pud_type);
    delayN(10);
    gpio_pudclock(pin, 1);
    delayN(10);
    gpio_pud(GPIO_PUD_OFF);
    gpio_pudclock(pin, 0);
}

void gpio_pudclock(int pin, uint8_t value)
{
    if(pin < 54)
    {
        gpio_ptr[GPIO_GPPUDCLK0 + pin/32] = value << (pin % 32);
    }
}

void core_set_bits(volatile int *addr, int value, int mask)
{
    uint32_t v = *addr;
    v = (v & ~mask) | (value & mask);
    *addr = v;
}



void deassert(uint8_t pin)
{
    if(pin > 53)
      return;
    if((pin / 32) == 0)
      {
          gpio_ptr[GPIO_GPCLR0] = (1 << pin);
      }
    else
      {
        gpio_ptr[GPIO_GPCLR1] = (1 << (pin % 32) );
      }
}

void assert(uint8_t pin)
{
    if(pin > 53)
      return;
    if((pin / 32) == 0)
      {
        gpio_ptr[GPIO_GPSET0] = (1 << pin);
      }
    else
      {
        gpio_ptr[GPIO_GPSET1]= (1 << (pin % 32) );
      }
}

uint32_t *gpio_get_base_ptr()
{
    return gpio_ptr;
}
