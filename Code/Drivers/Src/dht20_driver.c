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
uint8_t GET_STATUS_WORD = 0x71;
uint8_t status_word = 0;
uint8_t measured_data[6];
uint8_t temperature = 0;
uint8_t humidity = 0;
uint8_t data = 0;

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
	Delay_ms(10);

	uint8_t trigger_command[3] = {0xAC, 0x33, 0x00};
	I2C_MasterSendData(&I2C1Handle, trigger_command, sizeof(trigger_command), DHT20_ADDRESS, I2C_ENABLE_SR);

	Delay_ms(160);

	uint8_t recieve_buffer[6];
	I2C_MasterReceiveData(&I2C1Handle, recieve_buffer, 6, DHT20_ADDRESS, I2C_ENABLE_SR);

	if ((recieve_buffer[0] & 0x80) == 0)
	{
		data = (((uint32_t)recieve_buffer[3] & 0x0F) << 16) + ((uint32_t)recieve_buffer[4] << 8) + (uint32_t)recieve_buffer[5];
		temperature = data * 200.0f / (1 << 20) - 50;
	}
}

void DHT20_Init2(void)
{
	 Delay_ms(200);  // Wait after power-on

	 uint8_t dht20_registers[] = {0x1B, 0x1C, 0x1E};
	 I2C_MasterSendData(&I2C1Handle, dht20_registers, sizeof(dht20_registers), DHT20_ADDRESS, I2C_DISABLE_SR);
	 Delay_ms(160);

	    // Send trigger measurement command
	    uint8_t trigger_command[3] = {0xAC, 0x33, 0x00};
	    I2C_MasterSendData(&I2C1Handle, trigger_command, sizeof(trigger_command), DHT20_ADDRESS, I2C_ENABLE_SR);

	    // Wait 80ms for measurement to complete
	    Delay_ms(160);

	    // Check measurement completion using status word
	    uint8_t byte_word = 0xFF;
	    while (1)
	    {
	        I2C_MasterReceiveData(&I2C1Handle, &byte_word, 1, DHT20_ADDRESS, I2C_ENABLE_SR);
	        if ((byte_word & 0x80) == 0) // Check if bit 7 is cleared
	        {
	            break;  // Measurement is complete
	        }
	        Delay_ms(10);  // Wait a bit before polling again
	    }

	    // Add a small delay after receiving 0x18
	    Delay_ms(80);

	    // Now read the measurement data (6 bytes)
	    uint8_t recieve_buffer[7];
	    I2C_MasterReceiveData(&I2C1Handle, recieve_buffer, 7, DHT20_ADDRESS, I2C_DISABLE_SR);

	    uint8_t received_crc = recieve_buffer[6];
	        uint8_t calculated_crc = Calculate_CRC(recieve_buffer, 6);
	        if (received_crc != calculated_crc)
	        {
	            // Handle CRC error
	            return;
	        }

	        uint8_t byte_1 = recieve_buffer[2] & 0x0F;
	        	uint8_t byte_2 = recieve_buffer[3];
	        	uint8_t byte_3 = recieve_buffer[4];

	        	uint8_t combined_bytes = 0;

	        	combined_bytes = (byte_1 << 16) | byte_2 | byte_3;
	        	temperature = (combined_bytes / 0x100000) * 200 - 50;

	    // Extract temperature data
	    uint32_t data = (((uint32_t)recieve_buffer[3] & 0x0F) << 16) |
	                    ((uint32_t)recieve_buffer[4] << 8) |
	                    (uint32_t)recieve_buffer[5];
	    temperature = data * 200.0f / (1 << 20) - 50;

	    // Extract humidity data
	    uint32_t humidity_data = (((uint32_t)recieve_buffer[1] << 12) |
	                               ((uint32_t)recieve_buffer[2] << 4) |
	                               ((uint32_t)(recieve_buffer[3] >> 4) & 0x0F));
	    humidity = humidity_data * 100.0f / (1 << 20);
}

uint8_t Calculate_CRC(uint8_t *data, uint8_t length)
{
    uint8_t crc = 0xFF; // Initial CRC value
    uint8_t polynomial = 0x31; // Polynomial: 1 + X^4 + X^5 + X^8 (binary 0011 0001)

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i]; // XOR with each data byte

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80)
            {
                crc = (crc << 1) ^ polynomial; // Apply polynomial
            }
            else
            {
                crc <<= 1; // Shift left
            }
        }
    }

    return crc;
}

void Get_And_Check_Status_Word(void)
{
	I2C_MasterSendData(&I2C1Handle, &GET_STATUS_WORD, sizeof(GET_STATUS_WORD), DHT20_ADDRESS, I2C_ENABLE_SR);

	I2C_MasterReceiveData(&I2C1Handle, &status_word, sizeof(status_word), DHT20_ADDRESS, I2C_DISABLE_SR);

	if (status_word == 0x18)
	{
		// Proceed to the next step
	}
	else
	{
		// TODO: Initialize registers
		uint8_t dht20_registers[] = {0x1B, 0x1C, 0x1E};
		I2C_MasterSendData(&I2C1Handle, dht20_registers, sizeof(dht20_registers), DHT20_ADDRESS, I2C_DISABLE_SR);
	}
}

void Trigger_Measurement(void)
{
	Delay_ms(160);

	uint8_t measurement_parameters[] = {TRIGGER_MEASUREMENT, PARAMETER_ONE, PARAMETER_TWO};

	I2C_MasterSendData(&I2C1Handle, measurement_parameters, sizeof(measurement_parameters), DHT20_ADDRESS, I2C_ENABLE_SR);
}

void Wait_For_Masurement(void)
{
	I2C_MasterReceiveData(&I2C1Handle, &status_word, sizeof(status_word), DHT20_ADDRESS, I2C_ENABLE_SR);

	// Wait for measurement
	if ((status_word & 0x80) == 0)
	{
		I2C_MasterReceiveData(&I2C1Handle, measured_data, sizeof(measured_data), DHT20_ADDRESS, I2C_ENABLE_SR);
	}
}

void Wait_For_Masurement2(void)
{
	status_word = 0;
	do
	{
		I2C_MasterReceiveData(&I2C1Handle, &status_word, 1, DHT20_ADDRESS, I2C_ENABLE_SR);
	    Delay_ms(10);  // Add a small delay to avoid overwhelming the sensor
	} while (status_word & 0x80);
}

void Calculate_Temperature(void)
{
	uint8_t byte_1 = measured_data[2] & 0x0F;
	uint8_t byte_2 = measured_data[3];
	uint8_t byte_3 = measured_data[4];

	uint8_t combined_bytes = 0;

	combined_bytes = (byte_1 << 16) | byte_2 | byte_3;
	temperature = (combined_bytes / 0x100000) * 200 - 50;
}

void Calculate_Humidity(void)
{
	// TODO:
}

uint8_t Calculate_CRC8(uint8_t *data, uint8_t length)
{
    uint8_t crc = 0xFF; // Initial value
    uint8_t polynomial = 0x31; // CRC-8 polynomial: x^8 + x^5 + x^4 + 1

    for (uint8_t i = 0; i < length; i++)
    {
        crc ^= data[i]; // XOR the data byte with the current CRC

        for (uint8_t bit = 0; bit < 8; bit++)
        {
            if (crc & 0x80) // If MSB is 1
            {
                crc = (crc << 1) ^ polynomial; // Shift left and XOR with polynomial
            }
            else
            {
                crc <<= 1; // Just shift left
            }
        }
    }
    return crc; // Final CRC value
}

void New_DHT20_Init(void)
{
	char message_crc[50] = "Ok";
	char message_data[50] = "Ok";

	// Initialize registers
	uint8_t dht20_registers[3] = {0x1B, 0x1C, 0x1E};
	I2C_MasterSendData(&I2C1Handle, dht20_registers, 3, DHT20_ADDRESS, I2C_DISABLE_SR);
	Delay_ms(20);

	uint8_t dht20_cal[3] = {0xBE, 0x08, 0x00};
	I2C_MasterSendData(&I2C1Handle, dht20_cal, 3, DHT20_ADDRESS, I2C_DISABLE_SR);
	Delay_ms(20);

	Delay_ms(160);
	uint8_t temperature_not_raw = 2;

	uint8_t trigger_cm[3] = {0xAC, 0x33, 0x00};
	I2C_MasterSendData(&I2C1Handle, trigger_cm, 3, DHT20_ADDRESS, I2C_DISABLE_SR);

	Delay_ms(160);

	uint8_t status_word_first = 0;
	uint8_t RxBuffer[7] = {0};
	uint8_t crc_data;

	I2C_MasterReceiveData(&I2C1Handle, RxBuffer, 7, DHT20_ADDRESS, I2C_ENABLE_SR);

	Delay_ms(160);

	if ((RxBuffer[0] & 0x80) == 0)
	{
		uint8_t calculated_crc = Calculate_CRC8(RxBuffer, 6);
		uint8_t received_crc = RxBuffer[6];

		uint32_t temperature_raw = (((uint32_t)RxBuffer[3] & 0x0F) << 16) |
		                           ((uint32_t)RxBuffer[4] << 8) |
		                           (uint32_t)RxBuffer[5]; // 23C

		temperature_not_raw = temperature_raw * 200.0f / (1 << 20) - 50;

		uint32_t humidity_data = (((uint32_t)RxBuffer[1] << 12) |
			                               ((uint32_t)RxBuffer[2] << 4) |
			                               ((uint32_t)(RxBuffer[3] >> 4) & 0x0F));

		humidity = humidity_data * 100.0f / (1 << 20); // 62%

		 if (calculated_crc == received_crc)
		 {
			 sprintf(message_crc, "CRC Check Passed.\n");
		  }
		 else
		 {
	         sprintf(message_crc, "CRC Check Failed. Data may be corrupted.\n");
		 }
	}
	else
	{
		// Measurement is not complete yet
		sprintf(message_data, "Measurement not complete. Waiting...\n");
	}
}
