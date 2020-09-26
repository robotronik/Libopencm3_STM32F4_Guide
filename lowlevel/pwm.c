#include "pwm.h"

void pwm_setup() {
    _timer_setup(PWM_TIM_RCC, PWM_TIM, PWM_PRESCALE, PWM_PERIOD);
    _gpio_setup_pin_af(PWM_GPIO_RCC_EN, PWM_PORT_EN, PWM_PIN_EN, PWM_AF);
    _timer_setup_output_c(PWM_TIM, PWM_OC_ID, PWM_OC_MODE, 0);
    _timer_start(PWM_TIM);
}

void pwm_set_pulse_width(uint32_t pulse_width) {
    timer_set_oc_value(PWM_TIM, PWM_OC_ID, pulse_width);
}
