/*
 * gsmModule.c
 *
 *  Created on: 15-05-2012
 *      Author: Bartek
 */
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../boardConfiguration.h"
#include "../stdPeriphLibs/lpc17xx_rtc.h"
#include "commonFunctions.h"
#include "gsmModule.h"
#include "string.h"
#include <stdlib.h>

#define SERVER_IP 		"82.177.146.6"
#define CONNETION_TYPE 	"TCP"
#define PORT			"4444"

struct {
	uint16_t resultCode;
	char informationResponse[20];
	uint8_t responsePresent;
}ATV_responseData;

static LPC_UART_TypeDef *UARTx = (LPC_UART_TypeDef *)LPC_UART1;


static void UART_FullModemConfig(void);
static void GPIO_Config(void);
static uint32_t GSM_SerialSendString(uint8_t*  );
static void GSM_TerminateDataSending(void);
static void getServerResponse(void);
static void getAtvResponse( void );
static uint8_t * prepareServerConf(void);
static uint16_t GPRS_connectionReports(void);
static uint32_t buffLength(uint8_t* );
static uint32_t sendDataToServer(void);
static void prepareTxDataBuff(uint8_t measurementNr);
static void incrementPtr(void);
static void timeSynchronization(void);


static char GSM_RxBuff[200];
static char GSM_TxDataBuff[80];
static char TMP_BUFF[500];
static char GSM_TimeBuff[40];
static char GSM_ServerConf[40];
static uint32_t new = 0;
static uint32_t iter = 0;
static uint8_t * tmpPtr;
static uint8_t promptReceived=0;
static uint32_t bytes =0;
static volatile uint8_t nrOfMeasToSend=0;
//-------Server Response Flag
uint8_t recentServerResponseFlag = 0;
//==================


void GSM_Init(void)
{
	GPIO_Config();
	UART_FullModemConfig();

	prepareServerConf();

	//GSM_Switch(DISABLE);
	delay_ms(600);
	GSM_Switch(ENABLE);

	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);


	GSM_SendCommand("ATV0\r");
	GSM_SendCommand("ATE0\r");

}

static void GPIO_Config(void)
{
	PINSEL_CFG_Type PINSEL_ConfigStruct;
	//set gsm Status pin as input
	GPIO_SetDir(GPIO2, (0x01<<gsmStatPin), INPUT);
	//set gsm Power pin as output
	GPIO_SetDir(GPIO2, (0x01<<gsmPwrPin), OUTPUT);

	PINSEL_ConfigStruct.Funcnum = PINSEL_FUNC_0;
	PINSEL_ConfigStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
	PINSEL_ConfigStruct.Pinmode = PINSEL_PINMODE_PULLDOWN;
	PINSEL_ConfigStruct.Pinnum = gsmPwrPin;
	PINSEL_ConfigStruct.Portnum = GPIO2;
	PINSEL_ConfigPin(&PINSEL_ConfigStruct);

	PINSEL_ConfigStruct.Pinnum = gsmStatPin;
	PINSEL_ConfigPin(&PINSEL_ConfigStruct);

}

static void UART_FullModemConfig(void)
{
	PINSEL_CFG_Type PinSelectConfigStruct;
	UART_CFG_Type UARTConfigStruct;
	UART_FIFO_CFG_Type UARTFIFOConfigStruct;

	//GSM_RXD_PIN
	PinSelectConfigStruct.Funcnum = PINSEL_FUNC_2;
	PinSelectConfigStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinSelectConfigStruct.Pinmode = PINSEL_PINMODE_PULLUP;
	PinSelectConfigStruct.Pinnum = gsmRxdPin;
	PinSelectConfigStruct.Portnum = GPIO2;

	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_TXD_PIN
	PinSelectConfigStruct.Pinnum = gsmTxdPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_CTS_PIN
	PinSelectConfigStruct.Pinnum = gsmCtsPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_DCD_PIN
	PinSelectConfigStruct.Pinnum = gsmDcdPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_DTR_PIN
	PinSelectConfigStruct.Pinnum = gsmDtrPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_RI_PIN
	PinSelectConfigStruct.Pinnum = gsmRiPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);

	//GSM_RTS_PIN
	PinSelectConfigStruct.Pinnum = gsmRtsPin;
	PINSEL_ConfigPin(&PinSelectConfigStruct);


	/* Initialize UART Configuration parameter structure to default state:
	 * Baudrate = 9600bps
	 * 8 data bit
	 * 1 Stop bit
	 * None parity
	 */
	UART_ConfigStructInit(&UARTConfigStruct);

	/* Set Baudrate */
	UARTConfigStruct.Baud_rate = 9600;

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

	/* Initialize FIFO for UART1 peripheral */
	UART_FIFOConfig(UARTx, &UARTFIFOConfigStruct);

	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
	//UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_RTS, ACTIVE);

	/*  Enable UART Transmit */
	UART_TxCmd( UARTx, ENABLE);

	UART_IntConfig(UARTx, UART_INTCFG_RBR , ENABLE);
	//UART_IntConfig(UARTx, UART1_INTCFG_MS, ENABLE);
	UART_FullModemForcePinState(LPC_UART1, UART1_MODEM_PIN_DTR, ACTIVE);

}
//==========================================================================

FunctionalState GSM_GetStatus(void)
{
	if(GPIO_ReadValue(GPIO2)&0x200)
	{
		return ENABLE;
	}
	else
		return DISABLE;
}

void GSM_Switch(FunctionalState newState)
{

	if(GSM_GetStatus() == newState)
	{
		return;
	}
	else if(newState == ENABLE)
	{
		GPIO_SetValue(GPIO2, (0x01<<gsmPwrPin));
		while(!GSM_GetStatus());
		GPIO_ClearValue(GPIO2, (0x01<<gsmPwrPin));
		delay_ms(2000);
	}
	else if (newState == DISABLE)
	{
		while(GSM_GetStatus())
		{
			GPIO_SetValue(GPIO2, (0x01<<gsmPwrPin));
			delay_ms(800);
			GPIO_ClearValue(GPIO2, (0x01<<gsmPwrPin));
			delay_ms(10000);

		}

	}
}

static uint8_t * prepareServerConf(void)
{

	uint8_t * ptr = GSM_ServerConf;
	strcpy(ptr, "AT+QIOPEN=\"");
	ptr += 11;
	strcpy(ptr, CONNETION_TYPE );
	ptr += 3;
	strcpy(ptr, "\",\"" );
	ptr += 3;
	strcpy(ptr, SERVER_IP);
	ptr += buffLength(SERVER_IP);
	strcpy(ptr, "\"," );
	ptr += 2;
	strcpy(ptr, PORT);
	ptr += 4;
	strcpy(ptr, "\r" );
	*(++ptr) = '\0';
	ptr = GSM_ServerConf;

	return ptr;
}

void GSM_SendCommand(uint8_t * ptr)
{
	GSM_SerialSendString( ptr );
}

static uint32_t GSM_SerialSendString(uint8_t* ptr )
{
	uint32_t nrOfBytesSend;
	nrOfBytesSend = UART_Send(UARTx, ptr, buffLength(ptr), BLOCKING);
	while(UART_CheckBusy(UARTx));
	return nrOfBytesSend;
}

static uint32_t buffLength(uint8_t* ptr)
{
	uint32_t i=0;
	while(*ptr != '\0')
	{
		i++;
		ptr++;
	}
	return i;
}

void UART1_IRQHandler(void)
{
	char newData = (char)UART_ReceiveByte(UARTx);
	//TMP_BUFF[new++]=newData;

	if( (newData=='\r') & (iter!=0) )
	{
		GSM_RxBuff[iter] = '\r';
		getAtvResponse();
	}
	else if( (newData=='\n') & (iter!=0) )
	{
		GSM_RxBuff[iter]='\0';
		getServerResponse();
	}
	else if ( (newData=='>') & (iter == 0) )
	{
		promptReceived = 1;
	}
	else if( (newData != '\0') & (newData!='\360') & (newData!='\n') & (newData!='\r'))
	{
		GSM_RxBuff[iter++] = newData;
	}

NVIC_ClearPendingIRQ(UART1_IRQn);
}


/**
 *  recentServerResponse flag 0b0000 0000
 *  							   | ||||
 *  							   T CDSE
 *  C - continue
 *  D - devserver - v 1.0
 *  S - saved
 *  E - error
 *  T - time
 */
static void getServerResponse()
{
	recentServerResponseFlag = 0;

	if( !strcmp(GSM_RxBuff, "ERROR" ))
	{
		recentServerResponseFlag = 0x01;
	}
	else if ( !strcmp(GSM_RxBuff, "CONTINUE" ))
	{
		recentServerResponseFlag = 0x08;
	}
	else if ( !strcmp(GSM_RxBuff, "SAVED" ))
	{
		recentServerResponseFlag = 0x02;
	}
	else if ( !strcmp(GSM_RxBuff, "Devserver - version 1.0" ))
	{
		recentServerResponseFlag = 0x04;
	}
	else if (!strncmp(GSM_RxBuff, "<time>", 6 ))
	{
		recentServerResponseFlag = 0x10;
		strncpy(GSM_TimeBuff, GSM_RxBuff, 35);
	}
	iter = 0;
}

static void getAtvResponse(void)
{
	ATV_responseData.responsePresent = 1;

	if(iter==1)
	{
		uint8_t response = (uint8_t)(atoi(GSM_RxBuff));
		ATV_responseData.resultCode = (0x01<<response);
	}
	else
	{
		strncpy(ATV_responseData.informationResponse, GSM_RxBuff, iter);
		ATV_responseData.informationResponse[iter]='\0';
	}
	iter = 0;
}

void GSM_PrepareSMS(float tmp1, float tmp2)
{
	char data [] = {"Temperatura:   \nWilgotnosc:   "};
	uint8_t t1 = (uint8_t)tmp1;
	uint8_t t2 = (uint8_t)tmp2;
	uint8_t t3,t4;


	t3=t1%10;
	t1=t1/10;

	data[13] = (char)(48+t1);
	data[14] = (char)(48+t3);

	t4=t2%10;
	t2=t2/10;

	data[28] = (char)(48+t2);
	data[29] = (char)(48+t4);

	GSM_SendSMS("604218929", data);
}

void GSM_SendSMS(uint8_t* number, uint8_t* data)
{
	//NVIC_DisableIRQ(UART1_IRQn);
	GSM_SerialSendString("AT+CMGF=1\r");
	delay_ms(100);//while(!OKFlag);
	GSM_SerialSendString("at+CMGS=\"");
	GSM_SerialSendString(number);
	GSM_SerialSendString("\"\r");
	promptReceived=0;
	//while(!sendDataFlag);
	promptReceived=0;
	GSM_SerialSendString(data);
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x1A);
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x0D);
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x0A);
	NVIC_EnableIRQ(UART1_IRQn);
}

/**
 * Function establish connection with server
 * via GPRS service and send data
 */
uint8_t GSM_GPRSConnection(void)
{
	//=============DISCONNECT ALL
	GSM_SendCommand("AT+CGATT=0\r");
	GSM_SendCommand("AT+QICLOSE\r");
	delay_ms(5000);

	//============GPRS CONNECTION
	GSM_SendCommand("AT+CGDCONT=1,\"IP\",\"internet\",,0,0\r");
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	delay_ms(500);

	GSM_SendCommand("AT+CGATT=1\r");
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	delay_ms(500);
	//===========================

	//===========TCP/UDP connection
	recentServerResponseFlag = 0;
	GSM_SendCommand(GSM_ServerConf);
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	while ( !recentServerResponseFlag );
	if(recentServerResponseFlag == 0x01)
		{return 2;}
	//==============================

	//============SEND DEV_ID
	promptReceived = 0;
	GSM_SendCommand("AT+QISEND\r");
	while (!promptReceived);

	recentServerResponseFlag=0;
	GSM_SerialSendString("6382846480147182");
	GSM_TerminateDataSending();
	while(!recentServerResponseFlag);
	if(recentServerResponseFlag == 0x01)
		{return 2;}

	//===========SEND DATA
	promptReceived=0;
	GSM_SendCommand("AT+QISEND\r");
	while(!promptReceived);
	promptReceived=0;

	bytes = sendDataToServer();

	recentServerResponseFlag = 0;
	GSM_TerminateDataSending();
	while(!recentServerResponseFlag);
	if(recentServerResponseFlag == 0x01)
		{return 2;}

	//===========CLOSE CONNECTION
	GSM_SendCommand("AT+QICLOSE\r");
	GSM_SendCommand("AT+CGATT=0\r");

	return 0;
}

uint8_t GSM_requestTimeSynchro(void)
{
	//=============DISCONNECT ALL
	GSM_SendCommand("AT+CGATT=0\r");
	GSM_SendCommand("AT+QICLOSE\r");
	delay_ms(5000);

	//============GPRS CONNECTION
	GSM_SendCommand("AT+CGDCONT=1,\"IP\",\"internet\",,0,0\r");
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	delay_ms(500);

	GSM_SendCommand("AT+CGATT=1\r");
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	delay_ms(500);
	//===========================

	//===========TCP/UDP connection
	recentServerResponseFlag = 0;
	GSM_SendCommand(GSM_ServerConf);
	if(GPRS_connectionReports()!=0x01)
		{return 1;}
	while ( !recentServerResponseFlag );
	if(recentServerResponseFlag == 0x01)
		{return 2;}
	//==============================

	//============SEND DEV_ID
	promptReceived = 0;
	GSM_SendCommand("AT+QISEND\r");
	while (!promptReceived);

	recentServerResponseFlag=0;
	GSM_SerialSendString("6382846480147182");
	GSM_TerminateDataSending();
	while(!recentServerResponseFlag);
	if(recentServerResponseFlag == 0x01)
		{return 2;}

	//===========SEND TIME REQUEST
	promptReceived=0;
	GSM_SendCommand("AT+QISEND\r");
	while(!promptReceived);
	promptReceived=0;

	GSM_SerialSendString("time");
	recentServerResponseFlag = 0;
	GSM_TerminateDataSending();

	while(!recentServerResponseFlag);
	if(recentServerResponseFlag == 0x10)
	{
		timeSynchronization();
	}
	else
	{
		return 2;
	}

	//===========CLOSE CONNECTION
	GSM_SendCommand("AT+QICLOSE\r");
	GSM_SendCommand("AT+CGATT=0\r");

	return 0;
}

static uint32_t sendDataToServer(void)
{
	uint32_t nrOfByteSend = 0;
	uint8_t * txBuffPtr = GSM_TxDataBuff;
	uint8_t nrOfMeasurementTMP = 0;

	nrOfByteSend = GSM_SerialSendString("<request name=\"measurements\">");

	while(nrOfMeasurementTMP!=nrOfMeasToSend)
	{
		prepareTxDataBuff(nrOfMeasurementTMP++);
		nrOfByteSend += GSM_SerialSendString(txBuffPtr);
	}

	nrOfByteSend += GSM_SerialSendString("</request>");

	nrOfMeasToSend = 0;
	return nrOfByteSend;
}

static void prepareTxDataBuff(uint8_t measurementNr)
{
	tmpPtr = GSM_TxDataBuff;

	strcpy(tmpPtr, "<meas>");
	tmpPtr += 6;
	strcpy(tmpPtr, "<t>");
	tmpPtr+=3;
	strcpy(tmpPtr, measurements[measurementNr].valueType);
	incrementPtr();
	strcpy(tmpPtr, "</t>");
	tmpPtr += 4;
	strcpy(tmpPtr, "<ts>");
	tmpPtr += 4;
	strcpy(tmpPtr, measurements[measurementNr].timeStamp);
	incrementPtr();
	strcpy(tmpPtr, "</ts>");
	tmpPtr += 5;
	strcpy(tmpPtr, "<v>");
	tmpPtr += 3;
	if(strncmp(measurements[measurementNr].valueType, "GPS", 3))
	{
		sprintf(tmpPtr, "%.2f", measurements[measurementNr].value);
		incrementPtr();
	}
	else
	{
		//ToDo - GPS message
	}

	strcpy(tmpPtr, "</v></meas>\0");

}

static void incrementPtr(void)
{
	while(*tmpPtr!='\0')
	{
		tmpPtr++;
	}

}

//void debug()
//{
//	sendDataToServer(nrOfMeasToSend);
//}

static uint16_t GPRS_connectionReports(void)
{
	ATV_responseData.resultCode = 0;
	ATV_responseData.responsePresent = 0;
	while(!ATV_responseData.responsePresent);
	return ATV_responseData.resultCode;
}


/**
 * Function send via UART characters terminating
 * data sending to modem.
 */
static void GSM_TerminateDataSending(void)
{
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x1A);
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x0D);
	while(UART_CheckBusy(UARTx));
	UART_SendByte(UARTx, 0x0A);
	while(UART_CheckBusy(UARTx));
}

void GSM_saveDataToSend(float data, uint8_t* type)
{
	strcpy(measurements[nrOfMeasToSend].timeStamp, getRtcFullTime());
	measurements[nrOfMeasToSend].value = data;
	strcpy(measurements[nrOfMeasToSend++].valueType, type);
	nrOfMeasToSend %= MAX_MEASUREMENTS_TO_STORE+1;
}

static void timeSynchronization(void)
{
	uint8_t* ptr = GSM_TimeBuff;

	while((*ptr)!='>')
	{
		ptr++;
	}

	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, atoi((ptr+18)));
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MINUTE, atoi((ptr+15)));
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_HOUR, atoi((ptr+12)));
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, atoi((ptr+9)));
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MONTH, atoi((ptr+6)));
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_YEAR, atoi((ptr+1)));
}

uint32_t debug(void)
{
	return bytes;
}

