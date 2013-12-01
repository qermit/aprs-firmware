/*
 * commonFunctions.c
 *
 *  Created on: 29-07-2012
 *      Author: Bartek
 */

#include "commonFunctions.h"
#include "../stdPeriphLibs/lpc17xx_timer.h"
#include "../stdPeriphLibs/lpc17xx_uart.h"
#include "../stdPeriphLibs/LPC17xx.h"
#include "../stdPeriphLibs/lpc17xx_nvic.h"

void TIMER1_IRQHandler (void);
uint8_t delayTimeFlag = 0;

/*
 * @brief    Timer 0 interrupt
 * @param    None.
 * @return   None.
 */

void Timer1_init(uint32_t prescaleValue, uint32_t MatchValue)
{

	TIM_TIMERCFG_Type TMR1_Cfg;
	TIM_MATCHCFG_Type TMR1_Match;
	TIM_ResetCounter(LPC_TIM1);

	/* On reset, Timer0/1 are enabled (PCTIM1/1 = 1), and Timer2/3 are disabled (PCTIM2/3 = 0).*/
	/* Initialize timer 0, prescale count time of 100uS */
	TMR1_Cfg.PrescaleOption = TIM_PRESCALE_USVAL;
	TMR1_Cfg.PrescaleValue = prescaleValue;
	/* Use channel 0, MR0 */
	TMR1_Match.MatchChannel = 0;
	/* Enable interrupt when MR0 matches the value in TC register */
	TMR1_Match.IntOnMatch = ENABLE;
	/* Enable reset on MR0: TIMER will reset if MR0 matches it */
	TMR1_Match.ResetOnMatch = TRUE;
	/* Don't stop on MR0 if MR0 matches it*/
	TMR1_Match.StopOnMatch = TRUE;
	/* Do nothing for external output pin if match (see cmsis help, there are another options) */
	TMR1_Match.ExtMatchOutputType = TIM_EXTMATCH_NOTHING;
	/* Set Match value, count value of 10000 (10000 * 100uS = 1000000us = 1s --> 1 Hz) */
	TMR1_Match.MatchValue = MatchValue;
	/* Set configuration for Tim_config and Tim_MatchConfig */
	TIM_Init(LPC_TIM1, TIM_TIMER_MODE, &TMR1_Cfg);
	TIM_ConfigMatch(LPC_TIM1, &TMR1_Match);

	/* Preemption = 1, sub-priority = 1 */
	NVIC_SetPriority(TIMER1_IRQn, ((0x01<<3)|0x01));
	/* Enable interrupt for timer 0 */
	NVIC_EnableIRQ(TIMER1_IRQn);
	/* Start timer 0 */
	TIM_Cmd(LPC_TIM1, ENABLE);
}

void TIMER1_IRQHandler (void)
{
	/*  Clear Interrupt */
	TIM_ClearIntPending(LPC_TIM1,TIM_MR0_INT);
	delayTimeFlag = 1;
	TIM_Cmd(LPC_TIM1,DISABLE);
	NVIC_DisableIRQ(TIMER1_IRQn);

}

void delay_ms(uint32_t time)
{
	Timer1_init(time, 1000);
	while(delayTimeFlag==0);
	delayTimeFlag=0;
}
void delay_us(uint32_t time)
{
	Timer1_init(time, 1);
	while(delayTimeFlag==0);
	delayTimeFlag=0;
}

void sendFloat(float var)
{
	uint8_t s = (uint8_t)var;
	uint8_t tmp = s;
	char c[2];

	tmp=s%10;
	s=s/10;

	c[0] = (char)(48+s);
	c[1] = (char)(48+tmp);

	UART_Send(LPC_UART2, c, 2, BLOCKING);

}
