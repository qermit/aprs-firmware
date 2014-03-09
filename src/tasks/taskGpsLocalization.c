/*
 * taskGpsLocalization.c
 *
 *  Created on: 19-07-2012
 *      Author: Bartek
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "commonFunctions.h"
#include <string.h>
#include <stdlib.h>
#include "gpsModule/gpsModule.h"
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "taskGpsLocalization.h"



volatile NmeaGgaMessage_T lastReceivedGgaMessage;



volatile xQueueHandle gpsQueue;
char *token_table[MAX_TOKEN_TABLE_COUNT]; /// Tabela tokenow
char recieve_buffer[NMEA_MESSAGE_MAX_LENGTH];
uint8_t recieve_buffer_pos = 0;
uint8_t is_nmea_message = 0;
uint8_t token_count = 0;


void parseGgaMessage(void)
{

	strcpy((char*)lastReceivedGgaMessage.utcTime, token_table[1]);
	lastReceivedGgaMessage.latitude = strtod(token_table[2], NULL);
	lastReceivedGgaMessage.latitudeIndicator = *(token_table[3]);
	lastReceivedGgaMessage.longitude = strtod(token_table[4], NULL);
	lastReceivedGgaMessage.longitudeIndicator = *(token_table[5]);
	lastReceivedGgaMessage.positionFixStatusIndicator = (uint8_t)atoi(token_table[6]);
	lastReceivedGgaMessage.satellitesUsed = (uint8_t)atoi(token_table[7]);
	lastReceivedGgaMessage.horizontalDilutionOfPrecision = strtod(token_table[8],NULL);
	lastReceivedGgaMessage.mslAltitude = strtod(token_table[9], NULL);
	lastReceivedGgaMessage.mslUnits = *(token_table[10]);
	lastReceivedGgaMessage.geoidSeparation = strtod(token_table[11], NULL);
	lastReceivedGgaMessage.geoidSeparationUnits = *(token_table[12]);
	lastReceivedGgaMessage.diffStation = (uint8_t)atoi(token_table[14]);
}



void vGpsTask(void *pvParameters)
{
	delay_ms(100);
	gpsQueue = xQueueCreate(NMEA_MESSAGE_MAX_LENGTH, sizeof(char));
	if (gpsQueue == 0) {
		return;
	}
	gpsSetQueueID(gpsQueue);
	GpsInitialization();

	//UART_Send(LPC_UART0, "$PMTK314,0,0,0,5,0,0,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
	//UART_Send(LPC_UART0,(uint8_t *) "$PMTK314,0,0,0,5,5,5,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
	UART_Send(LPC_UART0,(uint8_t *) "$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*2C\r\n",52, BLOCKING);

	//UART_Send(LPC_UART0,(uint8_t *) "$PMTK000*32\r\n",13, BLOCKING);
	UART_Send(LPC_UART0,(uint8_t *) "$PMTK101*32\r\n",13, BLOCKING);

	while(UART_CheckBusy(LPC_UART0));
	NVIC_EnableIRQ(UART0_IRQn);


	char new_byte;

	while(1)
	{
		if (xQueueReceive(gpsQueue,&new_byte,10) == pdTRUE) {
			if (new_byte == '$') {
				is_nmea_message = 1;
				token_count = 1;
				recieve_buffer[0] = '$';
				recieve_buffer_pos = 1;
				token_table[0] = recieve_buffer;
				continue;
			}
			if (is_nmea_message == 0) continue;

			if(new_byte == '\n' || new_byte == '\r' ||
					new_byte == '*' || new_byte == ',') {
				recieve_buffer[recieve_buffer_pos++]=0;
				token_table[token_count++] = &(recieve_buffer[recieve_buffer_pos]);
			} else {
				recieve_buffer[recieve_buffer_pos++]=new_byte;
			}
			if(new_byte == '\r') {
				is_nmea_message = 0;

				if( strcmp(token_table[0], "$GPGGA") == 0)
				{
					//$GPGGA,081604.000,5204.8543,N,02101.6233,E,1,8,1.06,96.7,M,39.3,M,,*66
					//$GPGGA,082316.000,5204.8526,N,02101.6227,E,1,8,1.04,93.9,M,39.3,M,,*6C
					//$GPGGA,083352.000,5205.1129,N,02102.3054,E,2,9,0.99,107.4,M,39.3,M,0000,0000*58
					//*check fix presence
					if (atoi(token_table[6]) != 0) {
						parseGgaMessage();
					}
					asm("nop");

					//			if(checkFixPresence()!=0)
					//			{
					//				parseGgaMessage();
					//			}
				} else if (strcmp(token_table[0], "$GPRMC")==0) {
					//$GPRMC,081253.000,A,5204.8527,N,02101.6226,E,0.04,0.00,210214,,,A*6A
					//$GPRMC,082223.000,A,5204.8527,N,02101.6234,E,0.33,175.46,210214,,,A*68

					char *test = token_table[2];
					if(test[0] == 'V') {
						asm("nop");
					} else if (test[0] == 'A') {
						asm("nop");
					}
				}	else if (strcmp(token_table[0], "$GPGSA")==0) {
					// GSA - SATELITES AVAILVLE
					//$GPGSA,A,3,20,31,17,11,01,,,,,,,,1.80,1.54,0.93*06
					//$GPGSA,A,3,20,31D*0C\r\n

					asm("nop");
				} else if (strcmp(token_table[0], "$GPVTG") == 0) {
					//$GPVTG,71.41,T,,M,0.06,N,0.10,K,A*09\r\n
					// 10 km/h
					//$GPVTG,70.02,T,,M,8.04,N,14.90,K,A*38\r\n
					// 25 km/h
					//$GPVTG,248.73,T,,M,12.53,N,23.23,K,D*37
					asm("nop");
				} else if (strcmp(token_table[0], "$GPGSV") == 0) {
					// GSV - Satelistes in view
					// $GPGSV,4,1,13,20,76,279,46,01,62,169,45,32,61,082,44,23,40,210,48*7F
					// $GPGSV,2,1,08,20,71,276,26,01,67,172,40,11,37,173,31,31,32,091,*79

					char *test = token_table[3];
					if (strncmp(test, "00", 2)!= 0){
						asm("nop");
					}
				} else if (strcmp(token_table[0], "$PMTK011")==0) {
					asm("nop");
				} else if (strcmp(token_table[0], "$PMTK010")==0) {
					asm("nop");
				} else {
					asm("nop");
				}

				asm("nop");

			}




			asm("nop");
		}

		continue;
	}
}

void vGpsTest(void *pvParameters)
{

	while(1)
	{

		char a[] = "$dupa.\n";
//
//		uint8_t *ptr = a;
//
//		UART_Send(LPC_UART1,ptr,strlen(a),BLOCKING );
		vTaskDelay(1000/portTICK_RATE_MS);
	}

}
