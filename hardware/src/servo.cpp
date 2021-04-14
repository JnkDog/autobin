
#include <iostream>
//#include <time.h>
#include <unistd.h> 
//#include <Adafruit_PCA9685.h>
#include <stdint.h>
#include <stdio.h>
#include "PCA9685.h"
PCA9685 PWM = PCA9685(); // called this way, it uses the default address 0x40
#include "servo.h"

servo::servo() {
	PWM.reset();
    PWM.setPWMFreq(60);
	
	
}

servo::~servo() {
	//delete i2c;
}

void servo::set_servo_open (int channel) {
   
    sleep(2);
    while (1)
	{
		for (int i = 500; i < 2500; i += 10)
		{
			PWM.setPulse(channel, i);
			usleep(20000);
		}
	}
    //PCA9685.setPWM(channel, 0, SERVOMAX);
    printf("%d号舵机开启%d\n",channel,SERVOMAX);
}


void servo::set_servo_close (int channel) {
    //set_servo_open(int channel);
    sleep(2);
    PWM.setPWM(channel, 0, SERVOMIN);
    printf("%d号舵机开启%d\n",channel,SERVOMIN);
}