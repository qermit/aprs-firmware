/*
 * commonFunctions.h
 *
 *  Created on: 29-07-2012
 *      Author: Bartek
 */

#include "../stdPeriphLibs/lpc_types.h"

#ifndef COMMONFUNCTIONS_H_
#define COMMONFUNCTIONS_H_



void delay_ms(uint32_t time);
void delay_us(uint32_t time);
void TIMER1_IRQHandler (void);
void sendFloat(float );

#endif /* COMMONFUNCTIONS_H_ */
