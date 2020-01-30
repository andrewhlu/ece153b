/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Andrew Lu, Norman Chung
 * Section: Wednesday 7:00 - 9:50 PM
 * Lab: 2A
 */

#include "stm32l476xx.h"
 
void PWM_Init() {
	// Enable GPIO Port E Clock
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOEEN;
	
	// Enable TIM1 Clock
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	// Configure PE8
	GPIOE -> MODER &= ~GPIO_MODER_MODE8_0;
	GPIOE -> MODER |= GPIO_MODER_MODE8_1;
	GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED8_0;
	GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED8_1;
	GPIOE -> PUPDR &= ~GPIO_PUPDR_PUPD8_0;
	GPIOE -> PUPDR &= ~GPIO_PUPDR_PUPD8_1;
	
	// Configure PWM Output for TIM1 CH 1N
	GPIOE -> AFR[1] |= GPIO_AFRH_AFSEL8_0;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL8_1;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL8_2;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL8_3;
	
	//(a) Set the direction such that the timer counts up.
	TIM1 -> CR1 &= ~TIM_CR1_DIR;
	//(b) Set the prescaler value.
	TIM1 -> PSC &= ~TIM_PSC_PSC;
	//(c) Set the auto-reload value.
	TIM1 -> ARR &= ~TIM_ARR_ARR;
	TIM1 -> ARR |= (uint32_t)1000;
	// (d) Clear the output compare mode bits for channel 1
	TIM1 -> CCMR1 &= ~TIM_CCMR1_OC1M;
	// (d) Set the output compare mode bits to PWM mode 1.
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_1;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1M_2;
	// (d) Enable output preload for channel 1
	TIM1 -> CCMR1 |= TIM_CCMR1_OC1PE;
	//(e) Set the output polarity for channel 1N to active high.
	TIM1 -> CCER &= ~TIM_CCER_CC1NP;
	//(f) Enable the output for channel 1N.
	TIM1 -> CCER |= TIM_CCER_CC1NE;
	//(g) Enable the main output.
	TIM1 -> BDTR |= TIM_BDTR_MOE;
	//(h) Set the capture/compare value. For now, set it such that the duty cycle of the PWM output is 50%.
	TIM1 -> CCR1 &= ~TIM_CCR1_CCR1;
	// TIM1 -> CCR1 |= (uint32_t)500;
	//(i) Enable the counter.
	TIM1 -> CR1 |= TIM_CR1_CEN;
}
 
int main() {
	// Initialization - We will use the default 4 MHz clock
	PWM_Init();
	
	// Periodic Dimming
	int i;
	uint32_t arr = 1000;
	uint32_t ccr = 0;
	while(1) {
		// Change duty cycle
		TIM1 -> CCR1 = ccr % arr;
		ccr += 1;
		
		for(i=0; i<1000; ++i); // Some Delay
	}
	
	return 0;
}
