/**
 * @file
 * 
 * This file is part of STM32F401REGuide
 * 
 * @brief PWM
 * 
 * @date  2020/09/18 
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav roborenar
*/

#pragma once
#include <libopencm3/stm32/timer.h>

#include "timer.h"
#include "gpio.h"

#define PWM_PRESCALE (84) // 84 MHz clock
#define PWM_PERIOD (20000)

#define PWM_TIM     TIM1
#define PWM_TIM_RCC     RCC_TIM1
#define PWM_GPIO_RCC_EN     RCC_GPIOA
#define PWM_PORT_EN     GPIOA
#define PWM_PIN_EN      GPIO10

#define PWM_AF      GPIO_AF1
#define PWM_OC_ID   TIM_OC3
#define PWM_OC_MODE     TIM_OCM_PWM1

/**
 * @brief Setup PWM on pin
*/
void pwm_setup();

/**
 * @brief Setup PWM pulse width
*/
void pwm_set_pulse_width(uint32_t timer_peripheral, enum tim_oc_id oc_id, uint32_t pulse_width);
