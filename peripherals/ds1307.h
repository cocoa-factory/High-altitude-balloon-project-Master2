//////////////////////////////////////////////////////////////////////////////////////////
//	
//	File		: 'ds1307.h'
//	Author		: Alan K. Duncan <duncan.alan@mac.com>
//	Created		: 2012-05-01
//	Revised		: 2012-05-09
//	Version		: 1.0
//	Target MCU	: ATmega644A
//	
//	This file provides an interface to the DS1307 real-time clock, using its I2C interface
//
//
//////////////////////////////////////////////////////////////////////////////////////////

#ifndef DS1307_H_
#define DS1307_H_

#include "../common/global.h"

#define DS1307_SECONDS_ADDR		0x00
#define DS1307_MINUTES_ADDR		0x01
#define DS1307_HOURS_ADDR		0x02
#define DS1307_DAY_ADDR			0x03
#define DS1307_DATE_ADDR		0x04
#define DS1307_MONTH_ADDR		0x05
#define DS1307_YEAR_ADDR		0x06
#define DS1307_CONTROL_ADDR		0x07



enum { kDS1307Mode12HR, kDS1307Mode24HR};
typedef u08 DS1307HourMode;

enum { 
	k_ds1307_sqw_mode_a,	//	1 Hz
	k_ds1307_sqw_mode_b,	//	4.096 kHz
	k_ds1307_sqw_mode_c,	//	8.192 kHz
	k_ds1307_sqw_mode_d		//	32.768 kHz
};
typedef u08 ds1307_sqw_mode_t;

void ds1307_init(DS1307HourMode mode);
u08 ds1307_seconds(void);
u08 ds1307_minutes(void);
u08 ds1307_hours(void);
u08 ds1307_date(void);
void ds1307_set_seconds(u08 seconds);
void ds1307_set_minutes(u08 minutes);
void ds1307_set_hours(u08 hours);
void ds1307_set_year(u08 year);
void ds1307_sqw_set_mode(ds1307_sqw_mode_t mode);

#endif /* DS1307_H_ */