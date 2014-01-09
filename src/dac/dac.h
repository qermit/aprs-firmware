/*****************************************************************************
 *   dac.h:  Header file for NXP LPC17xx Family Microprocessors
 *
 *   Copyright(C) 2009, NXP Semiconductor
 *   All rights reserved.
 *
 *   History
 *   2009.05.25  ver 1.00    Prelimnary version, first Release
 *
******************************************************************************/
#ifndef __DAC_H 
#define __DAC_H

#include "../stdPeriphLibs/lpc_types.h"


#define DAC_BIAS	0x00010000
#define SINE_TABLE_SIZE 64
//#define SINE_TABLE_SIZE 100

void DACInit( void );
uint8_t DACSetVal(uint8_t i);
uint32_t SineGetVal(uint8_t i);


#endif /* end __DAC_H */
/*****************************************************************************
**                            End Of File
******************************************************************************/
