/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 1B
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize Joystick
	// TODO
	
	// Configure SYSCFG EXTI
	// TODO
	
	// Configure EXTI Trigger
	// TODO
	
	// Enable EXTI
	// TODO
	
	// Configure and Enable in NVIC
	// TODO
}

// Write Interrupt Handlers (look in startup_stm32l476xx.s to find the 
// interrupt handler names that you should use)
// TODO

void ExampleInterruptHandler(void) {
	// Clear interrupt pending bit
	
	// Define behavior that occurs when interrupt occurs
}