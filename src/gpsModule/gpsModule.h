/*
 * gpsModule.h
 *
 *  Created on: 11-05-2012
 *      Author: Bartek
 */

#ifndef GPSMODULE_H_
#define GPSMODULE_H_

#include "../stdPeriphLibs/lpc_types.h"
#include "stdint.h"

/* Defines	*/


/* Global variables */
typedef struct
{
	char	utcTime[10];	/* UTC of position (just time) */
	double	latitude;	/* Latitude in NDEG - [degree][min].[sec/60] */
	char    latitudeIndicator;	/* [N]orth or [S]outh */
	double	longitude;	/* Longitude in NDEG - [degree][min].[sec/60] */
    char    longitudeIndicator;	/* [E]ast or [W]est */
    uint8_t	positionFixStatusIndicator;	/* GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 6 = Estimated (Dead Reckoning)Fix) */
    uint8_t	satellitesUsed;	/* Number of satellites in use (not those in view), Range 0 to 12*/
    float	horizontalDilutionOfPrecision;	/* HDOP, Horizontal dilution of precision */
    float	mslAltitude;	/* Antenna altitude above/below mean sea level (geoid) */
    char    mslUnits;	/* [M]eters (Antenna height unit) */
    float	geoidSeparation;	/* Geoidal separation (Diff. between WGS-84 earth ellipsoid and mean sea level. '-' = geoid is below WGS-84 ellipsoid) */
    char    geoidSeparationUnits;	/* [M]eters (Units of geoidal separation) */
    uint8_t	diffStation;	/* Differential Reference Station ID */

}NmeaGgaMessage_T;


volatile NmeaGgaMessage_T lastReceivedGgaMessage;



void GpsInitialization(void);

char* getFieldPtr(uint8_t fieldNr);

char * gps_getToken(uint8_t i);
void gps_tokenize_input();
int gps_tokens();

void UART0_IRQHandler(void);
void parseGgaMessage(void);
uint8_t checkFixPresence(void);
char* getMessageIdPtr(void);
char* getGpsPosition(void);
char* getGpsTime(void);

int8_t gpsProcessingBuffer();
void gpsSetProcessingBuffer(int8_t i);

/* Private functions */


#endif /* GPSMODULE_H_ */
