/**
 * Name        : main.c
 * Version     :
 * Description : main definition for FreeRTOS application
 */

/*
 * FreeRTOS includes
 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "modules.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "../stdPeriphLibs/lpc17xx_systick.h"
#include "commonFunctions.h"
#include <stdio.h>

#include "tasks/taskLedBlinking.h"
#include "tasks/taskGpsLocalization.h"
#include "tasks/measurementsTask.h"
#include "tasks/idleTask.h"
#include "dac/dac.h"


#define USE_MODULE_GPS

#define USERTASK_STACK_SIZE configMINIMAL_STACK_SIZE

void __error__(char *pcFilename, unsigned long ulLine) {
}

#define DMA_SIZE                1



static void allPinsAsOut(void)
{
	GPIO_SetDir(0, 0x0FFFFFFFF, 1);
	GPIO_SetDir(1, 0x0FFFFFFFF, 1);
	GPIO_SetDir(3, 0x0FFFFFFFF, 1);
	GPIO_SetDir(4, 0x0FFFFFFFF, 1);
}

static void setupHardware(void) {
	SystemInit();
	SystemCoreClockUpdate();//CLKPWR_ConfigPPWR()
	//allPinsAsOut();
	//delay_ms(500);
	powerSectionInit();

#ifdef USE_MODULE_GSM
	GSM_Init();
#endif

	sensorControlInit();
	setActiveChannel(1);
#ifdef USE_MODULE_SENSORS
	//DS18B20_Init();
	initSHT21();
#endif

	//debugLed();
	//displayInitialization();
	//qtouchKeyboardInitialization();

#ifdef USE_MODULE_RTC
	initRTC();
#endif
}


/**
 * Simple task that increments a counter
 */
void vUserTask2(void *pvParameters) {
	int count = 0;
	while (1) {
		count++;
		taskENTER_CRITICAL();
		getSHT21Hum();
		taskEXIT_CRITICAL();
		vTaskDelay(101/portTICK_RATE_MS) ;
	}
}

void vMeasTask(void *pvParameters)
{
	while(1)
	{
	//	getSHT21Hum();
	//	getSHT21Temp();

		vTaskDelay(1000/portTICK_RATE_MS);
	}

}



/**
 * Program entry point 
 */
int main(void) {


	setupHardware();
	delay_ms(1000);

	vSemaphoreCreateBinary(xSemaphoreGPS);
	xSemaphoreTake(xSemaphoreGPS,0);

#ifdef USE_MODULE_GPS
	xTaskCreate( vGpsTask, ( signed portCHAR * ) "gpsTask", USERTASK_STACK_SIZE, NULL, 3, &xGpsTaskHandle );
#endif

	// To tak naprawde jest task od DACa
	xTaskCreate( vLedTask, ( signed portCHAR * ) "Led", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	//	/*
	//	 * Start the tasks defined
	//	 */
	//
	//
	//	xTaskCreate( vMeasTask, ( signed portCHAR * ) "measTask", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY+2, NULL );
	//
	//
	//	/*
	//	 * Start the scheduler.
	//	 */
	//	vTaskStartScheduler();
	//
	//	/*
	//	 * Will only get here if there was insufficient memory to create the idle task.
	//	 */



	//		uint8_t i = 10;
	//		uint8_t returnVal;
	//		uint32_t bytes;
	//		while( GSM_requestTimeSynchro() );
	//
	//		while(1)
	//		{
	//			getRtcFullTime();
	//
	//			for(i=10; i!=0; i--)
	//			{
	//				getSHT21Hum();
	//				getSHT21Temp();
	//				delay_ms(30000);
	//			}
	//
	//			returnVal = 1;
	//
	//			for(i=0;((i<10)&(returnVal!=0)); i++)
	//			{
	//				returnVal = GSM_GPRSConnection();
	//			}
	//			bytes = debug();
	//		}




	/*
	 * Start the tasks defined within this file/specific to this demo.
	 */
	//	xTaskCreate( vMeasTask, ( signed portCHAR * ) "measure", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	//	xTaskCreate( vUserTask2, ( signed portCHAR * ) "Task2", USERTASK_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );

	/*
	 * Start the scheduler.
	 */

	vTaskStartScheduler();

	/*
	 * Will only get here if there was insufficient memory to create the idle task.
	 */
	while(1)
	{

	}
	return 1;
}
