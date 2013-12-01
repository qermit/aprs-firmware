/*
 * SHT21.c
 *
 *  Created on: 06-09-2012
 *      Author: Bartek
 */

#include "SHT21.h"
#include "../stdPeriphLibs/lpc_types.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_i2c.h"
#include "../gsmModule/gsmModule.h"

#define SHT_ADR 0x40

static uint8_t SHT_RxBuff[5];
static void pinInit(void);

void initSHT21(void)
{
	pinInit();
}

static void pinInit(void)
{
	PINSEL_CFG_Type PinSelConfigStruct;

	PinSelConfigStruct.Funcnum = PINSEL_FUNC_2;
	PinSelConfigStruct.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
	PinSelConfigStruct.Pinmode = PINSEL_PINMODE_TRISTATE;
	PinSelConfigStruct.Pinnum = 10;
	PinSelConfigStruct.Portnum = 0;

	PINSEL_ConfigPin(&PinSelConfigStruct);

	PinSelConfigStruct.Pinnum = 11;

	PINSEL_ConfigPin(&PinSelConfigStruct);

	I2C_Init(LPC_I2C2, 100000 );

	I2C_Cmd(LPC_I2C2, ENABLE);

}
float getSHT21Temp(void)
{
	uint8_t I2C_TxBuffer[3];
	uint16_t tmp;
	float temperature;

	I2C_M_SETUP_Type I2C_MasterInitStruct;


		I2C_TxBuffer[0] = 0xE3;


		I2C_MasterInitStruct.sl_addr7bit = SHT_ADR;
		I2C_MasterInitStruct.tx_count = 0;
		I2C_MasterInitStruct.tx_data = I2C_TxBuffer;
		I2C_MasterInitStruct.tx_length = 1;
		I2C_MasterInitStruct.retransmissions_count = 0;
		I2C_MasterInitStruct.retransmissions_max = 5;
		I2C_MasterInitStruct.rx_data = SHT_RxBuff;
		I2C_MasterInitStruct.rx_length = 3;

		I2C_MasterTransferData(LPC_I2C2, &I2C_MasterInitStruct, I2C_TRANSFER_POLLING);

		tmp = (SHT_RxBuff[0]<<8);
		tmp |= (SHT_RxBuff[1]&0xFC);

		temperature = -46.85 + 175.72*((float)tmp/65536);

		GSM_saveDataToSend(temperature, "TEMP");

	return temperature;
}
float getSHT21Hum(void)
{
	uint8_t I2C_TxBuffer[3];
	uint16_t tmp;
	float humildity;

	I2C_M_SETUP_Type I2C_MasterInitStruct;


	I2C_TxBuffer[0] = 0xE3;


	I2C_MasterInitStruct.sl_addr7bit = SHT_ADR;
	I2C_MasterInitStruct.tx_count = 0;
	I2C_MasterInitStruct.tx_data = I2C_TxBuffer;
	I2C_MasterInitStruct.tx_length = 1;
	I2C_MasterInitStruct.retransmissions_count = 0;
	I2C_MasterInitStruct.retransmissions_max = 5;
	I2C_MasterInitStruct.rx_data = SHT_RxBuff;
	I2C_MasterInitStruct.rx_length = 3;

	I2C_MasterTransferData(LPC_I2C2, &I2C_MasterInitStruct, I2C_TRANSFER_POLLING);

	tmp = (SHT_RxBuff[0]<<8);
	tmp |= (SHT_RxBuff[1]&0xFC);

	humildity = -6.0 + 125.0*((float)tmp/65536);

	GSM_saveDataToSend(humildity, "HMDT");

	return humildity;
}
