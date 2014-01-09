/*
 * taskLedBlinking.c
 *
 *  Created on: 07-06-2012
 *      Author: Bartek
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#include "../stdPeriphLibs/lpc17xx_gpio.h"
#include "../stdPeriphLibs/lpc17xx_gpdma.h"
#include "../stdPeriphLibs/lpc17xx_dac.h"
#include "../stdPeriphLibs/lpc17xx_pinsel.h"
#include "taskLedBlinking.h"
#include "../boardConfiguration.h"
#include "../dac/dac.h"

#define LED_TMP_PIN 9
const unsigned short sine_buff[64] = {
		0x8000	,
		0x8C80	,
		0x9900	,
		0xA500	,
		0xB100	,
		0xBC40	,
		0xC700	,
		0xD100	,
		0xDA40	,
		0xE2C0	,
		0xEA40	,
		0xF0C0	,
		0xF600	,
		0xFA40	,
		0xFD40	,
		0xFF40	,
		0xFFC0	,
		0xFF40	,
		0xFD40	,
		0xFA40	,
		0xF600	,
		0xF0C0	,
		0xEA40	,
		0xE2C0	,
		0xDA40	,
		0xD100	,
		0xC700	,
		0xBC40	,
		0xB100	,
		0xA500	,
		0x9900	,
		0x8C80	,
		0x8000	,
		0x7380	,
		0x6700	,
		0x5B00	,
		0x4F00	,
		0x43C0	,
		0x3900	,
		0x2F00	,
		0x25C0	,
		0x1D40	,
		0x15C0	,
		0x0F40	,
		0x0A00	,
		0x05C0	,
		0x02C0	,
		0x00C0	,
		0x0040	,
		0x00C0	,
		0x02C0	,
		0x05C0	,
		0x0A00	,
		0x0F40	,
		0x15C0	,
		0x1D40	,
		0x25C0	,
		0x2F00	,
		0x3900	,
		0x43C0	,
		0x4F00	,
		0x5B00	,
		0x6700	,
		0x7380	};

__attribute__ ((section(".ahb_ram")))
static unsigned long int dma_buff_ping[64];	//Having DMA buffers in RAM allows them to be filled with arbitrary waveforms
__attribute__ ((section(".ahb_ram")))
static unsigned long int dma_buff_pong[64];	//if an application only needs fixed waveforms, simply update LLI's to point to const arrays
__attribute__ ((section(".ahb_ram")))
static dmaLinkedListNode dmaList[2];		//Array of nodes, both will be set to point to each other creating a looping "list"


#define TABLE_ELEMENTS 64
void initDMA(dmaLinkedListNode* pList)
{
	LPC_SC->PCONP |= 1<<29;		//Power GPDMA module

	LPC_GPDMA->DMACConfig = 1; 	//Enable GPDMA

	//Clear any previous interrupts
	LPC_GPDMA->DMACIntTCClear = 0xFF;

	//Initialize the channel with previously configured LL;
	LPC_GPDMACH0->DMACCSrcAddr = pList->sourceAddr;
	LPC_GPDMACH0->DMACCDestAddr = pList->destAddr;
	LPC_GPDMACH0->DMACCControl = pList->dmaControl;
	LPC_GPDMACH0->DMACCLLI = (unsigned long int) pList & 0xFFFFFFFC; //Lower bits must be 0

	//Set up all relevant bits
	//SrcPeripheral = 0, DestPeripheral = DAC
	LPC_GPDMACH0->DMACCConfig = 7<<6 | 1<<11;

	//Finally, enable the channel -
	LPC_GPDMACH0->DMACCConfig |= 1<<0;
}

#define TONE_TO_DIVISOR(toneFreq) (100000000/(toneFreq*8*TABLE_ELEMENTS))
#define Q16_MIN			(1<<14)
#define Q16_MAX			(1<<19)
#define Q16_INIT		(1<<16)
#define Q16_STEP		(1<<9)

uint8_t Channel0_TC;


void DMA_IRQHandler (void)
{
        // check GPDMA interrupt on channel 0
        if (GPDMA_IntGetStatus(GPDMA_STAT_INT, 0)){
                // Check counter terminal status
                if(GPDMA_IntGetStatus(GPDMA_STAT_INTTC, 0)){
                        GPDMA_ClearIntPending (GPDMA_STATCLR_INTTC, 0);
                        Channel0_TC++;
                }
                // Check error terminal status
                if (GPDMA_IntGetStatus(GPDMA_STAT_INTERR, 0)){
                        GPDMA_ClearIntPending (GPDMA_STATCLR_INTERR, 0);
  //                      Channel0_Err++;
                }
        }
}


static void initDAC(void)
{
	LPC_PINCON->PINSEL1 = 0x2<<20; 				//Select AOUT function on pin

	LPC_SC->PCLKSEL0 |= 0x3 << 22; 				//DAC Clocked at CCLK/8
	LPC_DAC->DACCNTVAL = TONE_TO_DIVISOR(1200);	//Timer rate = 100MHz/(8*444) => Timer/64samples = Waveform freq ~= 440Hz
	LPC_DAC->DACCTRL = 1<<2 | 1<<3; 			//Enable counter and DMA requests
}
#define BOUND(l, n, h) ((n)<(l) ? (l) : ((n)>(h) ? (h) : (n)))
void updateBuffer(unsigned short* pSrcWave,unsigned int fractionalDeltaQ16)
{
	unsigned int i;
	unsigned int fractionalIndexQ16 = 0;
	unsigned int lastIndex = 0;
	unsigned long int* pDestWaveBuff;
	dmaLinkedListNode* pNodeToUpdate;

	//Local pointers make use of 'active' DMA LLI...
	pNodeToUpdate = (dmaLinkedListNode*) LPC_GPDMACH0->DMACCLLI;
	pDestWaveBuff = (unsigned long int*) pNodeToUpdate->sourceAddr;

	//Wait until another buffer becomes active
	// !! NOTE !!
	//The highest frequency the project will produce is ~10KHz.
	//This updating method does not make any guarantee as to the duration of real time
	//required to perform an update. As such, higher frequency signals and/or signals
	//with larger buffers may not swap properly.
	while( (dmaLinkedListNode*) LPC_GPDMACH0->DMACCLLI == pNodeToUpdate)
	{
		;
	}

	//Now that another buffer is being transferred, it is safe to modify.
	//At SystemCoreClock=100MHz it is assumed that traversing this loop
	//will require less than the 100uS required to update buffers at 10KHz
	for ( i=0; i<TABLE_ELEMENTS; i++, fractionalIndexQ16 += fractionalDeltaQ16 )
	{
		if ( ((fractionalIndexQ16>>16) &0x3F) < lastIndex)
		{
			//wrap around has occurred, exit loop and update count
			break;
		}

		//Q16 fixed point fractional representation
		pDestWaveBuff[i] = pSrcWave[(fractionalIndexQ16>>16) &0x3F];
		//Record "last" seen index for boundary checking
		lastIndex = (fractionalIndexQ16>>16) &0x3F;
	}

	//If no wrap around has occurred i=64, otherwise i<64
	//This prevents strange waveforms from being loaded when frequency is reduced
	//and/or reset via center joystick position.
	pNodeToUpdate->dmaControl &= ~0xFFF;
	pNodeToUpdate->dmaControl |= BOUND(0,i-1,63);
}

void vLedTask(void *pvParameters) {

	unsigned char i;

	unsigned long int stepDeltaQ16 = Q16_INIT;	//Initial value: 1.0
	unsigned short* pSrc;
	dmaLinkedListNode* pNodePing = &dmaList[0];
	dmaLinkedListNode* pNodePong = &dmaList[1];

	Channel0_TC = 0;

	//SystemInit(); //Called by startup code

	//Make initial image of waveform in both buffers
	//Buffers live in RAM to enable users to populate them with arbitrary waveforms
	//If constant waveforms are all that an application requires simply point LLI structures
	//below to point to FLASH
	pSrc = (unsigned short*) &sine_buff[0];
	for (i=0; i<TABLE_ELEMENTS; i++)
	{
		dma_buff_ping[i] = pSrc[i];
		dma_buff_pong[i] = pSrc[i];
	}

	pNodePing->sourceAddr = (unsigned long int) &dma_buff_ping[0];
	pNodePing->destAddr = (unsigned long int) &LPC_DAC->DACR;
	pNodePing->nextNode = (unsigned long int) pNodePong; //bounce between double buffers forever
	//Size is 64 buffer elements, 32bits each
	//Transferred into DAC peripheral as 32 bits
	//Increment source address, but not destination
	pNodePing->dmaControl = TABLE_ELEMENTS | 0x2<<18 | 0x2<<21 | 1<<26;

	//Different sourceAddr for 2nd LLI node, but otherwise the same
	pNodePong->sourceAddr = (unsigned long int) &dma_buff_pong[0];
	pNodePong->destAddr = (unsigned long int) &LPC_DAC->DACR;
	pNodePong->nextNode = (unsigned long int) pNodePing; //bounce between double buffers forever
	pNodePong->dmaControl = TABLE_ELEMENTS | 0x2<<18 | 0x2<<21 | 1<<26;

	//InitSysTick(100);
	initDAC();
	initDMA(pNodePing);

	//updateBuffer(pSrc,stepDeltaQ16);
	//updateBuffer(pSrc,stepDeltaQ16);
	dmaLinkedListNode* pNodeOld = NULL;
	while (1) {

		dmaLinkedListNode* pNodeCurr = (dmaLinkedListNode*) LPC_GPDMACH0->DMACCLLI;
		if (pNodeCurr != pNodeOld) {
			pNodeOld = pNodeCurr;
		}
		vTaskDelay(1);
	}
}


