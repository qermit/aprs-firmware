/*
 * powerMenagement.h
 *
 *  Created on: 23-05-2012
 *      Author: Bartek
 */

#ifndef POWERMENAGEMENT_H_
#define POWERMENAGEMENT_H_
#include "../stdPeriphLibs/lpc_types.h"

void powerSectionInit (void);
void chargerCmd( FunctionalState );
void setUsbCurrent( logicalState_t );

float getBatteryVoltage( void );
chargingStatus_t getChargerStatus( void );

#endif /* POWERMENAGEMENT_H_ */
