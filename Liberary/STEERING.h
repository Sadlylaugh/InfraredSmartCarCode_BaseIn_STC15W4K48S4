#ifndef _STEERING_H_
#define _STEERING_H_
#include "IO.h"
#include "OWN51.h"
#include "W4K51.h"
#define CYCLE_COUNT SYSclk/12/50  					//一个50Hz周期需要的定时器计数次
#define MIN_COUNT 1200//CYCLE_COUNT*1/100		//适配舵机处最小角时的高电平计数次数
#define POSITION_COUNT 3550									//舵机运动的计数宽度，1200+3550时为180度，最大
#define T3R 1<<3
#define T3x12 1<1
#define ET3 1<<5


//舵机库使用定时器T0产生50Hz PWM波，周期20ms
//T3工作在普通12分频模式，24Mhz下计数频率为2Mhz
//50Hz下要求T3的装载值为2M/50=40000
//占空比在5%到10%时舵机有动作（1ms到2ms的高电平）
//1.5ms为置中
void steeringSet(float position);
#endif