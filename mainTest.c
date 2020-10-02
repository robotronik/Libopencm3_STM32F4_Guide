#include "clock.h"
#include "uart.h"
#include "gpio.h"
#include "led.h"
#include "pwm.h"
#include "exti.h"

void test_led(uint32_t delay);
void test_pwm();
void test_uart();
void test_exti();

int main() {
    //setup
    clock_setup();
  //exti_setup();
	//Choose one and only one test
	//test_led(100);
	test_pwm();
	//test_uart();
	//test_exti();
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
        pwm_set_pulse_width(pw);
        delay_ms(100);
    }
}

void test_uart(){
	uart_setup();

	fprintf(stderr,"Test message on DEBUG uart\n");

	while(1);
}

void test_exti(){
    uart_setup();
    led_setup();
    gpio_toggle(LED_PORT, LED_PIN);
    button_switch_init();

    while(1){
        if(!gpio_get(BUTTON_PORT,BUTTON_PIN)){
            fprintf(stderr,"j'appuie sur le bouton\n");
        }
        delay_ms(500);
    }
}
