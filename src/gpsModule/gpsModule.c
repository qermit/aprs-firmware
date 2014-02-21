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
#define GPS_UARTBaudRate		9600
#define NMEA_MESSAGE_MAX_LENGTH 100



/* Private variables */
//static const uint8_t nmeaMessageMaxLength = NMEA_MESSAGE_MAX_LENGTH;
static LPC_UART_TypeDef *UARTx = (LPC_UART_TypeDef *)LPC_UART0;

static volatile uint8_t gpsDataReadyFlag = 0;
static volatile uint8_t dataCounter = 0;
static volatile uint8_t receivingGpsFrame = FALSE;

static char gpsRxBuffer [ NMEA_MESSAGE_MAX_LENGTH ];



/* Private function prototypes -----------------------------------------------*/
static void UART_Config(void);


#define MAX_TOKEN_TABLE_COUNT 20
static char *token_table[MAX_TOKEN_TABLE_COUNT];
static int token_count = 0;




void GpsInitialization(void)
{
	GPIO_SetDir(GPIO0, 0x01<<gpsEnPin, OUTPUT);
	UART_Config();

	token_count = 0;
	//memset(token_table,0,);
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
	UART_Send(UARTx, "$PMTK314,0,0,0,5,0,0,0,0,1,1,1,1,1,1,1,1,1*2C\r\n", 47, BLOCKING );
	while(UART_CheckBusy(UARTx));

}


char * gps_getToken(uint8_t i) {
	if (i>=token_count) {
		return 0;
	} else {
		return token_table[i];
	}
}
static int8_t processing_buffer = -1;
int8_t gpsProcessingBuffer(){
	return processing_buffer;
}

void gpsSetProcessingBuffer(int8_t i){
	processing_buffer = i;
}

void gps_tokenize_input() {
	char * ptr = getMessageIdPtr();
	token_count = 1;
	token_table[0] = ptr;
	int counter = 0;
	while(*ptr != '*' && *ptr != '\r' && counter < NMEA_MESSAGE_MAX_LENGTH){
		if (*ptr == ',') {
			*ptr = 0;
			if (token_count >= MAX_TOKEN_TABLE_COUNT) {
				break;
			} else {
				token_table[token_count]=ptr+1;
			}
			token_count++;
		}
		ptr++;
		counter++;
	}
	*ptr=0;
}

int gps_tokens(){
	return token_count;
}



void UART0_IRQHandler(void)
{
	char newData;


	newData = (char)UART_ReceiveByte(UARTx);

	if (processing_buffer== -1) {
		if(newData == '$')
		{
			receivingGpsFrame = TRUE;
			gpsRxBuffer[dataCounter++] = newData;
		}
		else if ( (receivingGpsFrame) &&  (dataCounter <  NMEA_MESSAGE_MAX_LENGTH) )
		{
			if(newData != '\n')
			{
				gpsRxBuffer[dataCounter++] = newData;
			}
			else
			{
				gpsRxBuffer[dataCounter++] = newData;
				dataCounter = 0;
				//static portBASE_TYPE xHigherPriorityTaskWoken;
				//xHigherPriorityTaskWoken = pdFALSE;
				//xSemaphoreGiveFromISR(xSemaphoreGPS, &xHigherPriorityTaskWoken);
				xSemaphoreGive(xSemaphoreGPS);
				receivingGpsFrame = FALSE;
			}
		}
		else if ( (receivingGpsFrame) && (dataCounter >= NMEA_MESSAGE_MAX_LENGTH) )
		{
			dataCounter = 0;
			receivingGpsFrame = FALSE;
		}
	}
	NVIC_ClearPendingIRQ(UART0_IRQn);
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



void parseGgaMessage(void)
{

	memcpy((char*)lastReceivedGgaMessage.utcTime, getFieldPtr(1), 9*sizeof(char) );
	//lastReceivedGgaMessage.latitude = strtod(getFieldPtr(2), NULL);
	lastReceivedGgaMessage.latitudeIndicator = *(getFieldPtr(3));
	//lastReceivedGgaMessage.longitude = strtod(getFieldPtr(4), NULL);
	lastReceivedGgaMessage.longitudeIndicator = *(getFieldPtr(5));
	lastReceivedGgaMessage.positionFixStatusIndicator = (uint8_t)atoi(getFieldPtr(6));
	lastReceivedGgaMessage.satellitesUsed = (uint8_t)atoi(getFieldPtr(7));
	//lastReceivedGgaMessage.horizontalDilutionOfPrecision = strtod(getFieldPtr(8),NULL);
	//lastReceivedGgaMessage.mslAltitude = strtod(getFieldPtr(9), NULL);
	lastReceivedGgaMessage.mslUnits = *(getFieldPtr(10));
	//lastReceivedGgaMessage.geoidSeparation = strtod(getFieldPtr(11), NULL);
	lastReceivedGgaMessage.geoidSeparationUnits = *(getFieldPtr(12));
	lastReceivedGgaMessage.diffStation = (uint8_t)atoi(getFieldPtr(14));

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

uint8_t checkFixPresence(void)
{
	uint8_t fix = (uint8_t)atoi(getFieldPtr(6));
	return fix;
}

