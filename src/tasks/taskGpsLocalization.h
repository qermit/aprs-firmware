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


/**
 * @brief vGpsTask
 * @param pvParameters
 */
void vGpsTask(void *pvParameters);

/**
 * @brief vGpsTest
 * @param pvParameters
 */
void vGpsTest(void *pvParameters);

#endif /* TASKGPSLOCALIZATION_H_ */
