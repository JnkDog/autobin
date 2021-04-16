
#include <iostream>
#include <unistd.h> 
#include <stdint.h>
#include <stdio.h>
#include "PCA9685.h"
#include "servo.h"

#define SERVOMIN  150 
// this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 
// this is the 'maximum' pulse length count (out of 4096)

PCA9685 PWM = PCA9685(); 
// called this way, it uses the default address 0x40
/*
you can also call it with a different address you want
PCA9685 pwm = PCA9685(0x41);
*/

//initialize
servo::servo() {
	PWM.reset();
    PWM.setPWMFreq(60);	
}

servo::~servo() {
	
}

void servo::set_servo_open (int channel) {
   
    sleep(2);
    
    /*
    while (1)
	{
		for (int i = 500; i < 2500; i += 10)
		{
			PWM.setPulse(channel, i);
			usleep(20000);
		}
	}
    */
    PWM.setPWM(channel, 0, SERVOMAX);
    printf("No.%d servo opens %d\n",channel,SERVOMAX);
}


void servo::set_servo_close (int channel) {
    
    sleep(2);
    PWM.setPWM(channel, 0, SERVOMIN);
    printf("No.%d servo closes %d\n",channel,SERVOMIN);
}