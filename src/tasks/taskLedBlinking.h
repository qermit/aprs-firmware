/*
 * taskLedBlinking.h
 *
 *  Created on: 07-06-2012
 *      Author: Bartek
 */

#ifndef TASKLEDBLINKING_H_
#define TASKLEDBLINKING_H_

void vLedTask(void *pvParameters);


typedef struct
{
	unsigned long int sourceAddr;
	unsigned long int destAddr;
	unsigned long int nextNode;
	unsigned long int dmaControl;
}dmaLinkedListNode;

#endif /* TASKLEDBLINKING_H_ */
