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

xSemaphoreHandle xSemaphoreGPS;
xTaskHandle xGpsTaskHandle;

void vGpsTask(void *pvParameters);
void vGpsTest(void *pvParameters);

#endif /* TASKGPSLOCALIZATION_H_ */
