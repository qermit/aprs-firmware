/*
 * DS18B20.h
 *
 *  Created on: 04-09-2012
 *      Author: Bartek
 */
#include "../stdPeriphLibs/lpc_types.h"


#ifndef DS18B20_H_
#define DS18B20_H_

void DS18B20_Init(void);
uint8_t DS18B20_Reset(void);
float DS18B20_GetTemperature(void);


#endif /* DS18B20_H_ */
