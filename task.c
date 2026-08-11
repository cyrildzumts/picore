#include <task.h>
#include <gpio.h>
#include <timer.h>


void task1(){
    while(1){
        core_blink(PIN_37, 1);
        printf("task 1");
    }

}

void task2(){
    while(1){
        core_blink(PIN_33,2);
        printf("task 2");
    }

}

void task3(){
    while(1){
        core_blink(PIN_35,3);
        printf("task 3");
    }
}
