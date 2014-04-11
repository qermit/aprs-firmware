/*
 * gpsModule.c
 *
 *  Created on: 11-05-2012
 *      Author: Bartek
 */

/* Includes         */
#include "gpsModule.h"
#include "../boardConfiguration.h"
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../tasks/taskGpsLocalization.h"
#include "commonFunctions.h"
#include "string.h"
#include <stdlib.h>
#include <FreeRTOS.h>
#include <../FreeRTOS_include/task.h>
#include <../FreeRTOS_include/semphr.h>
#include <../FreeRTOS_include/queue.h>


/* Defines 			*/




/* Private variables */
//static const uint8_t nmeaMessageMaxLength = NMEA_MESSAGE_MAX_LENGTH;
static LPC_UART_TypeDef *UARTx = (LPC_UART_TypeDef *)LPC_UART0;

static volatile uint8_t gpsDataReadyFlag = 0;
static volatile uint8_t dataCounter = 0;
static volatile uint8_t receivingGpsFrame = FALSE;

static char gpsRxBuffer [ NMEA_MESSAGE_MAX_LENGTH ];



/* Private function prototypes -----------------------------------------------*/
static void UART_Config(void);


volatile xQueueHandle uart0_queue_id;
void gpsSetQueueID(xQueueHandle gpsQueue) {
	uart0_queue_id = gpsQueue;
}


void GpsInitialization(void)
{
	GPIO_SetDir(GPIO0, 0x01<<gpsEnPin, OUTPUT);
	UART_Config();

}

static void UART_Config(void)
{
	/* UART Configuration structure variable */
	UART_CFG_Type UARTConfigStruct;
	/* UART FIFO configuration structure variable */
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;
	/* Pin configuration for UART0 */
	PINSEL_CFG_Type PinCfg;


		/*
		 * Initialize UART0 pin connect
		 */
		PinCfg.Funcnum = PINSEL_FUNC_1;
		PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
		PinCfg.Pinmode = PINSEL_PINMODE_PULLUP;
		PinCfg.Pinnum = gpsTxdPin;
		PinCfg.Portnum = GPIO0;
		PINSEL_ConfigPin(&PinCfg);
		PinCfg.Pinnum = gpsRxdPin;
		PINSEL_ConfigPin(&PinCfg);


	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	/* Set Baudrate */
	UARTConfigStruct.Baud_rate = GPS_UARTBaudRate;

	/* Initialize UART0 peripheral with given to corresponding parameter */
	UART_Init(UARTx, &UARTConfigStruct);

	/* Initialize FIFOConfigStruct to default state:
	 * 				- FIFO_DMAMode = DISABLE
	 * 				- FIFO_Level = UART_FIFO_TRGLEV0
	 * 				- FIFO_ResetRxBuf = ENABLE
	 * 				- FIFO_ResetTxBuf = ENABLE
	 * 				- FIFO_State = ENABLE
	 */
	UART_FIFOConfigStructInit(&UARTFIFOConfigStruct);

	/* Initialize FIFO for UART0 peripheral */
	UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

	/*  Enable UART Transmit */
	UART_TxCmd( UARTx, ENABLE);

	UART_IntConfig(UARTx, UART_INTCFG_RBR , ENABLE);

	//NVIC_EnableIRQ(UART0_IRQn);

	//GPS enable
	delay_ms(20);
	GPIO_SetValue(GPIO0, 0x01<<gpsEnPin);
	delay_ms(500);
//	UART_Send(UARTx, "$PMTK314,0,0,0,5,0,0,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
//	while(UART_CheckBusy(UARTx));

}


volatile portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

void UART0_IRQHandler(void)
{
	char newData;

	xHigherPriorityTaskWoken = pdFALSE;
	newData = (char)UART_ReceiveByte(UARTx);
	xQueueSendFromISR( uart0_queue_id, &newData, &xHigherPriorityTaskWoken );
	portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
	//taskYIELD();
    NVIC_ClearPendingIRQ(UART0_IRQn);

	//NVIC_ClearPendingIRQ(UART0_IRQn);
//
//	if (processing_buffer== -1) {
//		if(newData == '$')
//		{
//			receivingGpsFrame = TRUE;
//			gpsRxBuffer[dataCounter++] = newData;
//		}
//		else if ( (receivingGpsFrame) &&  (dataCounter <  NMEA_MESSAGE_MAX_LENGTH) )
//		{
//			if(newData != '\n')
//			{
//				gpsRxBuffer[dataCounter++] = newData;
//			}
//			else
//			{
//				gpsRxBuffer[dataCounter++] = newData;
//				dataCounter = 0;
//				//static portBASE_TYPE xHigherPriorityTaskWoken;
//				//xHigherPriorityTaskWoken = pdFALSE;
//				//xSemaphoreGiveFromISR(xSemaphoreGPS, &xHigherPriorityTaskWoken);
//				xSemaphoreGive(xSemaphoreGPS);
//				receivingGpsFrame = FALSE;
//			}
//		}
//		else if ( (receivingGpsFrame) && (dataCounter >= NMEA_MESSAGE_MAX_LENGTH) )
//		{
//			dataCounter = 0;
//			receivingGpsFrame = FALSE;
//		}
//	}
}


char* getGpsPosition(void)
{
	char position [ 10 ];
	char *wsk;

	wsk = position;
	gpsDataReadyFlag = 0;
	UART_Send(UARTx, (uint8_t*)"$GPRMC", sizeof("$GPRMC"), NONE_BLOCKING);
	//while(!gpsDataReadyFlag);


	return wsk;
}



char* getMessageIdPtr(void)
{
	return getFieldPtr(0);
}

/**
 *  param:		fieldNr
 *  return:		pointer to first field component;
 */
char* getFieldPtr(uint8_t fieldNr)
{
	uint8_t comaCount = 0;

	char * ptr = gpsRxBuffer;

	while(comaCount != fieldNr)
	{
		if(*(ptr++) == ',')
		{
			comaCount++;
		}
	}



	return ptr;
}

