/*
 * gsmModule.h
 *
 *  Created on: 15-05-2012
 *      Author: Bartek
 */

#include "../stdPeriphLibs/lpc_types.h"


#ifndef GSMMODULE_H_
#define GSMMODULE_H_

#define MAX_MEASUREMENTS_TO_STORE 20

typedef struct
{
	char timeStamp[25];
	float value;
	char valueType[6];
}GSM_dataToSend;

GSM_dataToSend measurements [MAX_MEASUREMENTS_TO_STORE];

//volatile uint8_t sendsms = 0;

void GSM_Init(void);
void GSM_Switch(FunctionalState );
void GSM_SendCommand(uint8_t *);
void GSM_SendSMS(uint8_t*, uint8_t*);
uint8_t GSM_GPRSConnection(void);
void GSM_PrepareSMS(float tmp1, float tmp2);
void GSM_saveDataToSend(float data, uint8_t* type);
uint8_t GSM_requestTimeSynchro(void);
uint32_t debug(void);

FunctionalState GSM_GetStatus(void);

void UART1_IRQHandler(void);

#endif /* GSMMODULE_H_ */
