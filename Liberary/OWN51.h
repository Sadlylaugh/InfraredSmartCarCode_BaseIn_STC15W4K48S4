#ifndef _OWN51_H_
#define _OWN51_H_
#include "W4K51.h"
#include "string.h"
#include <intrins.h>

#define SYSclk 24000000L          //系统频率@24Mhz
#define BAUD1 115200L             //串口1通信波特率
#define BAUD2 115200L							//串口2通信波特率
#define BAUD3 115200L	   					//串口3通信波特率
#define PWM_FREQUENCY 40000L      //6口PWM频率

#define PI 3.1416F

long map(long x, long in_min, long in_max, long out_min, long out_max);
void Delay1ms();		//@24.000MHz
void delayms(unsigned ms);
void Delay1us();		//@24.000MHz
void delayus(unsigned us);


char *consver(char *str);
void itoa(unsigned long num,char* s);
unsigned long atoi(char *s);
unsigned long powOwn(unsigned char x);//10的X次方
#endif