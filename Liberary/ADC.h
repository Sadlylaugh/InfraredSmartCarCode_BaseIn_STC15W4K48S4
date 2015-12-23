#ifndef _ADC_H_
#define _ADC_H_
#include "W4K51.h"

#define ADC_POWER 0x80
#define ADC_START 0x08
#define ADC_FLAG 0x10

#define ADC_SPEEDLL 0x00
#define ADC_SPEEDL 0x20
#define ADC_SPEEDH 0x40
#define ADC_SPEEDHH 0x60

void initADC(); //初始化所有AD口
unsigned int getAD(char ch); //获取指定口的AD值
#endif