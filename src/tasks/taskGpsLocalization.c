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

static uint8_t compare(char* s1,char* s2, uint8_t length);


void vGpsTask(void *pvParameters)
{
char mesageID [5];
char* idPtr;
uint8_t i = 0;
delay_ms(100);
//UART_Send(LPC_UART0, "$PMTK314,0,0,0,5,0,0,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
UART_Send(LPC_UART0, "$PMTK314,0,0,0,5,5,5,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
while(UART_CheckBusy(LPC_UART0));
NVIC_EnableIRQ(UART0_IRQn);

	while(1)
	{
		if(xSemaphoreTake(xSemaphoreGPS,portMAX_DELAY) == pdTRUE)
		{
			idPtr = getMessageIdPtr();
			idPtr++;
			for( ; *(idPtr)!=',' ; idPtr++)
			{
				mesageID[i++] = *idPtr;
			}
			mesageID[i]='\n';
			i=0;
		if( compare((char*)mesageID, "GPGGA", 5))
			{
				if(checkFixPresence()!=0)
				{
					parseGgaMessage();
				}
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

static uint8_t compare(char* s1,char* s2, uint8_t length)
{
	for(;length!=0;length--)
	{
		if(*s1==*s2)
		{
			s1++;
			s2++;
		}
		else
		{
			return 0;
		}
	}
return 1;
}
