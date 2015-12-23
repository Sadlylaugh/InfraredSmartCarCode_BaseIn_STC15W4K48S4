#ifndef _PWM_H_
#define _PWM_H_
//STC15W4K32S4系列的PWM模块头文件
//使用STC15W4K32S4系列的6路增强型PWM波形发生器
//PWM时钟输入选择系统时钟
//输出端口为
//PWM2:P3.7, PWM3:P2.1, PWM4:P2.2,
//PWM5:P2.3, PWM6:P1.6, PWM7:P1.7
#include "OWN51.h"
#include "W4K51.h"
#include "IO.h"


#define PWM_CYCLE_LOADVALUE SYSclk/PWM_FREQUENCY //设定每周期计数次数，@40KHz PWM @24Mhz下每周期计数600次

void initPWM(unsigned char PWMPoint);
void startPWM(unsigned char PWMPoint,unsigned int dutyRation);

#endif