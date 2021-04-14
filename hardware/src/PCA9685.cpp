#include <stdbool.h>
#include <stdio.h>
#include "wiringPi.h"
#include <wiringPiI2C.h> 
#include <unistd.h>
#include "PCA9685.h"



bool WriteByte(int fd, unsigned char regAddr, unsigned char data);
unsigned char ReadByte(int fd, unsigned char regAddr);


bool PCA9685_initSuccess = false;
int PCA9685_fd = 0;



PCA9685::PCA9685() 
{
	//initialize PCA9685_ADDRESS 0x40
	PCA9685_fd = wiringPiI2CSetup(PCA9685_ADDRESS);
	if (PCA9685_fd <= 0) 
		return false;
	PCA9685_initSuccess = true;
	reset();
	return true;
}

PCA9685::~PCA9685() {

	delete PCA9685_fd;
}


void PCA9685::reset() 
{
	if (true == PCA9685_initSuccess) 
	{
		//sleep mode, Low power mode. Oscillator off
		WriteByte(PCA9685_fd, PCA9685_MODE1, 0x00);
		WriteByte(PCA9685_fd, PCA9685_MODE2, 0x04);
		usleep(1000);
		//Delay Time is 0, means it always turn into high at the begin
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_L + PCA9685_LED_SHIFT * 0, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_H + PCA9685_LED_SHIFT * 0, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_L + PCA9685_LED_SHIFT * 1, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_H + PCA9685_LED_SHIFT * 1, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_L + PCA9685_LED_SHIFT * 2, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_H + PCA9685_LED_SHIFT * 2, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_L + PCA9685_LED_SHIFT * 3, 0);
		WriteByte(PCA9685_fd, PCA9685_LED0_ON_H + PCA9685_LED_SHIFT * 3, 0);
		usleep(1000);
	}
	else 
	{
		printf("pca9685 doesn't init\n");
	}
}

//! Set the frequency of PWM
/*!
 \param freq desired frequency. 40Hz to 1000Hz using internal 25MHz oscillator.
 */

void PCA9685::setPWMFreq(unsigned short freq) 
{ //set frequency
	unsigned char preScale = (PCA9685_CLOCK_FREQ / 4096 / freq) - 1;
	unsigned char oldMode = 0;
	printf("set PWM frequency to %d HZ\n",freq);
	//read old mode
	oldMode = ReadByte(PCA9685_fd, PCA9685_MODE1);
	//setup sleep mode, Low power mode. Oscillator off (bit4: 1-sleep, 0-normal)
	WriteByte(PCA9685_fd, PCA9685_MODE1, (oldMode & 0x7F) | 0x10);
	//set freq
	WriteByte(PCA9685_fd, PCA9685_PRE_SCALE, preScale);
	//setup normal mode (bit4: 1-sleep, 0-normal)
	WriteByte(PCA9685_fd, PCA9685_MODE1, oldMode);
	usleep(1000); // >500us
	//setup restart (bit7: 1- enable, 0-disable)
	WriteByte(PCA9685_fd, PCA9685_MODE1, oldMode | 0x80);
	usleep(1000); // >500us
}

void PCA9685::setPWM(unsigned char channel, unsigned short on, unsigned short value)
{
	//Set the PWM of each channel
	if (!PCA9685_initSuccess) 
	{
		printf("Set Pwm failure!\n");
		return;
	}

	WriteByte(PCA9685_fd, PCA9685_LED0_ON_L + PCA9685_LED_SHIFT * channel, on & 0xFF);
	WriteByte(PCA9685_fd, PCA9685_LED0_ON_H + PCA9685_LED_SHIFT * channel, on >> 8);
	WriteByte(PCA9685_fd, PCA9685_LED0_OFF_L + PCA9685_LED_SHIFT * channel, value & 0xFF);
	WriteByte(PCA9685_fd, PCA9685_LED0_OFF_H + PCA9685_LED_SHIFT * channel, value >> 8);
}


void PCA9685::setPulse(unsigned char channel, unsigned short pulse)
{
	pulse = pulse * 4096 / 20000;
	setPwm(channel, 0, pulse);
}







bool WriteByte(int fd, unsigned char regAddr, unsigned char data) 
{
	if (wiringPiI2CWriteReg8(fd, regAddr, data) < 0)
		return -1;
	return 0;
}



unsigned char ReadByte(int fd, unsigned char regAddr) 
{
	unsigned char data; // `data` will store the register data
	data = wiringPiI2CReadReg8(fd, regAddr);
	if (data < 0)
		return -1;
	return data;
}
