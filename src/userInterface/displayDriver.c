/*
 * displayDriver.c
 *
 *  Created on: 06-06-2012
 *      Author: Bartek
 */


/**
 * Includes
 */
#include "../powerManagement/powerMenagement.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_spi.h"
#include "../stdPeriphLibs/lpc17xx_ssp.h"
#include "../stdPeriphLibs/lpc17xx_i2c.h"
//#include "../boardConfiguration.h"
#include "../stdPeriphLibs/lpc_types.h"
#include "../stdPeriphLibs/LPC17xx.h"
#include "displayDriver.h"
#include "commonFunctions.h"


/**
 * Define
 */
#define SPI_CLOCK_RATE 5000000
#define OLED_SPI LPC_SPI
#define OLED_I2C LPC_I2C1
#define CS 		16 //0
#define CTRL 	28//1
#define DC 		25//1
#define RST 	26//1
#define DATA 	18//0
#define CLK 	15//0
/**
 * Private function
 */
static void displayPinsConfiguration(void);
static void SPI_Configuration(void);


void WriteSpiData(uint16_t);
void WriteSpiCommand(uint16_t);
void Cls(void);
static void WriteSpi(uint16_t);


void displayInitialization(void)
{
	displayPinsConfiguration();
	boostConverterCMD(DISABLE);

	SPI_Configuration();
	Init_SSD1305();
}

void displayPinsConfiguration()
{
	PINSEL_CFG_Type PinSelConfigStruct;
	//Set LCD reset pin as output
	GPIO_SetDir(1, 0x01<<RST, OUTPUT);
	//Set LCD Data or Command pin as output
	GPIO_SetDir(1, 0x01<<DC, OUTPUT);
	// Set boost Converter Enable pin as output
	GPIO_SetDir(1, ( 0x01<<CTRL ), OUTPUT);
	//Set LCD CS as output pin
	GPIO_SetDir(0, 0x01<<CS, OUTPUT);


	PinSelConfigStruct.Portnum = 0;
	PinSelConfigStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinSelConfigStruct.Pinmode = PINSEL_PINMODE_PULLUP;
	PinSelConfigStruct.Pinnum = DATA;
	PinSelConfigStruct.Funcnum = PINSEL_FUNC_3;

	PINSEL_ConfigPin(&PinSelConfigStruct);

	PinSelConfigStruct.Pinnum = CLK;

	PINSEL_ConfigPin(&PinSelConfigStruct);

	//PinSelConfigStruct.Pinnum = CS;
	//PinSelConfigStruct.Funcnum = PINSEL_FUNC_0;
	//PINSEL_ConfigPin(&PinSelConfigStruct);

	GPIO_SetValue(0, 0x01<<CS);
}

void SPI_Configuration(void)
{
	SPI_CFG_Type SPI_ConfigStruct;

	SPI_ConfigStruct.CPHA = SPI_CPHA_SECOND;
	SPI_ConfigStruct.CPOL = SPI_CPOL_HI;
	SPI_ConfigStruct.ClockRate = SPI_CLOCK_RATE;
	SPI_ConfigStruct.DataOrder = SPI_DATA_MSB_FIRST;
	SPI_ConfigStruct.Databit = SPI_DATABIT_8;
	SPI_ConfigStruct.Mode = SPI_MASTER_MODE;

	SPI_Init(OLED_SPI, &SPI_ConfigStruct);


	SPI_IntCmd(OLED_SPI, ENABLE);

//	SSP_CFG_Type SSP_ConfigStuct;
//
//	SSP_ConfigStuct.Databit = SSP_DATABIT_8;
//	SSP_ConfigStuct.CPHA = SSP_CPHA_SECOND;
//	SSP_ConfigStuct.CPOL = SSP_CPOL_LO;
//	SSP_ConfigStuct.ClockRate = SPI_CLOCK_RATE;
//	SSP_ConfigStuct.FrameFormat = SSP_FRAME_SPI;
//	SSP_ConfigStuct.Mode = SSP_MASTER_MODE;
//
//	SSP_Init(OLED_SPI, &SSP_ConfigStuct);
//	SSP_Cmd(OLED_SPI, ENABLE);
//	NVIC_DisableIRQ(SSP1_IRQn);

}

void WriteSpiCommand(uint16_t data)
{
	GPIO_SetValue(1, 0x01<<DC);
	GPIO_ClearValue(0, 0x01<<CS);
	WriteSpi(data);
	while(!SPI_CheckStatus(SPI_GetStatus(OLED_SPI),SPI_STAT_SPIF));
	GPIO_SetValue(0, 0x01<<CS);
}

static void WriteSpi( uint16_t data)
{
	SPI_DATA_SETUP_Type SPI_Data;
	SPI_Data.counter = 0;
	SPI_Data.length = 1;
	SPI_Data.rx_data = 0;
	SPI_Data.tx_data = data;
	SPI_Data.status = SPI_STAT_DONE;

	//SPI_ReadWrite(OLED_SPI, &SPI_Data, SPI_TRANSFER_POLLING);
	SPI_SendData(OLED_SPI, data);
}



void Init_SSD1305()
{
//	GPIO_ClearValue(1, 0x01<<DC);
	GPIO_ClearValue(1, 0x01<<CTRL);
    GPIO_ClearValue(1, 0x01<<RST);
    delay_us(5);
    GPIO_SetValue(1, 0x01<<RST);



	WriteSpiCommand(0xAE);
	WriteSpiCommand(0xA1); //segment remap
	WriteSpiCommand(0xDA); //common pads hardware: alternative
	WriteSpiCommand(0x12);
	WriteSpiCommand(0xC8); //common output scan direction:com63~com0
	WriteSpiCommand(0xA8); //multiplex ration mode:63
	WriteSpiCommand(0x3F);
	WriteSpiCommand(0xD5); //display divide ratio/osc. freq. mode
	WriteSpiCommand(0x70); //Osc. Freq:320kHz,DivideRation:1
	WriteSpiCommand(0x81); //contrast control
	WriteSpiCommand(0x29); // mode:64
	WriteSpiCommand(0xD9); //set pre-charge period
	WriteSpiCommand(0x22); //set period 1:1;period 2:15
	WriteSpiCommand(0x20); //Set Memory Addressing Mode
	WriteSpiCommand(0x02); //page addressing mode
	WriteSpiCommand(0xDB); //VCOM deselect level mode
	WriteSpiCommand(0x3C); //set Vvcomh=0.83*Vcc
	WriteSpiCommand(0xAD); //master configuration
	WriteSpiCommand(0x8A); //external VCC supply
	WriteSpiCommand(0xA4); //out follows RAM content
	WriteSpiCommand(0xA6); //set normal display

//	WriteSpiCommand(0x20);
//	WriteSpiData(0x00);
//	WriteSpiData(0x21);
//	WriteSpiData(0x0);
//	WriteSpiData(131);
//	WriteSpiData(0x22);
//	WriteSpiData(0x0);
//	WriteSpiData(0x7);
//	uint16_t z = 0;
//	for (z=0;z<528;z++) WriteSpiData(0x00);
//	delay_ms(100);


	GPIO_SetValue(1, 0x01<<CTRL);

	delay_ms(100);

	WriteSpiCommand(0xAF);



//	sendToDisplay(0xAE, COMMAND);
//	sendToDisplay(0xA1, COMMAND); //segment remap
//	sendToDisplay(0xDA, COMMAND); //common pads hardware: alternative
//	sendToDisplay(0x12, COMMAND);
//	sendToDisplay(0xC8, COMMAND); //common output scan direction:com63~com0
//	sendToDisplay(0xA8, COMMAND); //multiplex ration mode:63
//	sendToDisplay(0x3F, COMMAND);
//	sendToDisplay(0xD5, COMMAND); //display divide ratio/osc. freq. mode
//	sendToDisplay(0x70, COMMAND); //Osc. Freq:320kHz,DivideRation:1
//	sendToDisplay(0x81, COMMAND); //contrast control
//	sendToDisplay(0x29, COMMAND); // mode:64
//	sendToDisplay(0xD9, COMMAND); //set pre-charge period
//	sendToDisplay(0x22, COMMAND); //set period 1:1;period 2:15
//	sendToDisplay(0x20, COMMAND); //Set Memory Addressing Mode
//	sendToDisplay(0x02, COMMAND); //page addressing mode
//	sendToDisplay(0xDB, COMMAND); //VCOM deselect level mode
//	sendToDisplay(0x3C, COMMAND); //set Vvcomh=0.83*Vcc
//	sendToDisplay(0xAD, COMMAND); //master configuration
//	sendToDisplay(0x8E, COMMAND); //external VCC supply
//	sendToDisplay(0xA4, COMMAND); //out follows RAM content
//	sendToDisplay(0xA7, COMMAND); //set normal display


}

void Cls()
{
	uint16_t i =0;

//	WriteSpiCommand(0x20);
//	WriteSpiCommand(0x00);
//	WriteSpiCommand(0x21);
//	WriteSpiCommand(0);
//	WriteSpiCommand(131);
//	WriteSpiCommand(0x22);
//	WriteSpiCommand(0);
//	WriteSpiCommand(7);
//	for(i=0;i<528;i++)
//	{
//		WriteSpiData(0x00);
//	}
}

void boostConverterCMD( FunctionalState newState )
{
	if(newState==ENABLE)
	{
		GPIO_SetValue(1, (0x01<<CTRL));
	}
	else
	{
		GPIO_ClearValue(1, (0x01<<CTRL));
	}

}


//	WriteSpiCommand(0xae);
//	WriteSpiCommand(0xd5);
//	WriteSpiCommand(0x10);
//	WriteSpiCommand(0xA8);
//	WriteSpiCommand(0x1f);
//	WriteSpiCommand(0xd3);
//	WriteSpiCommand(0x00);
//	WriteSpiCommand(0x40);
//	WriteSpiCommand(0xad);
//	WriteSpiCommand(0x8e);
//	WriteSpiCommand(0xd8);
//	WriteSpiCommand(0x05);
//	WriteSpiCommand(0xa1);
//	WriteSpiCommand(0xc8);
//	WriteSpiCommand(0xda);
//	WriteSpiCommand(0x12);
//	WriteSpiCommand(0x91);
//	WriteSpiCommand(0x3f);
//	WriteSpiCommand(0x3f);
//	WriteSpiCommand(0x3f);
//	WriteSpiCommand(0x3f);
//	WriteSpiCommand(0x81);
//	WriteSpiCommand(0xbf);
//	WriteSpiCommand(0xd9);
//	WriteSpiCommand(0xd2);
//	WriteSpiCommand(0xdb);
//	WriteSpiCommand(0x08);
//	WriteSpiCommand(0xa4);
//	WriteSpiCommand(0xa6);

//	WriteSpiCommand(0xAE);
//	WriteSpiCommand(0xA1); //segment remap
//	WriteSpiCommand(0xDA); //common pads hardware: alternative
//	WriteSpiCommand(0x12);
//	WriteSpiCommand(0xC8); //common output scan direction:com63~com0
//	WriteSpiCommand(0xA8); //multiplex ration mode:63
//	WriteSpiCommand(0x3F);
//	WriteSpiCommand(0xD5); //display divide ratio/osc. freq. mode
//	WriteSpiCommand(0x70); //Osc. Freq:320kHz,DivideRation:1
//	WriteSpiCommand(0x81); //contrast control
//	WriteSpiCommand(0x29); // mode:64
//	WriteSpiCommand(0xD9); //set pre-charge period
//	WriteSpiCommand(0x22); //set period 1:1;period 2:15
//	WriteSpiCommand(0x20); //Set Memory Addressing Mode
//	WriteSpiCommand(0x02); //page addressing mode
//	WriteSpiCommand(0xDB); //VCOM deselect level mode
//	WriteSpiCommand(0x3C); //set Vvcomh=0.83*Vcc
//	WriteSpiCommand(0xAD); //master configuration
//	WriteSpiCommand(0x8E); //external VCC supply
//	WriteSpiCommand(0xA4); //out follows RAM content
//	WriteSpiCommand(0xA7); //set normal display
//	Cls();

//	delay_ms(105);
//	boostConverterCMD(ENABLE);
//	delay_ms(105);
//	WriteSpiCommand(0xaf);

//void WriteSpiCommand(unsigned char cmd)
//{
//	GPIO_ClearValue(0, 0x01<<lcdSselPin);
//	GPIO_ClearValue(1, 0x01<<lcdDcPin);
//	WriteSpi(cmd);
//}

//
//

//
//void sendToDisplay(uint8_t dataToSend, sendType_t type)
//{
//
//	if(type == COMMAND)
//	{
//		GPIO_ClearValue(1, 0x01<<lcdDcPin);
//	}
//	else if(type == DATA)
//	{
//		GPIO_SetValue(1, 0x01<<lcdDcPin);
//	}
//  GPIO_ClearValue(0, 0x01<<lcdSselPin);
//  while((GPIO_ReadValue(0)&0x40));
//  //delay_us(1);
////SSP_DATA_SETUP_Type newdata;
////newdata.
////SSP_ReadWrite(OLED_SPI, )
////  SPI_SendData(OLED_SPI, dataToSend);
//  SSP_SendData(OLED_SPI, dataToSend);
//
//  while(!(SSP_GetStatus(OLED_SPI, SSP_STAT_TXFIFO_EMPTY)));
//  GPIO_SetValue(0, 0x01<<lcdSselPin);
//  while(!(GPIO_ReadValue(0)&0x40));
//}
//void pinInit()
//{
//	PINSEL_CFG_Type PinSelConfigStruct;
//	//rst
//	GPIO_SetDir(1, 0x01<<RST, OUTPUT);
//
//	PinSelConfigStruct.Pinnum = RST;
//	PinSelConfigStruct.Portnum = 1;
//	PinSelConfigStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
//	PinSelConfigStruct.Pinmode = PINSEL_PINMODE_PULLUP;
//	PinSelConfigStruct.Funcnum = PINSEL_FUNC_0;
//
//	PINSEL_ConfigPin(&PinSelConfigStruct);
//
//	//dc
//	GPIO_SetDir(1, 0x01<<DC, OUTPUT);
//
//
//	//ctrl
//	GPIO_SetDir(1, 0x01<<CTRL, OUTPUT);
//
//	PinSelConfigStruct.Pinnum = CTRL;
//	PinSelConfigStruct.Portnum = 1;
//	PinSelConfigStruct.OpenDrain = PINSEL_PINMODE_NORMAL;
//	PinSelConfigStruct.Pinmode = PINSEL_PINMODE_PULLDOWN;
//	PinSelConfigStruct.Funcnum = PINSEL_FUNC_0;
//
//	PINSEL_ConfigPin(&PinSelConfigStruct);
//
//
//	//data
//	PinSelConfigStruct.Funcnum = PINSEL_FUNC_3;
//	PinSelConfigStruct.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
//	PinSelConfigStruct.Pinmode = PINSEL_PINMODE_TRISTATE;
//	PinSelConfigStruct.Pinnum = DATA;
//	PinSelConfigStruct.Portnum = 0;
//
//	PINSEL_ConfigPin(&PinSelConfigStruct);
//
//	//clk
//	PinSelConfigStruct.Pinnum = CLK;
//
//	PINSEL_ConfigPin(&PinSelConfigStruct);
//
//	I2C_Init(OLED_I2C, 400000 );
//
//	I2C_Cmd(OLED_I2C, ENABLE);
//
//
//}
