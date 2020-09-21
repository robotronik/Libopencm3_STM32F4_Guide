/**
 * @file
 * 
 * This file is part of STM32F401 Guide
 * 
 * @brief This implements the onboard LED GPIO 
 * 
 * @date  09/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav
*/

#pragma once

#include "gpio.h"

/**
 * @defgroup LED led
 * @{
 * @brief Definition for the LED
 *
 * We use the onboard LED PA5
 */
#define LED_GPIO_RCC			RCC_GPIOA
#define LED_PORT				GPIOA
#define LED_PIN					GPIO5
/** @} */

/**
 * @brief Setup GPIO on pin
*/
void led_setup();

/**
 * @brief toggle LED state
*/
void led_blink();
