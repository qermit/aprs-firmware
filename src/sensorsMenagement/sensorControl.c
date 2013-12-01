/*
 * sensorControl.c
 *
 *  Created on: 01-09-2012
 *      Author: Bartek
 */

#include "sensorControl.h"
#include "boardConfiguration.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"

static void ctrlPinsInit(void);

void sensorControlInit(void)
{
	ctrlPinsInit();
}

static void ctrlPinsInit(void)
{
	GPIO_SetDir(GPIO1, 0x01C0000, OUTPUT);
}

void setActiveChannel(uint8_t channelNr)
{
	if(channelNr==1)
	{
		GPIO_ClearValue(GPIO1, 0x01<<muxS0Pin);
		GPIO_ClearValue(GPIO1, 0x01<<muxS1Pin);
	}
	if(channelNr==2)
	{
		GPIO_SetValue(GPIO1, 0x01<<muxS0Pin);
		GPIO_ClearValue(GPIO1, 0x01<<muxS1Pin);
	}
	if(channelNr==3)
	{
		GPIO_ClearValue(GPIO1, 0x01<<muxS0Pin);
		GPIO_SetValue(GPIO1, 0x01<<muxS1Pin);
	}
	sensorConnectionCmd(ENABLE);
}

void sensorConnectionCmd(FunctionalState newState)
{
	if(newState==ENABLE)
	{
		GPIO_ClearValue(GPIO1, 0x01<<muxEnPin);
	}
	else
	{
		GPIO_SetValue(GPIO1, 0x01<<muxEnPin);
	}
}

//static void uartRjCfg(void)
//{
//
//	PINSEL_CFG_Type PinSel;
//
//	I2C_DeInit(LPC_I2C2);
//
//	UART_CFG_Type UARTConfigStruct;
//
//	PinSel.Funcnum = PINSEL_FUNC_1;
//	PinSel.OpenDrain = PINSEL_PINMODE_NORMAL;
//	PinSel.Pinmode = PINSEL_PINMODE_PULLUP;
//	PinSel.Portnum = 0;
//	PinSel.Pinnum = 10;
//
//	PINSEL_ConfigPin(&PinSel);
//	PinSel.Pinnum = 11;
//	PINSEL_ConfigPin(&PinSel);
//
//	UART_ConfigStructInit(&UARTConfigStruct);
//
//
//	/* Initialize UART0 peripheral with given to corresponding parameter */
//	UART_Init(LPC_UART2, &UARTConfigStruct);
//
//	UART_TxCmd( LPC_UART2, ENABLE);
//
//}
