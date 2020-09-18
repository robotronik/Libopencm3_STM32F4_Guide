#include <stdio.h>

#include "clock.h"
#include "uart.h"
#include "gpio.h"
#include "pwm.h"


int main() {
    
    //setup
    clock_setup();
    uart_setup();
    pwm_setup();

    pwm_set_pulse_width(PWM_TIM, PWM_OC_ID, 10000);

    fprintf(stderr,"Test message on DEBUG uart");

    uint32_t pw = 10;

    while (1) {
        pw = (pw+100)%20000;
        pwm_set_pulse_width(PWM_TIM, PWM_OC_ID, pw);
        delay_ms(100);
    }
    
    // we setup a GPIO as an output to make the led blink on the board (PA5)
    
    // _gpio_setup_pin(RCC_GPIOA, GPIOA, GPIO5, GPIO_MODE_OUTPUT);
    // //main loop
    // while(1){
    //     //toggle changes the state of the output specified
    //     gpio_toggle(GPIOA, GPIO5);
    //     //now we wait (in milliseconds !)
    //     delay_ms(100);

    // }
}



