//////////////////////////////////////////////////////////////////////////////////////////
//	
//	File		: 'bmp085.h'
//	Author		: Alan K. Duncan <duncan.alan@mac.com>
//	Created		: 2012-05-01
//	Revised		: 2012-05-09
//	Version		: 1.0
//	Target MCU	: ATmega644A
//	
//	This file provides an interface to the BMP085 barometric pressure sensor via I2C 
//	interface.
//
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef BMP085_H_
#define BMP085_H_

#include "../common/global.h"
#include "../common/states.h"

void bmp085_init(void);
void bmp085_convert(void);
void bmp085_convert2(long *temp1, long *press1);

void bmp085Convert(long* temperature, long* pressure);

#endif /* BMP085_H_ */