/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Norman Chung, Andrew Lu
 * Section: Wed 7-9:50 PM
 * Lab: 1B
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize Joystick
	GPIOA -> MODER &= ~GPIO_MODER_MODE0_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE0_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE1_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE1_1;
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_0;
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD0_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD0_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD1_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD1_1;
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD2_0;
	GPIOA -> PUPDR |= GPIO_PUPDR_PUPD2_1;
	
	// Configure SYSCFG EXTI
	SYSCFG -> EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0;
	SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PA;
	SYSCFG -> EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1;
	SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PA;
	SYSCFG -> EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI2;
	SYSCFG -> EXTICR[0] |= SYSCFG_EXTICR1_EXTI2_PA;
	// may have to change
	
	// Configure EXTI Trigger
	EXTI -> FTSR1 |= EXTI_FTSR1_FT0;
	EXTI -> FTSR1 |= EXTI_FTSR1_FT1;
	EXTI -> FTSR1 |= EXTI_FTSR1_FT2;
	
	// Enable EXTI
	EXTI -> IMR1 |= EXTI_IMR1_IM0;
	EXTI -> IMR1 |= EXTI_IMR1_IM1;
	EXTI -> IMR1 |= EXTI_IMR1_IM2;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI0_IRQn);
	NVIC_SetPriority(EXTI0_IRQn, 0);
	NVIC_EnableIRQ(EXTI1_IRQn);
	NVIC_SetPriority(EXTI1_IRQn, 0);
	NVIC_EnableIRQ(EXTI2_IRQn);
	NVIC_SetPriority(EXTI2_IRQn, 0);
}

// Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
void EXTI0_IRQHandler(void) {
	EXTI -> PR1 |= EXTI_PR1_PIF0;

	// Center toggles both LEDs
	Red_LED_Toggle();
	Green_LED_Toggle();
}

void EXTI1_IRQHandler(void) {
	EXTI -> PR1 |= EXTI_PR1_PIF1;

	// Left toggles green LED
	Green_LED_Toggle();
}
void EXTI2_IRQHandler(void) {
	EXTI -> PR1 |= EXTI_PR1_PIF2;

	// Right toggles red LED
	Red_LED_Toggle();
}
