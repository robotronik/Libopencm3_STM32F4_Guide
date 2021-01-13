/**
 * @file
 * 
 * This file is part of STM32F401 Guide
 * 
 * @brief This implements an ISR on a timer
 * 
 * @date  01/2021  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano
*/

#pragma once

#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/nvic.h>

#include "timer.h"
#include "uart.h" //only needed to print on uart in the routine

#define TIM_PRESCALE	    (84)
#define TIM_PERIOD	    	(500000)

#define TIM_RCC			    RCC_TIM4
#define TIM				    TIM4
#define TIM_OC_ID           TIM_OC1

#define TIM_OC_MODE         TIM_OCM_FROZEN
#define TIM_NVIC            NVIC_TIM4_IRQ
#define TIM_DIER_CCIE       TIM_DIER_CC1IE
#define TIM_SR_CCIF         TIM_SR_CC1IF


/**
 * @brief This function setup the timer and the OC for an interrupt with given period (using all definitions)
 */
void timer_setup_interrupt();

/**
 * @brief This function is given by libopencm3 as weak and contain the routine to be executed by a timer 4 interrupt event
 */
void tim4_isr();
