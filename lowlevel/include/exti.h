/**
 * @file
 *
 * This file is part of STM32F401REGuide
 *
 * @brief This function implements external interrupts coming from the blue
 * button PC13
 *
 * @date  09/2020
 *
 * Licence :
 *
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/
#pragma once

#include <stdint.h>
#include <stdio.h>

#include "libopencm3/stm32/exti.h"
#include "libopencm3/cm3/nvic.h"

#include "gpio.h"
#include "led.h"

#define BUTTON_RCC     RCC_GPIOC
#define BUTTON_PORT    GPIOC
#define BUTTON_PIN     GPIO13
#define BUTTON_NVIC_INTERRUPT_NUMBER   	NVIC_EXTI15_10_IRQ
#define BUTTON_EXTI    EXTI13

#define BUTTON_PRIORITY 1



/**
 * @brief This function initializes the exti interrupt and nvic
 * interrupts will be received from gpio_port with the pin matching the number
 * of the exti
 *
 * @param exti the external interrupt peripheral linked to the gpio pin (number
 * must match !)
 * @param gpio_port the port on which the limit switch will be plugged
 * @param interrupt_number the interrupt number in the NVIC table
 * @param trig the type of event that will trigger the interrupt
 * (rising,falling,both)
 */
void _limit_switch_init(uint32_t exti,uint32_t gpio_port,uint8_t interrupt_number,enum exti_trigger_type trig);

/**
 * @brief Initialize the GPIO and interrupts for the blue button on the nucleo
 * F401RE (ie PC13)
 */
void button_switch_init();
