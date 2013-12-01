/*
 * realTimeClock.h
 *
 *  Created on: 22-10-2012
 *      Author: Bartek
 */

#include "../stdPeriphLibs/lpc_types.h"

#ifndef REALTIMECLOCK_H_
#define REALTIMECLOCK_H_

void initRTC(void);
uint8_t * getRtcFullTime(void);

#endif /* REALTIMECLOCK_H_ */
