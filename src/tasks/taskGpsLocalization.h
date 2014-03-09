/*
 * taskGpsLocalization.h
 *
 *  Created on: 19-07-2012
 *      Author: Bartek
 */
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>

#ifndef TASKGPSLOCALIZATION_H_
#define TASKGPSLOCALIZATION_H_

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



xSemaphoreHandle xSemaphoreGPS;
xTaskHandle xGpsTaskHandle;

void vGpsTask(void *pvParameters);
void vGpsTest(void *pvParameters);

#endif /* TASKGPSLOCALIZATION_H_ */
