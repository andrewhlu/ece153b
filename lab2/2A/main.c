/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 2A
 */

#include "stm32l476xx.h"
 
void PWM_Init() {
	// Enable GPIO Port E Clock
	// TODO
	
	// Enable TIM1 Clock
	// TODO
	
	// Configure PE8
	// TODO
	
	// Configure PWM Output for TIM1 CH 1N
	// TODO
}
 
int main() {
	// Initialization - We will use the default 4 MHz clock
	PWM_Init();
	
	// Periodic Dimming
	int i;
	while(1) {
		// TODO (changing duty cycle, etc.)
		
		for(i=0; i<1000; ++i); // Some Delay
	}
	
	return 0;
}
