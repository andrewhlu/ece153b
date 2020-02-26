#include "stm32l476xx.h"

#include "I2C.h"
#include "LCD.h"
#include "SysClock.h"

#include "string.h"
#include "stdio.h"
	
int main(void) {
	System_Clock_Init(); // System Clock = 80 MHz
	
	LCD_Initialization();
	LCD_Clear();
	
	// Initialize I2C
	I2C_GPIO_Init();
	I2C_Initialization();
	
	int i;
	char message[6];
	uint8_t SlaveAddress;
	uint8_t Data_Receive[6];
	uint8_t Data_Send[6] = {0};
	while(1) {
		// Reset values
		memset(Data_Receive, 0, sizeof(Data_Receive));
		memset(message, '\0', sizeof(message));

		// Determine Slave Address
		// Note the "<< 1" must be present because bit 0 is treated as a don't care in 7-bit addressing mode
		SlaveAddress = 0b1001000 << 1;
		
		// Get Temperature
		// First, send a command to the sensor for reading the temperature
		I2C_SendData(I2C1, SlaveAddress, Data_Send, 1);

		// Next, get the measurement
		I2C_ReceiveData(I2C1, SlaveAddress, Data_Receive, 1);
		int8_t measurement = Data_Receive[0];

		if(measurement & 0x80) {
			// The number is in negative 2s complement, convert it to negative decimal
			measurement = ~measurement;
			measurement += 0x01;
			measurement *= -1;
		}

		sprintf(message, "%6d", measurement);

		// Print Temperature to LCD
		LCD_DisplayString(message);
		
		// Some delay
		for(i = 0; i < 50000; ++i); 
	}
}
