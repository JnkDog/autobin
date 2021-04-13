#ifndef __PCA9685_H
#define __PCA9685_H


#define PCA9685_ADDRESS			0x40			//pca9685地址
#define PCA9685_CLOCK_FREQ		25000000		//PWM频率25MHz
#define PCA9685_MODE1			0x00
#define PCA9685_MODE2			0x01
#define PCA9685_PRE_SCALE		0xFE
#define PCA9685_LED0_ON_L		0x06
#define PCA9685_LED0_ON_H		0x07
#define PCA9685_LED0_OFF_L		0x08
#define PCA9685_LED0_OFF_H		0x09
#define PCA9685_LED_SHIFT		4


class PCA9685
{
	public:
		//PCA9685(uint8_t addr = 0x40);
		//virtual ~PCA9685();
		
        PCA9685();
        void reset();
        void setPWMFreq(unsigned short freq);
        void setPWM(unsigned char channel, unsigned short on, unsigned short value);
        void setPulse(unsigned char channel, unsigned short pulse);  
	/*
	private:
		uint8_t devID;
		int fd;
		double _freq;
    */
};

/*
bool PCA9685Init();
void ResetPca9685();
void PCA9685SetPwmFreq(unsigned short freq);
void PCA9685SetPwm(unsigned char channel, unsigned short on, unsigned short value);
void SetServoPulse(unsigned char channel, unsigned short pulse);
*/

#endif