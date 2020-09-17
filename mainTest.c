#include <stdio.h>

#include "clock.h"
#include "uart.h"
#include "gpio.h"


int main() {
    
    //setup
    clock_setup();
    uart_setup();

    fprintf(stderr,"Test message on DEBUG uart");

    
    // we setup a GPIO as an output to make the led blink on the board (PA5)
    
    _gpio_setup_pin(RCC_GPIOA, GPIOA, GPIO5, GPIO_MODE_OUTPUT);
    //main loop
    while(1){
        //toggle changes the state of the output specified
        gpio_toggle(GPIOA, GPIO5);
        //now we wait (in milliseconds !)
        delay_ms(100);

    }
}



