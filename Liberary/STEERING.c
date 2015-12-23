#include "STEERING.h"
unsigned int xdata highCount;
unsigned int xdata lowCount;
sbit steering=P4^5;

//舵机库使用定时器T3产生50Hz PWM波，周期20ms
//T3工作在普通12分频模式，24Mhz下计数频率为2Mhz
//50Hz下要求T3的装载值为2M/50=40000
//
/*------------------------------------
舵机驱动函数，使用T3产生PWM波，参数为角度，0-180
--------------------------------------*/
void steeringSet(float position)
{

//	steering=0;
//	if(position>POSITION_COUNT||position<0)
//		return;
	if(position<0||position>180)
		return;
	P4M1=0x00;
	P4M0=0xff;
	sendStrNNL("Steer set as ");
	sendInt(position);
	sendStr(" degrees.");
	position=(unsigned int)(POSITION_COUNT*(position/180.0))+MIN_COUNT;
	//position=map(position,0,3550,1200,4750);
	highCount=(65536-position);
//	sendInt(highCount);
	lowCount=65536-(CYCLE_COUNT-position);
//	sendInt(lowCount);
	TL1=highCount%256;
	TH1=highCount/256;
//	TMOD|=1<<4;
//	ET1=1;
//	TR1=1;
	T4T3M&=~T3R;
	T3L=highCount%256;
	T3H=highCount/256;
	IE2|=ET3;
	T4T3M|=T3R;
	steering=1;
	EA=1;
}
void steeringitr() interrupt 19//3
{
	if(steering==1)
	{
		T4T3M&=~T3R;
		T3L=lowCount%256;
//		TL1=lowCount%256;
//		TH1=lowCount/256;
		steering=0;
		T3H=lowCount/256;
		T4T3M|=T3R;
	}
	else if(steering==0)
	{
		T4T3M&=~T3R;
		T3L=highCount%256;
//		TL1=highCount%256;
//		TH1=highCount/256;
		steering=1;
		T3H=highCount/256;
		T4T3M|=T3R;
	}
}
	