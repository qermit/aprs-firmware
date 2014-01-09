/*****************************************************************************
 *   dac.c:  DAC module file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
//#include "LPC17xx.h"
//#include "type.h"
#include "dac.h"

#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../boardConfiguration.h"
#include "../stdPeriphLibs/lpc17xx_rtc.h"

/*****************************************************************************
** Function name:		DACInit
**
** Descriptions:		initialize DAC channel
**
** parameters:			None
** Returned value:		None
** 
*****************************************************************************/

static uint32_t Sine[] = {512,544,576,608,639,670,700,730,759,786,813,838,862,885,907,926,944,961,975,988,999,1008,1015,1020,1023,1023,
           1023,1020,1015,1008,999,988,975,961,944,926,907,885,862,838,813,786,759,730,700,670,639,608,576,544,512,
            480,448,416,385,354,324,294,265,238,211,186,162,139,117,98,80,63,49,36,25,16,9,4,1,1,1,
            4,9,16,25,36,49,63,80,98,117,139,162,186,211,238,265,294,324,354,385,416,448,480};




void DACInit( void )
{
  /* setup the related pin to DAC output */
  LPC_PINCON->PINSEL1 = 0x00200000;	/* set p0.26 to DAC output */   
  return;
}


uint8_t DACSetVal(uint8_t i)
{

	uint32_t sine_val = 0;
	uint8_t sine_index = 0;

	if (i<SINE_TABLE_SIZE)  {
		sine_index = i;
	}
	sine_val = Sine[sine_index];
	LPC_DAC->DACR = ((sine_val&0x3FF) << 6) | DAC_BIAS;

	if ( sine_index++ >= SINE_TABLE_SIZE )
	{
		sine_index = 0;
	}
	return sine_index;

}
/*********************************************************************************
**                            End Of File
*********************************************************************************/
