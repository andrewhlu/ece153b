/*
 * ECE 153B - Winter 2020
 *
 * Name(s): Andrew Lu, Norman Chung
 * Section: Wednesday 7:00 - 9:50 PM
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
	// Remove the RTC write protection so that we can write to the RTC registers.
	RTC_Disable_Write_Protection();

	// Clear the alarm enable bit and the interrupt enable bit for both alarms. 
	// In addition, wait until access to both alarm registers is allowed. The 
	// hardware sets the write flag ALRxWF if alarm x can be modified.
	RTC -> CR &= ~RTC_CR_ALRAE;
	RTC -> CR &= ~RTC_CR_ALRBE;
	RTC -> CR &= ~RTC_CR_ALRAIE;
	RTC -> CR &= ~RTC_CR_ALRBIE;
	while(!((RTC -> ISR) & RTC_ISR_ALRAWF) || !((RTC -> ISR) & RTC_ISR_ALRBWF)) {}

	// Program Alarm A to set off an alarm when the seconds field of the RTC is 30 seconds.
	RTC -> ALRMAR |= RTC_ALRMAR_MSK4;
	RTC -> ALRMAR |= RTC_ALRMAR_MSK3;
	RTC -> ALRMAR |= RTC_ALRMAR_MSK2;
	RTC -> ALRMAR &= ~RTC_ALRMAR_MSK1;

	// BCD Tens is 011, BCD Ones is 0000
	RTC -> ALRMAR |= RTC_ALRMAR_ST_0;
	RTC -> ALRMAR |= RTC_ALRMAR_ST_1;

	// Program Alarm B to set off an alarm every second
	RTC -> ALRMBR |= RTC_ALRMBR_MSK4;
	RTC -> ALRMBR |= RTC_ALRMBR_MSK3;
	RTC -> ALRMBR |= RTC_ALRMBR_MSK2;
	RTC -> ALRMBR |= RTC_ALRMBR_MSK1;

	// Enable the alarm and its interrupt for both Alarm A and Alarm B.
	RTC -> CR |= RTC_CR_ALRAE;
	RTC -> CR |= RTC_CR_ALRBE;
	RTC -> CR |= RTC_CR_ALRAIE;
	RTC -> CR |= RTC_CR_ALRBIE;

	// Enable write protection for the RTC registers.
	RTC_Enable_Write_Protection();
}

void RTC_Alarm_Enable(void) {
	// Configure EXTI_RTSRx such that the interrupt triggers on the rising edge.
	EXTI -> RTSR1 |= EXTI_FTSR1_FT18;

	// Set the interrupt mask in EXTI_IMRx and the event mask in EXTI EMRx.
	EXTI -> IMR1 |= EXTI_IMR1_IM18;
	EXTI -> EMR1 |= EXTI_EMR1_EM18;

	// Clear the pending interrupt in EXTI_PRx by writing a 1 to the bit that 
	// corresponds to the target EXTI line.
	EXTI -> PR1 |= EXTI_PR1_PIF18;

	// Enable the interrupt in the NVIC and set it to have the highest priority.
	// The name of the RTC alarm in the NVIC is RTC_Alarm_IRQn.
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void RTC_Alarm_IRQHandler(void) {
	if(((RTC -> ISR) & RTC_ISR_ALRAF)) {
		// Alarm A was triggered
		// Toggle Red LED
		Red_LED_Toggle();

		// Clear Alarm A Flag
		RTC -> ISR &= ~RTC_ISR_ALRAF;

		// Clear interrupt pending bit
		EXTI -> PR1 |= EXTI_PR1_PIF18;
	}

	if(((RTC -> ISR) & RTC_ISR_ALRBF)) {
		// Alarm B was triggered
		// Toggle Green LED
		Green_LED_Toggle();

		// Clear Alarm B Flag
		RTC -> ISR &= ~RTC_ISR_ALRBF;

		// Clear interrupt pending bit
		EXTI -> PR1 |= EXTI_PR1_PIF18;
	}
}

int main(void) {
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	LCD_Initialization();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	while(1) {
		// Display the time.
		Get_RTC_Calendar(strTime, strDate);
		LCD_DisplayString(strTime);
	}
}
