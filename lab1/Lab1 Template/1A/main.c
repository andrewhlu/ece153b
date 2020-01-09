/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Norman Chung, Andrew Lu
 * Section: Wed 7-9:50 PM
 * Lab: 1A
 */

#include "stm32l476xx.h"
 
void Init() {
	// Enable HSI
	RCC->CR |= RCC_CR_HSION;
	while((RCC->CR & RCC_CR_HSIRDY) == 0);
	
	// Select HSI as system clock source
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while((RCC->CFGR & RCC_CFGR_SWS) == 0);
	
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
	
	// Initialize Green LED
	GPIOE -> MODER |= GPIO_MODER_MODE8_0;
	GPIOE -> MODER &= ~GPIO_MODER_MODE8_1;
	GPIOE -> OTYPER &= ~GPIO_OTYPER_OT8;
	GPIOE -> PUPDR &= ~GPIO_PUPDR_PUPD8_0;
	GPIOE -> PUPDR &= ~GPIO_PUPDR_PUPD8_1;
	
	// Initialize Joystick
	GPIOA -> MODER &= ~GPIO_MODER_MODE0_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE0_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE1_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE1_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE3_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE3_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE5_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE5_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD0_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD0_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD1_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD1_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD2_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD2_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD3_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD3_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD5_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD5_1;
}

void polling() {
	// Left toggles Green LED
	if((GPIOA -> IDR) & GPIO_IDR_ID1) {
		GPIOE -> ODR ^= GPIO_ODR_OD8;
	}

	// Right toggles Red LED
	if((GPIOA -> IDR) & GPIO_IDR_ID2) {
		GPIOB -> ODR ^= GPIO_ODR_OD2;
	}

	// Up sets both LEDs to high
	if((GPIOA -> IDR) & GPIO_IDR_ID3) {
		GPIOE -> ODR |= GPIO_ODR_OD8;
		GPIOB -> ODR |= GPIO_ODR_OD2;
	}

	// Down sets both LEDs to low
	if((GPIOA -> IDR) & GPIO_IDR_ID5) {
		GPIOE -> ODR &= ~GPIO_ODR_OD8;
		GPIOB -> ODR &= ~GPIO_ODR_OD2;
	}

	// Center toggles both LEDs
	if((GPIOA -> IDR) & GPIO_IDR_ID0) {
		GPIOE -> ODR ^= GPIO_ODR_OD8;
		GPIOB -> ODR ^= GPIO_ODR_OD2;
	}
}
 
int main() {
	// Initialization
	Init();
	
	// Polling to Check for Button Presses
	while(1) {
		polling();
	}
	
	return 0;
}
