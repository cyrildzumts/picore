#include <task.h>
#include <gpio.h>
#include <timer.h>
void task1(){
    gpio_pin_mode(PIN_37, OUTPUT);
    while(1){
        core_blink(PIN_37, 1);
        //printf("%s\n",__PRETTY_FUNCTION__);
    }

}

void task2(){
    gpio_pin_mode(PIN_33, OUTPUT);
    while(1){
        core_blink(PIN_33,2);
        delayN(WAIT_150_MS*4);
        //printf("%s\n",__PRETTY_FUNCTION__);
    }

}

void task3(){
    gpio_pin_mode(PIN_35, OUTPUT);
    while(1){
        core_blink(PIN_35,3);
        //printf("%s\n",__PRETTY_FUNCTION__);
    }
}
