#include "clock.h"
#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "pwm.h"

void test_led(uint32_t delay);
void test_pwm();
void test_uart();

int main() {
    //setup
    clock_setup();
    
	//Choose one and only one test
	//test_led(100);
	test_pwm();
	//test_uart();
}

void test_led(uint32_t delay){
	led_setup();

	while(1){
		led_blink();
		delay_ms(delay);
	}
}

void test_pwm(){
	pwm_setup();

	uint32_t pw = 10;

    while (1) {
        pw = (pw+100)%20000;
        pwm_set_pulse_width(PWM_TIM, PWM_OC_ID, pw);
        delay_ms(100);
    }
}
void test_uart(){
	uart_setup();

	fprintf(stderr,"Test message on DEBUG uart");

	while(1);
}


