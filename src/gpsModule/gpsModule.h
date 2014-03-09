/*
 * gpsModule.h
 *
 *  Created on: 11-05-2012
 *      Author: Bartek
 */

#ifndef GPSMODULE_H_
#define GPSMODULE_H_

#include "../stdPeriphLibs/lpc_types.h"
#include "stdint.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

/* Defines	*/
#define NMEA_MESSAGE_MAX_LENGTH 100
#define MAX_TOKEN_TABLE_COUNT 20





void GpsInitialization(void);

char* getFieldPtr(uint8_t fieldNr);
char * gps_getToken(uint8_t i);
void gps_tokenize_input();
int gps_tokens();

void UART0_IRQHandler(void);
void setGpsQueue(xQueueHandle queue_id);

uint8_t checkFixPresence(void);
char* getMessageIdPtr(void);
char* getGpsPosition(void);
char* getGpsTime(void);

int8_t gpsProcessingBuffer();
void gpsSetProcessingBuffer(int8_t i);

/* Private functions */


#endif /* GPSMODULE_H_ */
