/*
 * DS18B20.c
 *
 *  Created on: 04-09-2012
 *      Author: Bartek
 */

#include "DS18B20.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "sensorControl.h"
#include "../stdPeriphLibs/lpc_types.h"
#include "../commonFunctions.h"

#define CONVERT_T 0x44
#define READ_SCR 0xBE
#define WRITE_SCR 0x4E
#define SKIP_ROM 0xCC


static void oneWire(SetState newState);
static void oneWireInit(void);
static void send(uint8_t bit);
static void sendByte(uint8_t byte);
static void oneWireDir(portDirection_t dir);
static uint8_t getOneWireState(void);
uint8_t read(void);
uint16_t readByte(void);


void DS18B20_Init(void)
{
	oneWireInit();
}
static void oneWireInit(void)
{
	PINSEL_CFG_Type OneWireCfgStruct;

	OneWireCfgStruct.Funcnum = PINSEL_FUNC_0;
	OneWireCfgStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
	OneWireCfgStruct.Pinmode = PINSEL_PINMODE_TRISTATE;
	OneWireCfgStruct.Pinnum = 22;
	OneWireCfgStruct.Portnum = 0;

	PINSEL_ConfigPin(&OneWireCfgStruct);

}

static void oneWire(SetState newState)
{
	if(newState == SET)
	{
		GPIO_SetValue(0, 0x01<<22);
	}
	else
	{
		GPIO_ClearValue(0, 0x01<<22);
	}
}

static uint8_t getOneWireState(void)
{
	uint8_t currentState;
	uint32_t tmp=0;

	tmp = GPIO_ReadValue(0);

	if((tmp>>22)&0x01)
	{
		currentState = 1;
	}
	else
	{
		currentState = 0;
	}

	return currentState;
}

static void oneWireDir(portDirection_t dir)
{
	GPIO_SetDir(0, 0x01<<22, dir );
}


uint8_t DS18B20_Reset(void)
{
	uint8_t presence;
	oneWireDir(OUTPUT);
	oneWire(RESET);
	delay_us(700);
	//oneWire(SET);
	oneWireDir(INPUT);
	delay_us(50);


	presence = getOneWireState();

	delay_us(220);
	//presence = getOneWireState();
	oneWireDir(OUTPUT);

	return !presence;

}

float DS18B20_GetTemperature(void)
{

	//uint8_t presence =
			DS18B20_Reset();
	uint16_t tmp;//, tmp2;
	float temperature = 0.0;
	sendByte(SKIP_ROM);
	sendByte(CONVERT_T);
	delay_ms(750);

	if(DS18B20_Reset())
	{


		//presence = DS18B20_Reset();
		sendByte(SKIP_ROM);
		sendByte(READ_SCR);
		tmp = readByte();
		//tmp2 = readByte();

		//presence = DS18B20_Reset();

		temperature = (float)(tmp+(tmp*256.0))/16.0;
	}


	return temperature;
}

static void send(uint8_t bit)
{

	oneWireDir(OUTPUT);
	oneWire(RESET);
	//delay_us(5);
	if(bit == 1)
	{
		delay_us(10);
		oneWire(SET);
		delay_us(80);
		//oneWireDir(INPUT);
	}
	else
	{
		delay_us(80);
		oneWire(SET);
		delay_ms(10);
	}

		//oneWireDir(INPUT);

}
static void sendByte(uint8_t byte)
{
	uint8_t tmp = 0;
	uint8_t i = 0;

	for(i=0; i<8; i++)
	{
		tmp = byte>>i;
		tmp &= 0x01;
		send(tmp);
	}
	//delay_us(100);
}

uint8_t read(void)
{

	uint8_t presence;

	oneWireDir(OUTPUT);
	delay_us(50);
	oneWire(RESET);
	delay_us(10);
	oneWire(SET);
	oneWireDir(INPUT);
	delay_us(10);

	presence = getOneWireState();
    if(!presence)
    {
    	delay_us(100);
    }

    oneWireDir(OUTPUT);

	return (presence)?1:0;
}

uint16_t readByte(void)
{
	uint16_t val = 0;
	uint8_t i = 0;

	for(i=0; i<16; i++)
	{
		val |= read()<<i;
		//delay_us(15);
	}

	return val;
}
