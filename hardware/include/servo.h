#ifndef _servo_H
#define _servo_H

#include <iostream>
//#include <time.h>
//#include <unistd.h> 
//#include <Adafruit_PCA9685.h>
//#include <stdint.h>
#include "PCA9685.h"
#define PCA9685 PWM
// called this way, it uses the default address 0x40
//以这种方式调用，它使用默认地址0x40。
//PCA9685 pwm = PCA9685();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
//也可以用不同的地址调用它

/* Depending on your servo make, the pulse width min and max may vary, you  want these to be as small/large as possible without hitting the hard stop
 for max range. You'll have to tweak them as necessary to match the servos you
have!*/
/*根据你的伺服制作，脉冲宽度最小和最大可能变化，你想要这些尽可能小大而不碰到
硬停止，对于最大范围。你必须调整它们以匹配你的伺服系统！*/
#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
//这是“最小”脉冲长度计数（在4096）中
#define SERVOMAX  600 // this is the 'maximum' pulse length count (out of 4096)
//这是“最大”脉冲长度计数（在4096中）

using namespace std;

class servo //: public PCA9685
{
    //servo_min = 150  # Min pulse length out of 4096
    //servo_max = 600  # Max pulse length out of 4096
    //pwm = Adafruit_PCA9685.PCA9685()//# 地址默认为这个 address=PCA9685_ADDRESS
                                    //# 如果修改了模块上的地址，则需要修改。
    public: 
    servo();
	virtual ~servo();
    
    void set_servo_open(int channel);
    void set_servo_close(int channel);
    //PCA9685 pwm;   //= PCA9685();  
    //PWM = new PCA9685();
   // PCA9685.setPWMFreq(60); //设置频率
    
};

/*
servo::servo(int channel) {
	PWM = new PCA9685(addr);
	reset();
	setPWMFreq(1000);
}

servo::~servo() {
	delete i2c;
}

void servo::set_servo_open (int channel) {
    //set_servo_open(int channel);
    sleep(2);
    pwm.setPWM(channel, 0, SERVOMAX);
    print(channel,"号舵机开启",SERVOMAX);
}


void servo::set_servo_close (int channel) {
    //set_servo_open(int channel);
    sleep(2);
    pwm.setPWM(channel, 0, SERVOMIN);
    print(channel,"号舵机开启",SERVOMIN);
}


/*
class set_servo_open{
    set_servo_open(int channel);
    sleep(2);
    pwm.setPWM(channel, 0, servo_max);
    //print(channel,"号舵机开启",servo_max);  
}
    

def set_servo_close(channel):
    sleep(2)
    pwm.setPWM(channel, 0, servo_min)
    //print(channel,"号舵机关闭",servo_min)

}
*/
#endif