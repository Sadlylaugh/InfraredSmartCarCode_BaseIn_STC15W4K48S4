#include "OWN51.h"
#include "math.h"
#include "IO.h"

char* prompt[]={
"Send N# to quit.",		//use 0
"Send Y# to start.",	//use 1
"Waiting..."	,				//use 2
"*****************"		//use 3
};

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Delay1ms()		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}
void Delay1us()		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}


void delayms(unsigned ms)
{
	for(ms;ms>0;ms--)
		Delay1ms();
}
void delayus(unsigned us)
{
	for(us;us>0;us--)
		Delay1us();
}

char *consver(char *str)   
{   
	if(*str!='\0')
	{
		char temp;   
		char *s1 = str;   
		char *s2 = str+strlen(str)-1;
		for(;s1 < s2;s1++,s2--)
		{   
			temp = *s1;
			*s1 = *s2;  
			*s2 = temp;   
		}  
	}
	return str;   
}

void itoa(unsigned long num,char* s)
{
	char i=0;
	while(num)
	{
		s[i]=num%10+48;
		num/=10;
		i++;
	}
	s[i]='\0';
	consver(s);
}

unsigned long powOwn(unsigned char x)
{
	unsigned long y=1;
	for(;x>0;x--)
	{
		y=y*10;
	}
	return y;
}
unsigned long atoi(char *s)
{
	unsigned long num=0;
	unsigned char count=0;
	count=strlen(s);
	consver(s);
	for(;count;count--)
	{
		num+=(s[count-1]-48)*powOwn(count-1);
	}
	return num;
}
	