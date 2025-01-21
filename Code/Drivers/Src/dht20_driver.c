/*
 * dht20_driver.c
 *
 *  Created on: Dec 23, 2024
 *      Author: Bane
 */

#include "dht20_driver.h"
#include <stdio.h>

/*
 * Initializing/Declaring  variables and structure
 */
uint8_t temperature = 0;
uint8_t humidity = 0;

I2C_Handle_t I2C1Handle;

void I2C1_GPIOInits(void)
{
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;
	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;

	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PIN_PU;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = 4;
	I2CPins. GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_LOW;

	//scl
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_6;
	GPIO_Init(&I2CPins);


	//sda
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_7;

	GPIO_Init(&I2CPins);
}

void I2C1_Inits(void)
{
	I2C1Handle.pI2Cx = I2C1;
	I2C1Handle.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = 70000;

	I2C_Init(&I2C1Handle);

}

void DHT20_Init(void)
{
	// Initialize registers
	uint8_t dht20_registers[3] = {0x1B, 0x1C, 0x1E};
	I2C_MasterSendData(&I2C1Handle, dht20_registers, 3, DHT20_ADDRESS, I2C_DISABLE_SR);
	Delay_ms(20);

	uint8_t dht20_cal[3] = {0xBE, 0x08, 0x00};
	I2C_MasterSendData(&I2C1Handle, dht20_cal, 3, DHT20_ADDRESS, I2C_DISABLE_SR);
	Delay_ms(20);

	Delay_ms(160);

	uint8_t trigger_cm[3] = {0xAC, 0x33, 0x00};
	I2C_MasterSendData(&I2C1Handle, trigger_cm, 3, DHT20_ADDRESS, I2C_DISABLE_SR);

	Delay_ms(160);

	uint8_t RxBuffer[7] = {0};

	I2C_MasterReceiveData(&I2C1Handle, RxBuffer, 7, DHT20_ADDRESS, I2C_ENABLE_SR);

	Delay_ms(160);

	if ((RxBuffer[0] & 0x80) == 0)
	{
		uint32_t temperature_raw = (((uint32_t)RxBuffer[3] & 0x0F) << 16) |
		                           ((uint32_t)RxBuffer[4] << 8) |
		                           (uint32_t)RxBuffer[5]; // 23C

		temperature = temperature_raw * 200.0f / (1 << 20) - 50;

		uint32_t humidity_data = (((uint32_t)RxBuffer[1] << 12) |
			                               ((uint32_t)RxBuffer[2] << 4) |
			                               ((uint32_t)(RxBuffer[3] >> 4) & 0x0F));

		humidity = humidity_data * 100.0f / (1 << 20); // 62%
	}
}
