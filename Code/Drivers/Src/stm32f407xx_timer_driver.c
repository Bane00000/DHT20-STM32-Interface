/*
 * stm32f407xx_timer_driver.c
 *
 *  Created on: Dec 23, 2024
 *      Author: Bane
 */

#include "stm32f407xx_timer_driver.h"

void TIM_Config(void)
{
	// Enable TIM9 Clock
	RCC->APB2ENR |= (1u << 16);

	TIM9->PSC = 84-1; // One microsecond
	TIM9->ARR = 0xFFFF;

	// Enable counter
	TIM9->CR1 |= (1u << 0);

	//while(!(TIM9->SR & (1<<0)));
}

void Delay_us(uint16_t us)
{
	TIM_Config();

	// Reset counter to 0
	TIM9->CNT = 0;

	while(TIM9->CNT < us);
}

void Delay_ms(uint16_t ms)
{
	for(uint16_t i=0; i<ms; i++)
	{
		Delay_us(1000); // Delay of 1ms (1000us)
	}
}
