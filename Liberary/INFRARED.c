#include "INFRARED.h"
unsigned int MaxValue=0;
unsigned int MinValue=0;
unsigned int ADValue=0;
unsigned int TotalValue=0;
extern char* prompt[]; 
unsigned int xdata infraredADValue[8]=0;
unsigned int xdata infraredBlackValue[8]=0;//MINValue,电阻分压少,为低值
unsigned int xdata infraredWhiteValue[8]=0;//MAXValue,电阻分压多，为高值

//sbit IR1=P0^0;
//sbit IR2=P0^1;
//sbit IR3=P0^2;
//sbit IR4=P0^3;
//sbit IR5=P0^4;
//sbit IR6=P0^5;
//sbit IR7=P0^6;
//sbit IR8=P0^7;
long map(long x, long in_min, long in_max, long out_min, long out_max)
{
 return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
} 

void scantest()
{
		unsigned char i,j,light=0x01;
	unsigned int count=500;
	while(1)//等待停止
	{
		P0=0xff;
		P0M1=0x00;
		P0M0=0x00;
		for(i=0;i<8;i++)
		{
			P0=~(light<<i);
			delayus(3); 
			ADValue=getAD(i);
//			if(ADValue>1023)
//				count++;
			}
			P0=0xff;
			sendInt(ADValue);
		}
	}
void initInfraredCon()
{
	unsigned char i,j,light=0x01;
	sendStr("Start Infrared initialize.");
	waitForNext();
	sendStr("Wait for \"y#\" to stop the scan.");
//	infraredScan(5);//预扫描一次
	for(i=0;i<8;i++)//获取初值
	{
		infraredBlackValue[i]=10;
		infraredWhiteValue[i]=10;
	}
//	sendStrNNL("In White:");
//	showValue(3);
//	sendStr(" ");
//	sendStrNNL("In Black:");
//	showValue(2);
//	sendStr(" ");
	while(1)//等待停止
	{
		P0=0xff;
		P0M1=0x00;
		P0M0=0x00;
		for(i=0;i<8;i++)
		{
			MaxValue=200;
			MinValue=200;
			ADValue=0;
			TotalValue=0;
			
			P0=~(light<<i);
			delayus(3); 
			for(j=0;j<5;j++)//中位值平均滤波法
			{
				ADValue=getAD(i);

//				sendStrNNL("ADValue:");
//				sendInt(ADValue);
//				if(ADValue>MaxValue)
//					MaxValue=ADValue;
//				else if(ADValue<MinValue)
//					MinValue=ADValue;
				TotalValue+=ADValue;
//				sendStrNNL("TotalValue:");
//				sendIntNNL(TotalValue);
//				sendStrNNL(" ");
//				sendInt(j);
//				delayms(500);
			}
			P0=0xff;
			ADValue=TotalValue/5;
//			ADValue=(TotalValue-MaxValue-MinValue)/(5-2);
//			sendStrNNL("AND SO ON:");
//			sendInt(ADValue);
//							sendStr(prompt[3]);
//			if(ADValue<infraredBlackValue[i])//更新最小值
//				infraredBlackValue[i]=ADValue;
		if(ADValue>infraredWhiteValue[i])//更新最大值
				infraredWhiteValue[i]=ADValue;
		}
		if(hadRecive()&&(getDirection()==1))
			break;
	}
	sendStr(prompt[3]);
	sendStr("The Result is");
	sendStrNNL("In White:");
	showValue(3);
	sendStr(" ");
	sendStrNNL("In Black:");
	showValue(2);
	sendStr(" ");
	sendStr(prompt[3]);
	waitForNext();
}
	
void infraredScan(unsigned char times)
{
	unsigned char i,light=0x01;
	unsigned char j;
	P0=0xff;
	P0M1=0x00;
	P0M0=0x00;
	for(i=0;i<8;i++)
	{
		ADValue=0;
		TotalValue=0;
		
		P0=~(light<<i);
		delayus(3);  

		
//		ADValue=getAD(i);//取初始基准值
//		MaxValue=ADValue;
//		MinValue=ADValue;
		
		for(j=0;j<times;j++)//中位值平均滤波法
		{
			ADValue=getAD(i);
//			if(ADValue>MaxValue)
//				MaxValue=ADValue;
//			else if(ADValue<MinValue)
//				MinValue=ADValue;
			TotalValue+=ADValue;
		}
		P0=0xff;
		ADValue=TotalValue;//-MaxValue-MinValue;
		infraredADValue[i]=ADValue/times;
	}
	sendStr("Infrareds scan compelet.");
}

bit initInfrared()
{
	unsigned char i;
	unsigned char dirct;
	sendStr("Get ready to scan the BLACK surface.");//询问是否进行扫描
	sendStr(prompt[1]);
	sendStr(prompt[0]);
	while(1)
	{
		dirct=getDirection();
		if(dirct==1)//确认开始扫描
		{
			sendStr("Scan Start!");
			infraredScan(20);
			for(i=0;i<8;i++)
			{
				infraredBlackValue[i]=infraredADValue[i];
			}
			break;
		}
		else if(dirct==2)
			return 0;
		else
			sendStr(prompt[2]);
	}
	showValue(2);
	waitForNext();
	
	sendStr("Get ready to scan the White surface.");//询问是否进行扫描
	sendStr(prompt[1]);
	sendStr(prompt[0]);
	while(1)
	{
		dirct=getDirection();
		if(dirct==1)//确认开始扫描
		{
			infraredScan(20);
			for(i=0;i<8;i++)
			{
				infraredWhiteValue[i]=infraredADValue[i];
			}
			break;
		}
		else if(dirct==2)
			return 0;
		else
			sendStr(prompt[2]);
	}
	sendStr("Infrared initialize complete!");
	showValue(3);
	waitForNext();
	return 1;
}

void showValue(unsigned char c)
{
	char i;
	if(c==0)//百分比显示
	{
		for(i=0;i<8;i++)
		{
			//sendIntNNL((infraredADValue[i]-infraredBlackValue[i])*100/(infraredWhiteValue[i]-infraredBlackValue[i]));
			sendIntNNL(map(infraredADValue[i],infraredBlackValue[i],infraredWhiteValue[i],0,100));
		}
	}
	else if(c==1)//原值显示
	{
			for(i=0;i<8;i++)
		{
			sendIntNNL(infraredADValue[i]);
		}
	}
	else if(c==2)//显示黑色基准值
	{
		for(i=0;i<8;i++)
		{
			sendIntNNL(infraredBlackValue[i]);
		}
	}
	else if(c==3)//显示白色基准值
	{
		for(i=0;i<8;i++)
		{
			sendIntNNL(infraredWhiteValue[i]);
		}
	}
}