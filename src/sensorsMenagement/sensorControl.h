/*
 * sensorControl.h
 *
 *  Created on: 01-09-2012
 *      Author: Bartek
 */
#include "../stdPeriphLibs/lpc_types.h"

#ifndef SENSORCONTROL_H_
#define SENSORCONTROL_H_

struct sensorDetails
{
	char id[5];
	uint8_t channelNr;
};

void sensorControlInit(void);
void setActiveChannel(uint8_t channelNr);
void sensorConnectionCmd(FunctionalState newState);

#endif /* SENSORCONTROL_H_ */
