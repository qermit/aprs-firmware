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
#include "gpsModule/gpsModule.h"
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "taskGpsLocalization.h"





void vGpsTask(void *pvParameters)
{
	delay_ms(100);

	GpsInitialization();

	//UART_Send(LPC_UART0, "$PMTK314,0,0,0,5,0,0,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
	//UART_Send(LPC_UART0,(uint8_t *) "$PMTK314,0,0,0,5,5,5,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
	UART_Send(LPC_UART0,(uint8_t *) "$PMTK314,1,1,1,1,1,5,0,0,0,0,0,0,0,0,0,0,0,0,0*2C\r\n",52, BLOCKING);

	//UART_Send(LPC_UART0,(uint8_t *) "$PMTK000*32\r\n",13, BLOCKING);
	UART_Send(LPC_UART0,(uint8_t *) "$PMTK101*32\r\n",13, BLOCKING);

	while(UART_CheckBusy(LPC_UART0));
	NVIC_EnableIRQ(UART0_IRQn);


	while(1)
	{
		gpsSetProcessingBuffer(-1);
		if(xSemaphoreTake(xSemaphoreGPS,portMAX_DELAY) == pdTRUE)
		{
			gpsSetProcessingBuffer(0);
			gps_tokenize_input();
			char *token_msgid = gps_getToken(0);

			if( strcmp(token_msgid, "$GPGGA") == 0)
			{
				//$GPGGA,081604.000,5204.8543,N,02101.6233,E,1,8,1.06,96.7,M,39.3,M,,*66
				//$GPGGA,082316.000,5204.8526,N,02101.6227,E,1,8,1.04,93.9,M,39.3,M,,*6C
				//$GPGGA,083352.000,5205.1129,N,02102.3054,E,2,9,0.99,107.4,M,39.3,M,0000,0000*58
				asm("nop");

	//			if(checkFixPresence()!=0)
	//			{
	//				parseGgaMessage();
	//			}
			} else if (strcmp(token_msgid, "$GPRMC")==0) {
				//$GPRMC,081253.000,A,5204.8527,N,02101.6226,E,0.04,0.00,210214,,,A*6A
				//$GPRMC,082223.000,A,5204.8527,N,02101.6234,E,0.33,175.46,210214,,,A*68

				char *test = gps_getToken(2);
				if(test[0] == 'V') {
					asm("nop");
				} else if (test[0] == 'A') {
					asm("nop");
				}
			}	else if (strncmp(token_msgid, "GPGSA", 5)==0) {
				// GSA - SATELITES AVAILVLE
				//$GPGSA,A,3,20,31,17,11,01,,,,,,,,1.80,1.54,0.93*06
				//$GPGSA,A,3,20,31D*0C\r\n

				asm("nop");
			} else if (strncmp(token_msgid, "GPVTG", 5) == 0) {
				//$GPVTG,71.41,T,,M,0.06,N,0.10,K,A*09\r\n
				// 10 km/h
				//$GPVTG,70.02,T,,M,8.04,N,14.90,K,A*38\r\n
				// 25 km/h
				//$GPVTG,248.73,T,,M,12.53,N,23.23,K,D*37
				asm("nop");
			} else if (strncmp(token_msgid, "GPGSV", 5) == 0) {
				// GSV - Satelistes in view
				// $GPGSV,4,1,13,20,76,279,46,01,62,169,45,32,61,082,44,23,40,210,48*7F
				// $GPGSV,2,1,08,20,71,276,26,01,67,172,40,11,37,173,31,31,32,091,*79

				char *test = getFieldPtr(3);
				if (strncmp(test, "00", 2)!= 0){
					asm("nop");
				}
			} else if (strncmp(token_msgid, "PMTK011",7)==0) {
				asm("nop");
			} else if (strncmp(token_msgid, "PMTK010", 7)==0) {
				asm("nop");
			} else {
				asm("nop");
			}
		}
		//	vTaskDelay(1000/portTICK_RATE_MS);
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
