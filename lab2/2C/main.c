/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"
#include "lcd.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
	// Enable GPIO Clock for PB6.
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOBEN;

	// Set PB6 to alternative function mode and select alternate function 2 (TIM4_CH1)
	GPIOB -> MODER &= ~GPIO_MODER_MODE6_0;
	GPIOB -> MODER |= GPIO_MODER_MODE6_1;
	GPIOB -> AFR[0] &= ~GPIO_AFRL_AFSEL6_0;
	GPIOB -> AFR[0] |= GPIO_AFRL_AFSEL6_1;
	GPIOB -> AFR[0] &= ~GPIO_AFRL_AFSEL6_2;
	GPIOB -> AFR[0] &= ~GPIO_AFRL_AFSEL6_3;

	// Set PB6 to no pull-up, no pull-down.
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD6;

	// Enable timer 4 in RCC_APB1ENRx
	RCC -> APB1ENR1 |= RCC_APB1ENR1_TIM4EN;

	// Enable auto reload preload in the control register and set the auto reload 
	// value to its maximum value. Set the prescaler.
	TIM4 -> CR1 |= TIM_CR1_ARPE;
	TIM4 -> ARR |= TIM_ARR_ARR;
	TIM4 -> PSC = (uint32_t) 15;

	// In the capture/compare mode register, set the input capture mode bits such that 
	// the input capture is mapped to timer input 1.
	TIM4 -> CCMR1 |= TIM_CCMR1_CC1S_0;
	TIM4 -> CCMR1 &= ~TIM_CCMR1_CC1S_1;

	// In the capture/compare enable register, set bits to capture both rising/falling
	// edges and enable capturing.
	TIM4 -> CCER |= TIM_CCER_CC1P;
	TIM4 -> CCER |= TIM_CCER_CC1NP;
	TIM4 -> CCER |= TIM_CCER_CC1E;

	// In the DMA/Interrupt enable register, enable both interrupt and DMA requests. In
	// addition, enable the update interrupt.
	TIM4 -> DIER |= TIM_DIER_CC1IE;
	TIM4 -> DIER |= TIM_DIER_CC1DE;
	TIM4 -> DIER |= TIM_DIER_UIE;

	// Enable update generation in the event generation register.
	TIM4 -> EGR |= TIM_EGR_UG;

	// Clear the update interrupt flag.
	TIM4 -> SR &= ~TIM_SR_UIF;

	// Set the direction of the counter and enable the counter in the control register.
	TIM4 -> CR1 &= ~TIM_CR1_DIR;
	TIM4 -> CR1 |= TIM_CR1_CEN;

	// Enable the interrupt (TIM4_IRQn) in the NVIC and set its priority to 2.
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
}

void TIM4_IRQHandler(void) {
	if((TIM4 -> SR) & TIM_SR_CC1IF) {
		// An edge triggered this interrupt
		if((GPIOB -> IDR) & GPIO_IDR_ID6) {
			// A rising edge triggered this interrupt, store the counter value into lastValue
			lastValue = (TIM4 -> CCR1) & TIM_CCR1_CCR1;
		}
		else {
			// A falling edge triggered this interrupt, store the counter value into currentValue
			// and calculate the time interval
			currentValue = (TIM4 -> CCR1) & TIM_CCR1_CCR1;
			timeInterval = currentValue + (65536 * overflowCount) - lastValue;
			overflowCount = 0;
		}
	}
	else if((TIM4 -> SR) & TIM_SR_UIF) {
		// A counter overflow triggered this interrupt
		overflowCount++;
	}

	// Clear the update interrupt flag.
	TIM4 -> SR &= ~TIM_SR_UIF;
}

void Trigger_Setup() {
	// Enable GPIO Clock for PE11.
	RCC -> AHB2ENR |= RCC_AHB2ENR_GPIOEEN;

	// Set PE11 to alternative function mode and select alternate function 1 (TIM1_CH2).
	GPIOE -> MODER &= ~GPIO_MODER_MODE11_0;
	GPIOE -> MODER |= GPIO_MODER_MODE11_1;
	GPIOE -> AFR[1] |= GPIO_AFRH_AFSEL11_0;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL11_1;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL11_2;
	GPIOE -> AFR[1] &= ~GPIO_AFRH_AFSEL11_3;

	// Set PE11 to no pull-up, no pull-down.
	GPIOE -> PUPDR &= ~GPIO_PUPDR_PUPD11;

	// Set the output type of PE11 to push-pull.
	GPIOE -> OTYPER &= ~GPIO_OTYPER_OT11;

	// Set PE11 to very high output speed.
	GPIOE -> OSPEEDR |= GPIO_OSPEEDR_OSPEED11;

	// Enable timer 1 in RCC_APB2ENR.
	RCC -> APB2ENR |= RCC_APB2ENR_TIM1EN;

	// Set the prescaler to 15.
	TIM1 -> PSC &= ~TIM_PSC_PSC;
	TIM1 -> PSC |= (uint32_t) 15;

	// Enable auto reload preload in the control register and set the auto reload 
	// value to its maximum value.
	TIM1 -> CR1 |= TIM_CR1_ARPE;
	TIM1 -> ARR &= ~TIM_ARR_ARR;
	TIM1 -> ARR |= (uint32_t) 20;

	// Set the CCR value that will trigger the sensor.
	TIM1 -> CCR2 &= ~TIM_CCR2_CCR2;
	TIM1 -> CCR2 |= (uint32_t) 10;

	// In the capture/compare mode register, set the output control mode bits such that
	// the timer is in PWM Mode 1 and enable the output compare preload.
	TIM1 -> CCMR1 &= ~TIM_CCMR1_OC2M;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC2M_1;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC2M_2;
	TIM1 -> CCMR1 |= TIM_CCMR1_OC2PE;

	// Enable the output in the capture/compare enable register.
	TIM1 -> CCER |= TIM_CCER_CC2E;

	// In the break and dead-time register, set the bits for main output enable and
	// off-state selection for run mode.
	TIM1 -> BDTR |= TIM_BDTR_MOE;
	TIM1 -> BDTR |= TIM_BDTR_OSSR;

	// Enable update generation in the event generation register.
	TIM1 -> EGR |= TIM_EGR_UG;

	// Enable the update interrupt and clear the update interrupt flag.
	TIM1 -> DIER |= TIM_DIER_UIE;
	TIM1 -> SR &= ~TIM_SR_UIF;

	// Set the direction of the counter and enable the counter in the control register.
	TIM1 -> CR1 &= ~TIM_CR1_DIR;
	TIM1 -> CR1 |= TIM_CR1_CEN;
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	// Setup LCD
	LCD_Initialization();
	LCD_Clear();
	
	char message[6];
	while(1) {
		// Code for Part C1 -- Comment out when demoing Part C2
		// sprintf(message, "%.06d", (timeInterval / 1000));
		
		// Code for Part C2 -- Comment out when demoing Part C1
		TIM1 -> BDTR |= TIM_BDTR_MOE;
		if(timeInterval <= 25000) {
			sprintf(message, "%.06d", timeInterval / 58);
		}
		
		LCD_DisplayString((uint8_t *) message);
	}
}
