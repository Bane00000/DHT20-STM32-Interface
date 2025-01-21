/*
 * stm32f407xx_timer_driver.h
 *
 *  Created on: Dec 23, 2024
 *      Author: Bane
 */

#ifndef INC_STM32F407XX_TIMER_DRIVER_H_
#define INC_STM32F407XX_TIMER_DRIVER_H_

#include "stm32f407xx.h"

void TIM_Config(void);

void Delay_us(uint16_t us);

void Delay_ms(uint16_t ms);

#endif /* INC_STM32F407XX_TIMER_DRIVER_H_ */
