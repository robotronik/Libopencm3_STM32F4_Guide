/**
 * @file
 * 
 * This file is part of STM32F401 Guide
 * 
 * @brief This implements the setup of a gpio pin  
 * 
 * @date  07/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/

#pragma once

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

/**
 * @brief This function setup a pin for an alternate function
 * 
 * @param rcc_clken   reset clock control for the pin (usualy RCC_X with X 
 * the gpio_port)
 * @param gpio_port    port of the selected pin 
 * @param gpio_pin    number of the selected pin
 * @param gpio_altfun    identifier for the alternate function (usualy GPIO_AFX
 * with X the number for altfun)
 */
void _gpio_setup_pin_af(enum rcc_periph_clken rcc_clken, uint32_t gpio_port,
        uint16_t gpio_pin, uint8_t gpio_altfun);

/**
 * @brief This function setup a GPIO pin for standard input or output
 *
 * @param clken     the clock of the port to enable
 * @param port      the port to enable
 * @param pin       the pint to enable
 * @param mode      the mode of your GPIO (GPIO_MODE_INPUT,GPIO_MODE_OUTPUT)
 */
void _gpio_setup_pin(enum rcc_periph_clken rcc_clken, uint32_t port,uint16_t pin,
        uint8_t mode);
