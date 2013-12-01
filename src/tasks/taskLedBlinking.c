/*
 * taskLedBlinking.c
 *
 *  Created on: 07-06-2012
 *      Author: Bartek
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "taskLedBlinking.h"


void vLedTask(void *pvParameters) {
	static int iState = 0;

	while (1) {

		if (iState == 0)
		{
			iState = 1;
			GPIO_SetValue(1,(0x01<<28));
		}
		else
		{
			iState = 0;
			GPIO_ClearValue(1, (0x01<<28));
		}

		vTaskDelay(10000/portTICK_RATE_MS);
	}
}


