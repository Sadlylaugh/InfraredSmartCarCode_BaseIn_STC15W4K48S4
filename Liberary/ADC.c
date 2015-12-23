#include "ADC.h"
#include "IO.h"
#include "OWN51.h"
#include <intrins.h>

bit ADC_DONE=0;

void initADC()
{
	P1ASF=0xff;//打开所有P1口的模拟功能
	ADC_CONTR=1<<7;//打开ADC电源
	//delayms(100);
	ADC_RES=0;
	ADC_RESL=0;
	EADC=1;//ADC中断
	EA=1;
}

unsigned int getAD(char ch)
{
	ADC_RES=0;
	ADC_RESL=0;
	ADC_CONTR=ADC_POWER|ADC_SPEEDL|ADC_START|ch;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	while(!ADC_DONE);
	ADC_DONE=0;
	return ADC_RES*4+ADC_RESL;
}

void adc_itr() interrupt 5 
{
	ADC_CONTR&=~ADC_FLAG;//清除ADC_FLAG位
	ADC_DONE=1;
}
