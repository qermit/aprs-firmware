/*
 * poweMenagement.c
 *
 *  Created on: 23-05-2012
 *      Author: Bartek
 */

#include "powerMenagement.h"
#include "../boardConfiguration.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_adc.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"


static float batteryVoltage = 0.0;

static void GPIO_Initialization(void);
static void ADC_Initialization(void);


void powerSectionInit (void)
{
	GPIO_Initialization();
	//ADC_Initialization();
	chargerCmd(ENABLE);
	setUsbCurrent(HIGH);
}


void GPIO_Initialization(void)
{
	// Set EN i USEL as output pins
	GPIO_SetDir(GPIO1, ( (0x01<<chrgEnPin)|(0x01<<chrgUselPin) ), 1);
	// Set CHG as input pin
	GPIO_SetDir(GPIO1, (0x01<<chrgChgPin), 0);


}

void ADC_Initialization(void)
{
	PINSEL_CFG_Type PINSEL_CfgStruct;

	PINSEL_CfgStruct.Portnum 	= GPIO0;
	PINSEL_CfgStruct.Pinnum 	= vBattPin;
	PINSEL_CfgStruct.Pinmode	= PINSEL_PINMODE_TRISTATE;
	PINSEL_CfgStruct.Funcnum 	= PINSEL_FUNC_1;
	PINSEL_CfgStruct.OpenDrain 	= PINSEL_PINMODE_NORMAL;

	PINSEL_ConfigPin(&PINSEL_CfgStruct);

	ADC_Init(LPC_ADC, 200000);
	ADC_ChannelCmd(LPC_ADC,ADC_CHANNEL_3,ENABLE);

}

/**
 * Set charger functional state
 * params:	newState  - ENABLE or DISABLE
 */

void chargerCmd( FunctionalState newState)
{
	if(newState == ENABLE)
	{
		GPIO_SetValue(GPIO1, (0x01<<chrgEnPin) );
	}
	else
	{
		GPIO_ClearValue(GPIO1, (0x01<<chrgEnPin) );
	}
}


chargingStatus_t getChargerStatus( void )
{
	return (chargingStatus_t)((GPIO_ReadValue(GPIO1) >> chrgChgPin ) & 0x01);
}

void setUsbCurrent( logicalState_t newState )
{
	if(newState == HIGH)
	{
		GPIO_SetValue(GPIO1, 0x01<<chrgUselPin);
	}
	else
	{
		GPIO_ClearValue(GPIO1, 0x01<<chrgUselPin);
	}

}


float getBatteryVoltage( void )
{
	uint16_t data;
	ADC_StartCmd(LPC_ADC, ADC_START_NOW);
	while(!ADC_ChannelGetStatus(LPC_ADC, 3, 1));
	data = ADC_ChannelGetData(LPC_ADC, 3);
	batteryVoltage = (float)(data/4096.0)*3.3;
	return (batteryVoltage*2);
}

