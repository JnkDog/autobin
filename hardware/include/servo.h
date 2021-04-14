#ifndef _servo_H
#define _servo_H

#include <iostream>
#include "PCA9685.h"


/* Depending on your servo make, the pulse width min and max may vary, you  want these to be as small/large as possible without hitting the hard stop
 for max range. You'll have to tweak them as necessary to match the servos you
have!*/

#define SERVOMIN  150 
// this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 
// this is the 'maximum' pulse length count (out of 4096)

using namespace std;

class servo 
{
    
    public: 
    servo();
	virtual ~servo();
    
    void set_servo_open(int channel);
    void set_servo_close(int channel);
  
   // PCA9685.setPWMFreq(60); //设置频率
    
};


#endif