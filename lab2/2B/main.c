/*
 * ECE 153B - Winter 2020
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */
 
#include "stm32l476xx.h"

#include "LCD.h"
#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Set_Alarm(void) {
	// TODO
}

void RTC_Alarm_Enable(void) {
	// TODO
}

void RTC_Alarm_IRQHandler(void) {
	// TODO
}

int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	LCD_Initialization();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	while(1) {
		// TODO
		// uint8_t string = (uint8_t) RTC_TIME_GetHour();
		Get_RTC_Calendar(strTime, strDate);
		// test(strTime);
		LCD_DisplayString(strTime);
	}
}
