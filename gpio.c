#include "gpio.h"
#include <stdlib.h>

/*******************************************************
 * GPIO INTERFACE
 * ****************************************************/
volatile static  uint32_t* gpio_ptr = (uint32_t*)GPIO_BASE;
volatile static GPIO_REGISTERS* gpio_registers = (uint32_t*)GPIO_BASE;
volatile static CLOCK_MANAGER_CTL_REGISTERS* gpio_cm_registers = (uint32_t*)CLOCK_MANAGER_CTL_BASE;
volatile static PWM_CLOCK_MANAGER_CTL_REGISTERS* pwm_cm_reg = (PWM_CLOCK_MANAGER_CTL_REGISTERS*)PWM_CLOCK_BASE;

GPIO_REGISTERS *gpio_get_gpio_registers(){
  return gpio_registers;
}

CLOCK_MANAGER_CTL_REGISTERS *gpio_get_cm_registers(){
  return gpio_cm_registers;
}

PWM_CLOCK_MANAGER_CTL_REGISTERS *gpio_get_pwm_cm_registers(){
  return pwm_cm_reg;
}

void gpio_cm_init(){
  printf("initializing Clock Manager\n");
  gpio_reset_pin_setting(PIN_7);
  gpio_pin_mode(PIN_7, OUTPUT);
  gpio_set_pud(PIN_7,GPIO_PUD_UP);
  gpio_alt_func_pin(PIN_7, GPIO_PIN_ALT0);
  u_int32_t REG_GP0CTL = gpio_cm_registers->GP0CTL;
  gpio_cm_registers->GP0DIV |= CM_PASSWORD | (CM_DEFAULT_DIVI << 12);
  REG_GP0CTL = (REG_GP0CTL & ~(CM_MASK_MASH)) | CM_PASSWORD;
  delayN(10);
  gpio_cm_registers->GP0CTL = (REG_GP0CTL & ~(CM_MASK_MASH)) | CM_PASSWORD;
  delayN(10);
  gpio_cm_registers->GP0CTL |= CM_PASSWORD | CM_DEFAULT_SRC;
  delayN(10);
  gpio_cm_registers->GP0CTL |= CM_PASSWORD | CM_ENAB;
  delayN(10);
  printf("Clock Manager initialzed\n");
  gpio_cm_info("********** Clock Manager Info **********",gpio_cm_registers->GP0CTL, gpio_cm_registers->GP0DIV);
}

void gpio_pwm_cm_init(){
  printf("initializing PWM Clock Manager\n");
  gpio_set_pud(PIN_12, GPIO_PUD_DOWN);
  uint32_t content = gpio_get_gpio_registers()->GPFSEL1;
  content |= (GPIO_PIN_ALT5 << 24);
  gpio_get_gpio_registers()->GPFSEL1 = content;
  gpio_set_pud(PIN_12,GPIO_PUD_OFF);
  
  u_int32_t REG_GP0CTL = pwm_cm_reg->GP0CTL;
  pwm_cm_reg->GP0DIV |= CM_PASSWORD | (CM_DEFAULT_DIVI << 12);
  REG_GP0CTL = (REG_GP0CTL & ~(CM_MASK_MASH)) | CM_PASSWORD;
  delayN(100);
  pwm_cm_reg->GP0CTL = REG_GP0CTL;
  delayN(100);
  pwm_cm_reg->GP0CTL |= CM_PASSWORD | CM_DEFAULT_SRC;
  delayN(100);
  pwm_cm_reg->GP0CTL |= CM_PASSWORD | CM_ENAB;
  delayN(100);
  printf("PWM Clock Manager initialzed\n");
  gpio_cm_info("********** PWM Clock Manager Info **********\n",pwm_cm_reg->GP0CTL, pwm_cm_reg->GP0DIV);

}

uint32_t get_gpio_cm_gp0ctl(){
  return gpio_cm_registers->GP0CTL;
}

uint32_t get_gpio_cm_gp0divi(){
  return gpio_cm_registers->GP0DIV;
}

void gpio_cm_set_password(){
  gpio_cm_registers->GP0CTL = CM_PASSWORD;
  delayN(10);
  
}
void gpio_cm_set_mash(int mash){

}
int gpio_cm_is_busy(){
  return gpio_cm_registers -> GP0CTL & CM_BUSY;
}
void gpio_cm_kill(){

}
void gpio_cm_enable(int enable){
  gpio_cm_registers->GP0CTL |= CM_ENAB;
  delayN(10);
}
void gpio_cm_set_src(int src){
  gpio_cm_registers->GP0CTL = CM_DEFAULT_SRC;
  delayN(10);

}
void gpio_cm_set_divi(int divi){
  gpio_cm_registers->GP0DIV = CM_PASSWORD | CM_DEFAULT_DIVI;

}
void gpio_cm_set_divf(int divf){

}

uint32_t gpio_cm_read_pwd(uint32_t reg){
    return (reg & CM_MASK_PASSWORD) >> 24;
}

uint32_t gpio_cm_read_mash(uint32_t reg){
    return (reg & CM_MASK_MASH) >> 9;
}

uint32_t gpio_cm_read_busy(uint32_t reg){
    return (reg & CM_MASK_BUSY) >> 7;
}

uint32_t gpio_cm_read_enab(uint32_t reg){
    return (reg & CM_MASK_ENAB) >> 4;
}

uint32_t gpio_cm_read_src(uint32_t reg){
    return (reg & CM_MASK_SRC);
}

uint32_t gpio_cm_read_divi(uint32_t reg){
    return (reg & CM_MASK_DIVI) >> 12;
}

uint32_t gpio_cm_read_divf(uint32_t reg){
    return (reg & CM_MASK_DIVF);
}

void gpio_cm_info( char *str, uint32_t reg1, uint32_t reg2){
  
  uint32_t value = gpio_cm_read_pwd(reg1);
  printf("%s\n",str);
  printf("Clock Manager PWD : 0x%08X\n", value);
  value = gpio_cm_read_mash(reg1);
  printf("Clock Manager Mash : 0x%08X\n", value);
  value = gpio_cm_read_busy(reg1);
  printf("Clock Manager Busy : 0x%08X\n", value);
  value = gpio_cm_read_enab(reg1);
  printf("Clock Manager Enabled : 0x%08X\n", value);
  value = gpio_cm_read_src(reg1);
  printf("Clock Manager SRC : 0x%08X\n", value);
  value = gpio_cm_read_divi(reg2);
  printf("Clock Manager DIVI : 0x%08X\n", value);
  value = gpio_cm_read_divf(reg2);
  printf("Clock Manager DIVF : 0x%08X\n", value);
  //value = gpio_get_cm_registers()->GP0CTL;
  printf("Clock Manager GP0CTL : 0x%08X\n", reg1);
}

/*
void pwm_cm_info(){
  uint32_t value = gpio_cm_read_pwd();
  printf("********** PWM Clock Manager Info **********\n");
  printf("Clock Manager PWD : 0x%08X\n", value);
  value = gpio_cm_read_mash();
  printf("Clock Manager Mash : 0x%08X\n", value);
  value = gpio_cm_read_busy();
  printf("Clock Manager Busy : 0x%08X\n", value);
  value = gpio_cm_read_enab();
  printf("Clock Manager Enabled : 0x%08X\n", value);
  value = gpio_cm_read_src();
  printf("Clock Manager SRC : 0x%08X\n", value);
  value = gpio_cm_read_divi();
  printf("Clock Manager DIVI : 0x%08X\n", value);
  value = gpio_cm_read_divf();
  printf("Clock Manager DIVF : 0x%08X\n", value);
  value = gpio_get_cm_registers()->GP0CTL;
  printf("Clock Manager GP0CTL : 0x%08X\n", value);
}*/


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
    if (pin < 54)
    {
        int sel_reg = pin / 10;
        int shift = (pin % 10) * 3;
        
        gpio_ptr[sel_reg] &= ~(0x07 << shift);
        gpio_ptr[sel_reg] |= ((mode & 0x07) << shift);
    }
}

void gpio_set_pin(int pin)
{
  if(pin < 54)
  {
      if((pin / 32) == 0)
        {
          gpio_registers->GPSET0 |= (1 << pin);
        }
      else
        {
          gpio_registers->GPSET1 |= (1 << pin % 32);
        }
  }
}
void gpio_set_pins(int mask, int group)
{
  if(group == 0){
    gpio_registers->GPSET0 |= mask;
  }else if(group == 1){
    gpio_registers->GPSET1 |= mask;
  }
}

void gpio_clear_pin(int pin)
{
  if(pin > 53)
    return;
  if((pin / 32) == 0){
        gpio_registers->GPCLR0 = (1<< pin);
  }
  else{
      gpio_registers->GPCLR1 = (1<< pin % 32);
  }
}

void gpio_write_pin(int pin, int level)
{
    if( pin < 54)
    {
        if(level == LOW)
        {
            if((pin / 32) == 0){
                  //gpio_ptr[GPIO_GPCLR0] = (1 << pin);
                  gpio_registers->GPCLR0 = (1<< pin);
            }
            else{
                //gpio_ptr[GPIO_GPCLR1] = (1 << (pin % 32) );
                gpio_registers->GPCLR1 = (1<< pin % 32);
            }
        }
        else if (level == HIGH)
        {
            if((pin / 32) == 0)
              {
                //gpio_ptr[GPIO_GPSET0] = (1 << pin);
                gpio_registers->GPSET0 |= (1 << pin);
              }
            else
              {
                //gpio_ptr[GPIO_GPSET1]= (1 << (pin % 32) );
                gpio_registers->GPSET1 |= (1 << pin % 32);
              }
        }
    }
}

void gpio_clear_pins(int mask, int group)
{
  if(group == 0){
    gpio_registers->GPCLR0 |= mask;
  }else if(group == 1){
    gpio_registers->GPCLR1 |= mask;
  }
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
    //gpio_ptr[GPIO_GPPUD] = pud;
    gpio_registers->GPPUD = pud;
    delayN(150);
}

void gpio_set_pud(int pin, int pud_type)
{
    gpio_pud(pud_type);
    delayN(150);
    gpio_pudclock(pin, 1);
    delayN(150);
    gpio_pud(GPIO_PUD_OFF);
    delayN(150);
    gpio_pudclock(pin, 1);
    delayN(150);
}

void gpio_pudclock(int pin, int value)
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



void deassert(int pin)
{
    if(pin > 53)
      return;
    if(pin < 32){
      gpio_registers->GPCLR0 |= (0x01 << pin);
    }
    else{
      gpio_registers->GPCLR1 |= (0x01 << (pin % 32));
    }
}

void assert(int pin)
{
    if(pin > 53)
      return;
    if(pin < 32){
      gpio_registers->GPSET0 |= (0x01 << pin);
    }
    else{
      gpio_registers->GPSET1 |= (0x01 << (pin % 32));
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

reg64_t gpio_event_status_register()
{
    reg64_t reg;
    reg.low = gpio_registers->GPEDS0;
    reg.high = gpio_registers->GPEDS1;
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

reg64_t gpio_get_pin_level_register(){
   reg64_t reg;
   reg.low = gpio_registers->GPLEV0;
   reg.high = gpio_registers->GPLEV1;
   return reg;
}

void gpio_reset_pin_setting(int pin){
  gpio_pin_mode(pin, INPUT);
  gpio_set_pud(pin, GPIO_PUD_OFF);
}

void gpio_debug()
{
    reg64_t reg = gpio_get_pin_level_register();
    printf("\n-------------------------------------\n");
    reg = gpio_get_pin_level_register();
    printf("GPIO PIN LEVEL STATUS : \n"
           "LOW : 0x%0X\n"
           "HIGH: 0x%0X\n", reg.low, reg.high);
    
    print_bits(reg.low);
    printf("\n-------------------------------------\n");
    print_bits(reg.high);

    printf("\n-------------------------------------\n");

}

void assert_mask(uint32_t reg_index, uint32_t mask)
{
    gpio_ptr[reg_index] = mask;
}

void deassert_mask(uint32_t reg_index, uint32_t mask)
{
    gpio_ptr[reg_index] = mask;
}


unsigned to_bits(uint32_t n){
  char buffer [65];
  return atoi(itoa(n, buffer, 2));
}
