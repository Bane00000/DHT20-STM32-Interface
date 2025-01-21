/*
 * main.c
 *
 *  Created on: Dec 23, 2024
 *      Author: Bane
 */

#include <stdint.h>
#include "stm32f407xx.h"
#include "dht20_driver.h"

/*
 * PB6-> SCL
 * PB7-> SDA
 */

void GPIO_ButtonInit(void)
{
	GPIO_Handle_t GPIOBtn;

	GPIOBtn.pGPIOx = GPIOA;
	GPIOBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_0;
	GPIOBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GPIOBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GPIOBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GPIOBtn);

}

// https://www.youtube.com/watch?v=HBjws896hPA&t=1015s
int main(void)
{
	// Wait 100ms
	Delay_ms(4000);

	// GPIO button pin init
	GPIO_ButtonInit();

	// I2C pin inits
	I2C1_GPIOInits();

	// I2C init
	I2C1_Inits();

	I2C_PeripheralControl(I2C1, ENABLE);

	// ACK bit is = 1 after PE = 1
	I2C_ManageAcking(I2C1, I2C_ACK_ENABLE);

	Delay_ms(160);

	while(1)
	{
		//wait till button is pressed
		while(!GPIO_ReadFromInputPin(GPIOA, GPIO_PIN_NO_0));
		Delay_ms(10);

		DHT20_Init();
	}
}

