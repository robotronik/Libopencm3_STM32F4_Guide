/**
 * @file
 * 
 * This file is part of cdfr2020BaseRoulanteRework
 * 
 * @brief This implements the functions required setup a timer and its output channel  
 * 
 * @date  06/2020  
 * 
 * Licence :
 * 
 * Robotronik Phelma
 * @author NPXav Benano Trukbidule
*/

#pragma once

#include <stdint.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

/**
 * @brief This function setup an internal timer with the given parameters
 * 
 * @param rcc_clken    reset and clock control enable for the timer (clock tree)
 * @param timer_peripheral    timer selected
 * @param prescaler    the input frequency of the timer (sys_clk) is divided by this factor
 * @param period    period of the timer in us
 */
void _timer_setup(enum rcc_periph_clken rcc_clken, uint32_t timer_peripheral, uint32_t prescaler, uint32_t period);

/**
 * @brief This function configure the output comparator of a channel for the timer specified
 * 
 * @param timer_peripheral    selected timer
 * @param oc_id    selected channel of the output comparator
 * @param oc_mode    different mode used for the timer
 * @param oc_value    initial value of the duty cycle
 */
void _timer_setup_output_c(uint32_t timer_peripheral, enum tim_oc_id oc_id, enum tim_oc_mode oc_mode, uint32_t oc_value);

/**
 * @brief This function starts the given timer
 * 
 * @param timer_peripheral    selected timer
 */
void _timer_start(uint32_t timer_peripheral);
