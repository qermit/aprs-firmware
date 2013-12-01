///*
// * measurementsTask.c
// *
// *  Created on: 29-01-2013
// *      Author: Bartek
// */
//
//#include "FreeRTOS.h"
//#include "task.h"
//#include "queue.h"
//#include "semphr.h"
//
//#include "modules.h"
//#include "../stdPeriphLibs/lpc17xx_gpio.h"
//#include "../stdPeriphLibs/lpc17xx_pinsel.h"
//#include "../stdPeriphLibs/lpc17xx_uart.h"
//#include "../stdPeriphLibs/lpc17xx_systick.h"
//#include "commonFunctions.h"
//#include <stdio.h>
//
//#include "tasks/taskLedBlinking.h"
//#include "tasks/taskGpsLocalization.h"
//#include "tasks/measurementsTask.h"
//#include "tasks/idleTask.h"
//
//
//
//void vMeasTask(void *pvParameters)
//{
//	while(1)
//	{
//		//getSHT21Hum();
//		getSHT21Temp();
//
//		vTaskDelay(1000/portTICK_RATE_MS);
//	}
//
//}
