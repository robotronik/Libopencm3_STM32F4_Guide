#pragma once

#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>

#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/cm3/nvic.h>


/***************************************
simple uart configuration for debugging
***************************************/

// DEFINES FOR DEBUG_UART
#define DEBUG_RCC_USART RCC_USART2
#define DEBUG_USART USART2
#define DEBUG_UART_SPEED (9600) 

#define DEBUG_PORT_TX GPIOA
#define DEBUG_PORT_TX_RCC RCC_GPIOA
#define DEBUG_PIN_TX GPIO2 
#define DEBUG_AF_TX GPIO_AF7

#define DEBUG_PORT_RX GPIOA
#define DEBUG_PORT_RX_RCC RCC_GPIOA
#define DEBUG_PIN_RX GPIO3 
#define DEBUG_AF_RX GPIO_AF7

#define DEBUG_UART_EXTI EXTI26
#define DEBUG_UART_NVIC NVIC_USART2_IRQ

#define COMM_RCC_USART RCC_USART1
#define COMM_USART USART1
#define COMM_UART_SPEED (9600)

#define COMM_PORT_TX GPIOA
#define COMM_PORT_TX_RCC RCC_GPIOB 
#define COMM_PIN_TX GPIO9
#define COMM_AF_TX GPIO_AF7

#define COMM_PORT_RX GPIOA
#define COMM_PORT_RX_RCC RCC_GPIOB
#define COMM_PIN_RX GPIO10
#define COMM_AF_RX GPIO_AF7

#define COMM_UART_EXTI EXTI25
#define COMM_UART_NVIC NVIC_USART1_IRQ


/**
 * @brief setup communication uart and debug uart(usb through the stlink)
 * 
 */
void uart_setup();
/**
 * @brief implementation of write that redirects stdout on the communication uart and stderr on the debug uart
 * This function is never actually called by us: use fprintf and fscanf to communicate
 * 
 * @param file 
 * @param ptr 
 * @param len 
 * @return int 
 */
int _write(int file, const char *ptr, ssize_t len);
/**
 * @brief implementation of read that redirects stdout on the communication uart and stderr on the debug uart
 * This function is never actually called by us: use fprintf and fscanf to communicate
 * 
 * @param file 
 * @param ptr 
 * @param len 
 * @return int 
 */
int _read(int file,char *ptr, ssize_t len);
