#include "stm32l476xx.h"

#include "L3GD20.h"
#include "SPI.h"
#include "SysClock.h"
#include "SysTimer.h"
#include "UART.h"

#include <stdio.h>

typedef struct {
	float x; 
	float y; 
	float z; 
} L3GD20_Data_t;

float takeMeasurement(uint8_t bitNum, uint8_t hAddress, uint8_t lAddress) {
	uint8_t buffer = 0;
	int16_t measurement = 0;
	GYRO_IO_Read(&buffer, L3GD20_STATUS_REG_ADDR, sizeof(buffer));

	if(buffer & bitNum) {
		// New data is available
		buffer = 0;
		GYRO_IO_Read(&buffer, hAddress, sizeof(buffer));
		measurement |= buffer << 8;

		buffer = 0;
		GYRO_IO_Read(&buffer, lAddress, sizeof(buffer));
		measurement |= buffer;

		if(measurement & 0x80) {
			// The number is in negative 2s complement, convert it to negative decimal
			measurement = ~measurement;
			measurement += 0x01;
			measurement *= -1;
		}
	}

	// Convert to DPS (1 unit = 70 mdps)
	return measurement * 0.07;
}

int main(void){
	System_Clock_Init();   // System Clock = 80 MHz
	SysTick_Init();
	
	// Initialize Gyroscope
	GYRO_Init();  

	// Initialize USART2
	UART2_Init();
	UART2_GPIO_Init();
	USART_Init(USART2);

	while(1) {
		printf("X: %.02f , Y: %.02f, Z: %.02f\n", 
			takeMeasurement(0x01, L3GD20_OUT_X_H_ADDR, L3GD20_OUT_X_L_ADDR),
			takeMeasurement(0x02, L3GD20_OUT_Y_H_ADDR, L3GD20_OUT_Y_L_ADDR),
			takeMeasurement(0x04, L3GD20_OUT_Z_H_ADDR, L3GD20_OUT_Z_L_ADDR)
		);

		delay(500); // Small delay between receiving measurements
	}
}
