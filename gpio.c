#include "gpio.h"

/*******************************************************
 * GPIO INTERFACE
 * ****************************************************/
static uint32_t* gpio_ptr = (uint32_t*)GPIO_BASE;
/*
 * By Default All pins are set as INPUT */
void gpio_set_pin_IN( int pin)
{
  if(pin <= 53)
    {
      int sel_reg = pin / 10;
      gpio_ptr[sel_reg] &= ~(7 << ((pin % 10) * 3));
    }


}


void gpio_set_pin_OUT( int pin)
{
  if(pin <= 53)
    {
      int sel_reg = pin / 10;
      gpio_ptr[sel_reg] |= (1 << ((pin % 10) * 3));
    }

}

void gpio_pin_mode(int pin, PINMODE mode)
{
    int sel_reg = pin / 10;
    if(pin <= 53)
    {
        if(mode == INPUT)
        {
            gpio_ptr[sel_reg] &= ~(7 << ((pin % 10) * 3));
        }
        else if(mode == OUTPUT)
        {
            gpio_ptr[sel_reg] |= (1 << ((pin % 10) * 3));
        }
    }
}

void gpio_pin_setup(int mask, int sel_reg)
{
   gpio_ptr[sel_reg] = mask;
}

void gpio_alt_func_pin( int pin, int mode)
{
    int sel_reg;
    if(pin < 54)
      {
        sel_reg = pin / 10;
        gpio_ptr[sel_reg] |= (mode << ((pin % 10) * 3));
      }

}

void gpio_set_pin(int pin)
{
  if(pin < 54)
  {
      if((pin / 32) == 0)
        {
          gpio_ptr[GPIO_GPSET0] |= (1 << pin);
        }
      else
        {
          gpio_ptr[GPIO_GPSET1]|= (1 << (pin % 32) );
        }
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

void gpio_write_pin(int pin, int level)
{
    if( pin < 54)
    {
        if(level == LOW)
        {
            if((pin / 32) == 0)
              {
                  gpio_ptr[GPIO_GPCLR0] = (1 << pin);
              }
            else
              {
                gpio_ptr[GPIO_GPCLR1] = (1 << (pin % 32) );
              }
        }
        else if (level == HIGH)
        {
            if((pin / 32) == 0)
              {
                gpio_ptr[GPIO_GPSET0] = (1 << pin);
              }
            else
              {
                gpio_ptr[GPIO_GPSET1]= (1 << (pin % 32) );
              }
        }
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


int gpio_read_pin(int pin)
{
    int ret = -1;
    if(pin < 54)
        ret = (gpio_ptr[GPIO_GPLEV0 + pin/32] &  (1 << pin % 32)) ? 1 : 0 ;

    return ret;
}

void gpio_pud(int pud)
{
    gpio_ptr[GPIO_GPPUD] = pud;
}

void gpio_set_pud(int pin, int pud_type)
{
    gpio_pud(pud_type);
    delayN(15);
    gpio_pudclock(pin, 1);
    delayN(15);
    gpio_pud(GPIO_PUD_OFF);
    gpio_pudclock(pin, 0);
}

void gpio_pudclock(int pin, uint8_t value)
{
    if(pin < 54)
    {
        gpio_ptr[GPIO_GPPUDCLK0 + pin/32] |= value << (pin % 32);
    }
}

void core_set_bits(volatile int *addr, int value, int mask)
{
    int v = *addr;
    v = (v & ~mask) | (value & mask);
    *addr = v;
}



void deassert(uint8_t pin)
{
    if(pin > 53)
      return;
    if((pin / 32) == 0)
      {
          gpio_ptr[GPIO_GPCLR0] |= (1 << pin);
      }
    else
      {
        gpio_ptr[GPIO_GPCLR1] |= (1 << (pin % 32) );
      }
}

void assert(uint8_t pin)
{
    if(pin > 53)
      return;
    if((pin / 32) == 0)
      {
        gpio_ptr[GPIO_GPSET0] |= (1 << pin);
      }
    else
      {
        gpio_ptr[GPIO_GPSET1] |= (1 << (pin % 32) );
      }
}

void deassert2(int index_reg, int pin)
{
    gpio_ptr[index_reg] = (1 << pin);
}

void assert2(int index_reg, int pin)
{
    gpio_ptr[index_reg] = (1 << pin);
}
uint32_t *gpio_get_base_ptr()
{
    return gpio_ptr;
}


void gpio_enable_high_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPHEN0] |= (1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPHEN1] |= (1 << (pin % 32) );
          }
    }
}

void gpio_disable_high_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPHEN0] &= ~(1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPHEN1] &= ~(1 << (pin % 32) );
          }
    }
}

void gpio_enable_falling_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPFEN0] |= (1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPFEN1] |= (1 << (pin % 32) );
          }
    }
}

void gpio_disable_falling_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPFEN0] &= ~(1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPFEN1] &= ~(1 << (pin % 32) );
          }
    }
}

void gpio_enable_rising_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPREN0] |= (1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPREN1] |= (1 << (pin % 32) );
          }
    }
}

void gpio_disable_rising_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPREN0] &= ~(1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPREN1] &= ~(1 << (pin % 32) );
          }
    }
}

uint32_t gpio_event_detect(uint32_t pin)
{
    uint32_t ret = 0;
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            ret = gpio_ptr[GPIO_GPEDS0] & (1 << pin);
          }
        else
          {
            ret = gpio_ptr[GPIO_GPEDS1] & (1 << (pin % 32) );
          }
    }
    return ret;
}

void gpio_clear_event_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPEDS0] |= (1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPEDS1] |= (1 << (pin % 32) );
          }
    }
}

void gpio_enable_low_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPLEN0] |= (1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPLEN1] |= (1 << (pin % 32) );
          }
    }
}

void gpio_disable_low_detect(uint32_t pin)
{
    if(pin < 54)
    {
        if((pin / 32) == 0)
          {
            gpio_ptr[GPIO_GPLEN0] &= ~(1 << pin);
          }
        else
          {
            gpio_ptr[GPIO_GPLEN1] &= ~(1 << (pin % 32) );
          }
    }
}

Event_Status_Reg gpio_event_status_register()
{
    Event_Status_Reg reg;
    reg.low = gpio_ptr[GPIO_GPEDS0];
    reg.high = gpio_ptr[GPIO_GPEDS1];
    return reg;
}

void gpio_set_pudclock(int reg_index, uint32_t mask, int pud_type)
{
    gpio_pud(pud_type);
    delayN(10);
    gpio_ptr[reg_index] = mask;
    delayN(10);
    gpio_pud(GPIO_PUD_OFF);
    gpio_ptr[reg_index] = 0;
}

Reg_64BIT_t gpio_get_pin_level_register()
{
   Reg_64BIT_t reg;
   reg.low = gpio_ptr[GPIO_GPLEV0];
   reg.high = gpio_ptr[GPIO_GPLEV1];
   return reg;
}

void gpio_debug()
{
    Reg_64BIT_t reg;
    printf("\n-------------------------------------\n");
    reg = gpio_get_pin_level_register();
    printf("GPIO PIN LEVEL STATUS : \n"
           "LOW : %X\n"
           "HIGH: %X\n", reg.low, reg.high);
    printf("\n-------------------------------------\n");

    /*
    reg.low = gpio_ptr[GPIO_GPFSEL0];
    reg.high = gpio_ptr[GPIO_GPFSEL1];
    printf("GPIO FUNCTIONSELECT STATUS : \n"
           "FSEL0 : %X\n"
           "FSEL1 : %X\n", reg.low, reg.high);
    printf("\n-------------------------------------\n");
    */

}

void assert_mask(uint32_t reg_index, uint32_t mask)
{
    gpio_ptr[reg_index] = mask;
}

void deassert_mask(uint32_t reg_index, uint32_t mask)
{
    gpio_ptr[reg_index] = mask;
}
