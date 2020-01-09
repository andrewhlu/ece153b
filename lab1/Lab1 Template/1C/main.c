/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 1C
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "SysTimer.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	RCC->CFGR |= 0; // TODO - Replace with value that will make MSI the system clock source
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	RCC->CR |= 0; // TODO - Replace with value that will make range 8 MHz
	
	// Use the MSI clock range that is defined in RCC_CR
	RCC->CR |= 0; // TODO - Replace with value that will select range in RCC->CR
	
	// Enable MSI oscillator
	RCC->CR |= 0; // TODO - Replace with value that will enable MSI
	
	// Wait until MSI is ready
	while((RCC->CR & 0 /* TODO - Replace with value that checks whether MSI is ready */) == 0);
}

int main() {
	// Initialization 
	System_Clock_Init();
	SysTick_Init();
	LED_Init();
	
	while(1) {
		// Write Simple Program here...
	}
}
