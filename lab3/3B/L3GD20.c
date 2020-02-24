#include "L3GD20.h"
#include "SPI.h"

// Modifed from l3gd20.c V2.0.0, COPYRIGHT(c) 2015 STMicroelectronics

void GYRO_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Initialize SPI
	SPI2_GPIO_Init();
	SPI_Init();
	
	// Initialize Gyroscope
	GYRO_IO_CS_Init();	
	L3GD20_Init();
}

// Gyroscope IO functions
void GYRO_IO_CS_Init(void) {
	// Set GPIO pin to output mode.
	GPIOD -> MODER |= GPIO_MODER_MODE7_0;
	GPIOD -> MODER &= ~GPIO_MODER_MODE7_1;

	// Set GPIO pin to have a push-pull output type.
	GPIOD -> OTYPER &= ~GPIO_OTYPER_OT7;

	// Set GPIO pin to very high speed.
	GPIOD -> OSPEEDR |= GPIO_OSPEEDR_OSPEED7;

	// Configure GPIO pin to use no pull-up/down resistors for I/O.
	GPIOD -> PUPDR &= ~GPIO_PUPDR_PUPD7;

	// Deselect the Gyroscope
	L3GD20_CS_HIGH;
}

void GYRO_IO_Write(uint8_t *pBuffer, uint8_t WriteAddr, uint8_t NumByteToWrite){
	
	uint8_t rxBuffer[32];
	
	/* Configure the MS bit: 
       - When 0, the address will remain unchanged in multiple read/write commands.
       - When 1, the address will be auto incremented in multiple read/write commands.
	*/
	if(NumByteToWrite > 0x01){
		WriteAddr |= (uint8_t) MULTIPLEBYTE_CMD; // Set write mode & auto-increment
	}
	
	// Set chip select Low at the start of the transmission 
	L3GD20_CS_LOW;  // Gyro CS low
	SPI_Delay(10);  
	
	// Send the Address of the indexed register 
	SPI_Write(SPI2, &WriteAddr, rxBuffer, 1);
	
	// Send the data that will be written into the device (MSB First) 
	SPI_Write(SPI2, pBuffer, rxBuffer, NumByteToWrite);
  
	// Set chip select High at the end of the transmission  
	SPI_Delay(10); // Gyro CS high
	L3GD20_CS_HIGH;
}

void GYRO_IO_Read(uint8_t *pBuffer, uint8_t ReadAddr, uint8_t NumByteToRead){

	uint8_t rxBuffer[32];
	
	uint8_t AddrByte = ReadAddr | ((uint8_t) 0xC0);  // set read mode & autoincrement
	
	// Set chip select Low at the start of the transmission 
	L3GD20_CS_LOW; // Gyro CS low
	SPI_Delay(10);
	
	// Send the Address of the indexed register 
	SPI_Write(SPI2, &AddrByte, rxBuffer, 1);
	
	// Receive the data that will be read from the device (MSB First) 
	SPI_Read(SPI2, pBuffer, NumByteToRead);
  
	// Set chip select High at the end of the transmission  
	SPI_Delay(10); // Gyro CS high
	L3GD20_CS_HIGH;
}	

void L3GD20_Init(void) {  
	// Enable all axes and set the gyroscope to normal (active) mode. Set the data rate and 
	// bandwidth such that the output data rate is 95 Hz with a cut-off of 25.
	// (You should be writing to control register 1.)
	uint8_t buffer = 0;
	buffer |= L3GD20_Z_ENABLE | L3GD20_Y_ENABLE | L3GD20_X_ENABLE | L3GD20_MODE_ACTIVE | L3GD20_BANDWIDTH_4;
	buffer &= ~L3GD20_OUTPUT_DATARATE_1;
	GYRO_IO_Write(&buffer, L3GD20_CTRL_REG1_ADDR, sizeof(buffer));

	// Select the 2000 dps full scale. (You should be writing to control register 4.)
	buffer = 0;
	buffer |= L3GD20_FULLSCALE_2000;
	GYRO_IO_Write(&buffer, L3GD20_CTRL_REG4_ADDR, sizeof(buffer));
}
