/*
 * realTimeClock.c
 *
 *  Created on: 22-10-2012
 *      Author: Bartek
 */

#include "realTimeClock.h"
#include <stdlib.h>
#include "../stdPeriphLibs/lpc_types.h"
#include "../stdPeriphLibs/lpc17xx_rtc.h"

static char fullTime[] = {"2012-05-23 9:15:00\0"};
uint8_t * fullTimePtr = fullTime;

void initRTC(void)
{
	RTC_Init(LPC_RTC);
	RTC_ResetClockTickCounter(LPC_RTC);
	RTC_Cmd(LPC_RTC, ENABLE);


	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_SECOND, 50);
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MINUTE, 15);
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_HOUR, 9);
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_MONTH, 02);
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_YEAR, 2013);
	RTC_SetTime(LPC_RTC, RTC_TIMETYPE_DAYOFMONTH, 29);

}

uint8_t* getRtcFullTime(void)
{
	RTC_TIME_Type rtcFullTime;
	uint8_t* tmpPtr = fullTimePtr;


	RTC_GetFullTime(LPC_RTC, &rtcFullTime);
	sprintf(tmpPtr, "%4d",rtcFullTime.YEAR);
	tmpPtr+=4;
	(*tmpPtr++) = '-';
	sprintf(tmpPtr, "%02d",rtcFullTime.MONTH);
	tmpPtr+=2;
	(*tmpPtr++) = '-';
	sprintf(tmpPtr, "%02d",rtcFullTime.DOM);
	tmpPtr+=2;
	(*tmpPtr++) = ' ';
	sprintf(tmpPtr, "%02d",rtcFullTime.HOUR);
	tmpPtr+=2;
	(*tmpPtr++) = ':';
	sprintf(tmpPtr, "%02d",rtcFullTime.MIN);
	tmpPtr+=2;
	(*tmpPtr++) = ':';
	sprintf(tmpPtr, "%02d",rtcFullTime.SEC);
	tmpPtr+=2;
	(*tmpPtr) = '\0';

	return fullTimePtr;
}
