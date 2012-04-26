/*
 * temp102.c
 *
 * Created: 4/13/2012 11:14:59 PM
 *  Author: Administrator
 */ 

#include "tmp102.h"
#include "../common/global.h"
#include "../capabilities/i2c.h"
#include "../capabilities/vfd.h"
#include <util/delay.h>

#define TMP102_BASE_ADDRESS 0x90	//	assumes ADR0 is tied to GND

#define TMP102_TEMP_REG		0x00
#define TMP102_CONFIG_REG	0x01
#define TMP102_LOW_REG		0x02
#define TMP102_HIGH_REG		0x03

void tmp102_read_temp(volatile tmp102_t *device) {
	u08 data[2];
	data[0] = TMP102_TEMP_REG;
	u08 retval = i2cMasterSendNI(device->address,1,&data);
	if( retval != I2C_OK ) { 
		device->is_valid = FALSE;
		device->temperature = TMP102_INVALID_TEMP;
		return;
	}
	_delay_ms(5);
	retval = i2cMasterReceiveNI(device->address,2,&data);
	if( retval != I2C_OK ) { 
		device->is_valid = FALSE;
		device->temperature = TMP102_INVALID_TEMP;
		return;
	}
	
	device->is_valid = TRUE;
	u08 msb,lsb;
	msb = data[0];
	lsb = data[1];
	s16 temp = (msb<<8) | lsb;
	temp >>= 4; //The TMP102 temperature registers are left justified, correctly right justify them
	
	//The tmp102 does twos compliment but has the negative bit in the wrong spot, so test for it and correct if needed
	if(temp & (1<<11))
		temp |= 0xF800; //Set bits 11 to 15 to 1s to get this reading into real twos compliment

	//printf("%02d\n", temp);

	//But if we want, we can convert this directly to a celsius temp reading
	//temp *= 0.0625; //This is the same as a divide by 16
	//temp >>= 4; //Which is really just a shift of 4 so it's much faster and doesn't require floating point
	//Shifts may not work with signed ints (negative temperatures). Let's do a divide instead
	temp /= 16;

	device->temperature = temp;
}
