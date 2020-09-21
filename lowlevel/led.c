#include "led.h"

void led_setup(){
	_gpio_setup_pin(LED_GPIO_RCC, LED_PORT, LED_PIN, GPIO_MODE_OUTPUT);
}

void led_blink(){
	gpio_toggle(LED_PORT, LED_PIN);
}
