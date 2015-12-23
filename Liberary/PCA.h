#ifndef _PCA_H_
#define _PCA_H_
#include "OWN51.h"
#include "IO.h"

#define COUNT1US SYSclk/1000000L
//最大捕获脉冲频率1HZ,设定最大PCA溢出次数
#define MAXCRCOUNT COUNT1US*1000000L/65536

#define WHEEL_DIAMETER 48.5 //车轮直径，单位mm
#define WHEEL_RADIUS WHEEL_DIAMETER/2.0 //车轮半径，单位mm
#define WHEEL_CIRCUMFERENCE 2*WHEEL_RADIUS*PI
#define IMPUSLE_TIMES 4

#define ECF 0x01
#define ECCF0 0x01
#define ECCF1 0x01
#define CPS 0x08
#define CAPP0 1<<5//捕捉上升沿
#define CAPP1 1<<5
#define CAPN0 1<<4//捕捉下降沿
#define CAPN1 1<<4

#define PCA_SET_TO_POINT2 1<<4
#define PCA_SET_TO_POINT3 1<<5
enum PCASelect
{
	PCA0,
	PCA1
};
enum PCAMod
{
	capture,
	PWM
};

void initPCA0(enum PCAMod mod);
unsigned int getSpeed();
unsigned long getCycle(enum PCASelect select);

#endif