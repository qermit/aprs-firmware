/*
 * boardConfiguration.h
 *
 *  Created on: 11-05-2012
 *      Author: Bartek
 */

#ifndef BOARDCONFIGURATION_H_
#define BOARDCONFIGURATION_H_

// defines GPIO
#define GPIO0 0
#define GPIO1 1
#define GPIO2 2

/*************************/
/*******Pins GPIO0 *******
*************************/

//Battery voltage ADC
#define vBattPin    26

//USB PINS
#define usbPlusPin  29
#define usbMinusPin 30

// Q-touch keyboard control pins
// Q-touch uses I2C1 interface
#define qtSdaPin 0
#define qtSclPin 1

// RJ-50 Pins
// I2C2 and UART2
// SPI0
#define rjTxdSdaPin 10
#define rjRxdSclPin 11
#define rjSckPin  15
#define rjSselPin 16
#define rjMisoPin 17
#define rjMosiPin 18
#define rjGpioPin 22

// LCD display pins
// SPI1
#define lcdSselPin 6
#define lcdSckPin  7
#define lcdMosiPin 9

// GPS pins
// UART0
#define gpsTxdPin 2
#define gpsRxdPin 3
#define gpsEnPin  8


/*************************/
/*******Pins GPIO1 *******
*************************/

// Multiplexers control pins
#define muxS0Pin   18
#define muxS1Pin   19
#define muxEnPin   20

// Battery Charger Pins
#define chrgEnPin  22
#define chrgUselPin 23
#define chrgChgPin 24

// General Purpose Input/Output
#define Pin1 25
#define Pin2 26

// Q-touch keyboard control pins
// Q-touch uses I2C1 interface
#define qtRstPin 28


// Sensor ADC voltage pin
#define vSensPin   30

// LCD Step-Up converter shut down pin
#define lcdShdPin  31
// LCD control pins
#define lcdRstPin 9
#define lcdDcPin  10


/*************************/
/*******Pins GPIO2 *******
*************************/

// GSM module pins
// UART1
#define gsmTxdPin 0
#define gsmRxdPin 1
#define gsmCtsPin 2
#define gsmDcdPin 3
#define gsmDtrPin 5
#define gsmRiPin  6
#define gsmRtsPin 7
#define gsmPwrPin 8
#define gsmStatPin 9

// Qtouch interrupt pin
#define qtChgPin 10

#endif /* BOARDCONFIGURATION_H_ */
