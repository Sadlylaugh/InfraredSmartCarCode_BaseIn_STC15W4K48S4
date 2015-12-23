//捕获模式下，捕获脉冲频率最大为系统时钟SYSclk
#include "PCA.h"

unsigned int CRCount;
unsigned int firstCapture;
unsigned int secondCapture;
bit PCA0Captured;
unsigned int posi=0;



void initPCA0(enum PCAMod mod)
{
	switch(mod)
	{
		case capture://捕获模式
			AUXR1|=PCA_SET_TO_POINT2;//切换到第二管脚
			CMOD=CPS|ECF;//00001001 CPS2=1;ECF=1;
			CCAPM0=CAPN0|CAPP0|ECCF0;//00010001 CAPP0=0 CAPN0=1;ECCF0=1;
			firstCapture=0;
			secondCapture=0;
			CRCount=0;
			CL=0;
			CH=0;
			CR=1;
			EA=1;
		break;
		case PWM:
			break;
	}
}

unsigned int getSpeed()
{
	return WHEEL_CIRCUMFERENCE*getCycle(PCA0)/(IMPUSLE_TIMES);
}

	
/*------------------------------------
获取PCA的捕获脉冲频率
脉冲最高频率为系统时钟
最低频率为1/100,000us,即最大周期不超过100ms
--------------------------------------*/
unsigned long getCycle(enum PCASelect select)
{
//				firstCapture=0;
//			secondCapture=0;
//			CL=0;
//			CH=0;
//			CR=1;
//			EA=1;
	switch(select)
	{
		case PCA0://PCA模块0
			initPCA0(capture);
			while(!PCA0Captured);//等待捕获完成
			PCA0Captured=0;
			if(CRCount==0)				//如果捕获发生在PCA溢出之前
			{
//				return SYSclk/(secondCapture-firstCapture);
				return (secondCapture-firstCapture);
			}
			else									//如果捕获发生在PCA溢出之后
			{
				return SYSclk/((65536-firstCapture)+secondCapture+65536*CRCount);
			}
			break;
		case PCA1:
			break;
	}
}
/*------------------------------------
PCA中断处理程序
--------------------------------------*/
void PCAIntrrupt() interrupt 7
{
	if(CF)//如果是PCA溢出导致中断
	{
		CRCount++;
		if(CRCount>MAXCRCOUNT)//如果超过限定频率，直接返回最大频率
		{
			CCAPM0&=~ECCF0;//关闭ECCP，停止捕获中断
			CR=0;//关闭计数器
			PCA0Captured=1;
		}
		CF=0;
		return;
	}
	else if(CCF0)//如果发生捕获中断
	{
		if(!firstCapture)//如果是第一次捕获
		{
			firstCapture=CCAP0H*256+CCAP0L;
			CCF0=0;
			return;
		}
		else if(firstCapture&&!secondCapture)//如果是第二次捕获
		{
			secondCapture=CCAP0H*256+CCAP0L;
			CCF0=0;
			CCAPM0&=~1;//关闭ECCP，停止捕获中断
			CR=0;//关闭计数器
			PCA0Captured=1;;
			return;
		}
//		else
//		{
//			CCF0=0;
//			return;
//		}
	}
}
