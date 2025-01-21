/*
 * dht20_driver.h
 *
 *  Created on: Dec 23, 2024
 *      Author: Bane
 */

#ifndef INC_DHT20_DRIVER_H_
#define INC_DHT20_DRIVER_H_

#include "stm32f407xx.h"

/*
 * DHT20 commands and defines
 */
#define DHT20_ADDRESS				0x38

/*
 * Functions and structure declarations
 */

/*
 * Calculates temperature with the formula provided in datasheet
 */

void DHT20_Init(void);

void I2C1_GPIOInits(void);

void I2C1_Inits(void);


#endif /* INC_DHT20_DRIVER_H_ */
