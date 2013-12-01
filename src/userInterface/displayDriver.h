/*
 * displayDriver.h
 *
 *  Created on: 06-06-2012
 *      Author: Bartek
 */

#ifndef DISPLAYDRIVER_H_
#define DISPLAYDRIVER_H_

//------------------------------------------------------------------------------
//   OLED register definition
//------------------------------------------------------------------------------
#define MEMORY_ADDRESSING_MODE 0x20
#define SET_COLUMN_ADDRESS     0x21 // Setup column start and end address
#define SET_PAGE_ADDRESS       0x22
#define DEACTIVATE_SCROLL      0x2E
#define ACTIVATE_SCROLL        0x2F
#define DISPLAY_START_LINE     0x40 // (0x40 = start at line 0)
#define CONTRAST_CONTROL       0x81 // Contrast control mode set
#define BRIGHTNESS_CONTROL     0x82
#define LOOK_UP_TABLE          0x91
#define MULTIPLEX_RATIO        0xA8
#define DISPLAY_ON_DIM         0xAC // ACh = Display ON in dim mode
#define MASTER_CONFIGURE       0xAD // DC-DC voltage converter OFF/ON
#define DISPLAY_OFF            0xAE // AEh = Display OFF (sleep mode) (RESET)
#define DISPLAY_ON             0xAF // ACh = Display ON in dim mode
#define DISPLAY_OFFSET         0xD3 // Set vertical shift by COM from 0~63.
#define DISPLAY_CLOCK_RATIO    0xD5 // Set Display clock divide ratio/oscillator frequency
#define AREA_COLOR_POWER_MODE  0xD8
#define PRE_CHARGE_PERIOD      0xD9 // Set discharge/precharge period
#define COM_PIN_CONFIG         0xDA
#define VCOMH_DETECT           0xDB
#define ENTER_READ_MODIFY      0xE0
#define EXIT_READ_MODIFY       0xEE


/**
 * Typedef
 */
typedef enum {COMMAND = 0, DATA = 1} sendType_t;

void displayInitialization(void);
void sendToDisplay(uint8_t dataToSend, sendType_t type);
void boostConverterCMD( FunctionalState );
void Init_SSD1305(void);

#endif /* DISPLAYDRIVER_H_ */
