/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Norman Chung, Andrew Lu
 * Section: Wed 7-9:50 PM
 * Lab: 1C
 */

#include "LED.h"

void LED_Init(void) {
	// Enable GPIO Clocks
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	// Initialize Red LED
	GPIOB -> MODER |= GPIO_MODER_MODE2_0;
	GPIOB -> MODER &= ~GPIO_MODER_MODE2_1;
	GPIOB -> OTYPER &= ~GPIO_OTYPER_OT2;
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD2_0;
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD2_1;
}

void Red_LED_Off(void) {
	GPIOB -> ODR &= ~GPIO_ODR_OD2;
}

void Red_LED_On(void) {
	GPIOB -> ODR |= GPIO_ODR_OD2;
}

void Red_LED_Toggle(void){
	GPIOB -> ODR ^= GPIO_ODR_OD2;
}
