/*
 * qTouch.h
 *
 *  Created on: 30-05-2012
 *      Author: Bartek
 */

#ifndef QTOUCH_H_
#define QTOUCH_H_

void qtouchKeyboardInitialization(void);
void qtouchResetControler(void);
void EINT3_IRQHandler(void);
uint8_t getQtouchPressedKeyIndicator(void);
void EINT0_IRQHandler(void);
void debugq(void);

#endif /* QTOUCH_H_ */
