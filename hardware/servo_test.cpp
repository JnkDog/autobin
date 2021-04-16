#include <stdbool.h>
#include <stdio.h>
#include "wiringPi.h"
#include <wiringPiI2C.h> 
#include <unistd.h>


bool PCA9685Init();
void ResetPca9685();
void PCA9685SetPwmFreq(unsigned short freq);
void PCA9685SetPwm(unsigned char channel, unsigned short on, unsigned short value);
void SetServoPulse(unsigned char channel, unsigned short pulse);

#define PCA9685_ADDRESS			0x40			//pca9685address
#define PCA9685_CLOCK_FREQ		25000000		//PWMfrequency25MHz
#define PCA9685_MODE1			0x00
#define PCA9685_MODE2			0x01
#define PCA9685_PRE_SCALE		0xFE
#define PCA9685_LED0_ON_L		0x06
#define PCA9685_LED0_ON_H		0x07
#define PCA9685_LED0_OFF_L		0x08
#define PCA9685_LED0_OFF_H		0x09
#define PCA9685_LED_SHIFT		4



bool WriteByte(int fd, unsigned char regAddr, unsigned char data);
unsigned char ReadByte(int fd, unsigned char regAddr);


bool PCA9685_initSuccess = false;
int PCA9685_fd = 0;


int main()
{
	int PCA9865_channel = 15;
	PCA9685Init();
	PCA9685SetPwmFreq(50);
	while (1)
	{
		for (int i = 500; i < 2500; i += 10)
		{
			SetServoPulse(PCA9865_channel, i);
			usleep(20000);
		}
	}

	return 0;
}



bool PCA9685Init() 
{
	//initialize PCA9685_ADDRESS 0x40
	PCA9685_fd = wiringPiI2CSetup(PCA9685_ADDRESS);
	if (PCA9685_fd <= 0) 
		return false;
	PCA9685_initSuccess = true;
	ResetPca9685();
	return true;
}

void ResetPca9685() 
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

void PCA9685SetPwmFreq(unsigned short freq) 
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

void PCA9685SetPwm(unsigned char channel, unsigned short on, unsigned short value)
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


void SetServoPulse(unsigned char channel, unsigned short pulse)
{
	pulse = pulse * 4096 / 20000;
	PCA9685SetPwm(channel, 0, pulse);
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
