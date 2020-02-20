#include "stm32l476xx.h"

#include "LED.h"
#include "SysClock.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

// Initializes USART for each part of Lab 3A
// part = 1: UART Communication with Termite
// part = 2: Bluetooth Communication with Phone
void Init_USARTx(int part) {
	if(part == 1) {
		UART2_Init();
		UART2_GPIO_Init();
		USART_Init(USART2);
	} else if(part == 2) {
		UART1_Init();
		UART1_GPIO_Init();
		USART_Init(USART1);
	} else {
		// Do nothing...
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	LED_Init();
	
	// Initialize UART -- change the argument depending on the part you are working on
	Init_USARTx(1);
	
	char rxByte;
	while(1) {
		// TODO
	}
}
