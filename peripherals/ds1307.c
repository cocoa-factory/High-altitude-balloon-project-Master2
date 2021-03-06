//////////////////////////////////////////////////////////////////////////////////////////
//	
//	File		: 'ds1307.c'
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
#include "ds1307.h"
#include "../capabilities/i2c.h"
#include "../common/global.h"
#include <util/delay.h>
#include "../capabilities/uart2.h"

#define SQWE	(1<<4)
#define RS0	(1<<0)
#define RS1	(1<<1)
#define DS1307_BASE_ADDRESS 0xD0
#define CH (1<<7)
#define HR (1<<6)

u08 ds1307_read_register(u08 reg);
void  ds1307_write_register(u08 reg,u08 data);

u08 hour_mode;
u08 ampm_mode;

#define HOUR_24 0
#define HOUR_12 1

static unsigned int uint2bcd(unsigned int ival)
{
	return ((ival / 10) << 4) | (ival % 10);
}
char dec2bcd(char num)
{
  return ((num/10 * 16) + (num % 10));
}
// Convert Binary Coded Decimal (BCD) to Decimal
char bcd2dec(char num)
{
  return ((num/16 * 10) + (num % 16));
}

void ds1307_init(DS1307HourMode mode)
{
	
	/*	To start the oscillator, we need to write CH = 0 (bit 7/reg 0) */
	u08 seconds = ds1307_read_register(DS1307_SECONDS_ADDR);
	seconds &= ~CH;
	ds1307_write_register(DS1307_SECONDS_ADDR,seconds);
	ds1307_write_register(DS1307_SECONDS_ADDR,0x00);
	
	/*	set the mode */
	u08 hour = ds1307_read_register(DS1307_HOURS_ADDR);
	if( mode == kDS1307Mode24HR )
		hour &= ~HR;
	else
		hour |= HR;
	ds1307_write_register(DS1307_HOURS_ADDR, hour);
	hour_mode = HOUR_24;
}

u08 ds1307_seconds(void)
{
	u08 seconds_h,seconds_l;
	u08 seconds = ds1307_read_register(DS1307_SECONDS_ADDR);
	/*	mask the CH bit */
	seconds &= ~CH;
	/*	get the rest of the high nibble */
	seconds_h = seconds >> 4;
	seconds_l = seconds & 0b00001111;
	return seconds_h * 10 + seconds_l;
}

u08 ds1307_minutes(void)
{
	u08 minutes_h,minutes_l;
	u08 minutes = ds1307_read_register(DS1307_MINUTES_ADDR);
	minutes_h = minutes >> 4;
	minutes_l = minutes & 0b00001111;
	return minutes_h * 10 + minutes_l;
}

u08 ds1307_hours(void)
{
	u08 hours = ds1307_read_register(DS1307_HOURS_ADDR);
	if( hours & HR ) {
		//	12 hour mode
		hour_mode = HOUR_12;
		ampm_mode=(hours & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
		return bcd2dec(hours & 0x1F);
	}
	hour_mode = HOUR_24;
	ampm_mode = 0;
	return bcd2dec(hours & 0x3F);
}

u08 ds1307_date(void)
{
	u08 date_h,date_l;
	u08 date = ds1307_read_register(DS1307_DATE_ADDR);
	/*	mask the uppermost two bits */
	date &= ~(0b11000000);
	date_h = date >> 4;
	date_l = date & 0b00001111;
	return date_h * 10 + date_l;
}

void ds1307_set_seconds(u08 seconds)
{
	u08 bcd_seconds = uint2bcd(seconds);
	/* make sure CH bit is clear */
	bcd_seconds &= ~CH;
	ds1307_write_register(DS1307_SECONDS_ADDR,bcd_seconds);
}

void ds1307_set_minutes(u08 minutes)
{
	u08 bcd_minutes = uint2bcd(minutes);
	/*	make sure upper bit is clear */
	bcd_minutes &= ~(1<<7);
	ds1307_write_register(DS1307_MINUTES_ADDR,bcd_minutes);
}

void ds1307_set_hours(u08 hours)
{
	u08 hour_format = dec2bcd(hours);
	if( hour_mode ) {
		hour_format |= (1 << 6);
   	     if (ampm_mode)
	       hour_format |= (1 << 5);
         else
	       hour_format &= ~(1 << 5);
	}
	else {
		hour_format &= ~(1 << 6);
	}
	ds1307_write_register(DS1307_HOURS_ADDR,hour_format);
}

void ds1307_set_year(u08 year)
{
	 u08 bcd_year = uint2bcd(year);
	 ds1307_write_register(DS1307_YEAR_ADDR,bcd_year);
}

void  ds1307_write_register(u08 reg,u08 data)
{
	u08 device_data[2];
	device_data[0] = reg;
	device_data[1] = data;
	cli();
	i2cMasterSendNI(DS1307_BASE_ADDRESS,2,device_data);
	sei();
}

u08 ds1307_read_register(u08 reg)
{
	u08 device_data[2];
	device_data[0] = reg;
	i2cMasterSendNI(DS1307_BASE_ADDRESS,1,device_data);
	_delay_ms(10);
	i2cMasterReceiveNI(DS1307_BASE_ADDRESS,1,device_data);
	return device_data[0];
}

void ds1307_sqw_set_mode(ds1307_sqw_mode_t mode) {
	ds1307_write_register(DS1307_CONTROL_ADDR,0x90 + mode);
}