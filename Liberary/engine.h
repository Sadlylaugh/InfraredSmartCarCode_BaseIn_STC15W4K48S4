#ifndef _ENGINE_H_
#define _ENGINE_H_
#include "PWM.h"
//@24Mhz,PWM通道频率40KHz，PWM有600级可调
//电机启动需要占空比为40%，即40%-100%处占空比可调节电机功率
//使用PWM3(P2.1)h和PWM4(P2.2)做驱动
#define ENGINE_PWMPOINT1 3
#define ENGINE_PWMPOINT2 4
sbit INT2=P2^0;
//#define MIN_PWMLEVEL 400

void initEngine();
void engineSet(unsigned int speed);
void carBrake(unsigned char level);
#endif