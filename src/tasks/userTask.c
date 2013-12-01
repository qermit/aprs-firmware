/*
 * userTask.c
 *
 *  Created on: 31-07-2012
 *      Author: Bartek
 */

#include "userTask.h"

#include "FreeRTOS.h"
#include "semphr.h"

void vUser()
{
	vSemaphoreCreateBinary(keyboardSemaphore);

}
