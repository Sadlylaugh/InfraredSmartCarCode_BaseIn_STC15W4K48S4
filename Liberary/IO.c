//若无特别说明,工作频率一般为24MHz
//单片机定时器工作在1T模式
//串行口1使用定时器T1的16位自动重载模式作为波特率发生器
//串行口2使用定时器T2的16位自动重载模式作为波特率发生器
//工作在模式1（10位无校验）
#include "IO.h"
#include <stdio.h>
#include <intrins.h>
bit enSerial1=0;
bit enSerial2=0;									//用于判断串口是否已经初始化

bit busyU1=0;											//用于判断串行口1是否在发送
bit recDoneU1=0;									//用于判断串行口1是否有接收

bit busyU2=0;
bit recDoneU2=0;

bit busyU3=0;	//用于判断串行口1是否在发送
bit recDoneU3=0;									

char xdata IOBufU1[MAXIOBUFU];		//串行口1的接收缓冲区
char xdata IOBufU2[1];   //串行口2的接收缓冲区
char xdata IOBufU3[1];   //串行口2的接收缓冲区
char IOTopU1=0;
char IOTopU2=0;
char IOTopU3=0;


/*----------------------
串口的初始化程序，参数指定为哪个串口
串行口1使用T2发生波特率
串行口2使用T2发生波特率
-----------------------*/
void initSerial(enum UartSerial serial)
{
	switch(serial)
	{
		case serial1:												//串行口1使用T2定时器
			SCON = 0x50; 
			AUXR |= 1<<2;     								//定时器2为1T模式
		  AUXR|=1;													//释放T1
			T2L = (65536-SYSclk/(BAUD1*4))%256; 	  //设置波特率重装值
			T2H = (65536-SYSclk/(BAUD1*4))/256;	   	//波特率定义为115200
			AUXR |=0x10;                   		//启动定时器T2
			ES=1;                   			//使能串口中断
			EA = 1;
			enSerial1=1;
		break;
		case serial2:
			//P_SW2|=1;//串行口2放到RXD2_2,TXD2_2，然而实测无法工作
			S2CON|=0x10;//00010000 8位UART
			AUXR |= 1<<2;     								//定时器2为1T模式
			T2L = (65536-SYSclk/(BAUD2*4))%256; 	  //设置波特率重装值
			T2H = (65536-SYSclk/(BAUD2*4))/256;	   	//波特率定义为115200
			AUXR |=0x10;                   		//启动定时器T2
			IE2 |= ES2;                   			//使能串口中断
			EA = 1;
		case serial3:
			S3CON|=1<<4;//00010000						//8位UART，允许接收
		//串行口使用定时器T2做波特率发生器
			AUXR |= 1<<2;     								//定时器2为1T模式
			T2L = (65536-SYSclk/(BAUD3*4))%256; 	  //设置波特率重装值
			T2H = (65536-SYSclk/(BAUD3*4))/256;	   	//波特率定义为115200
			AUXR |=0x10;                   		//启动定时器T2
			IE2 |= ES3;                   			//使能串口中断
			EA=1;
		break;
		default:
			break;
	}
}

/*----------------------------
串口1中断服务程序
-----------------------------*/
void uartU1() interrupt 4
{
   if (RI)
   {
     RI = 0;                	  //清除RI位
		 IOBufU1[IOTopU1]=SBUF;	
		 if(IOBufU1[IOTopU1]=='#'||IOTopU1==20)	//若接收到结尾为#的指令或缓冲区超出则结束接收
		 {
			 recDoneU1=1;
			 IOBufU1[IOTopU1]='\0';
			 IOTopU1=0;
		 }
		 else
			 IOTopU1++;
   }
   if (TI)
   {
      TI = 0;                 	//清除TI位
      busyU1 = 0;               //清忙标志
   }
}
/*----------------------------
串口2中断服务程序
-----------------------------*/
void uartU2() interrupt 8
{
	if (S2CON & S2RI)
	{
		 S2CON &= ~S2RI;  	  				//清除S2RI位
		 IOBufU2[IOTopU2]=S2BUF;	
		 if(IOBufU2[IOTopU2]=='#'||IOTopU2==20)	//若接收到结尾为#的指令或缓冲区超出则结束接收
		 {
			 recDoneU2=1;
			 IOBufU2[IOTopU2]='\0';
			 IOTopU2=0;
		 }
	}
	if (S2CON & S2TI)
	{
		S2CON &= ~S2TI;
		busyU2 = 0;
	}
}
/*----------------------------
串口3中断服务程序
-----------------------------*/
void uartU3() interrupt 17
{
	if (S3CON & S3RI)
	{
		 S3CON &= ~S3RI;  	  				//清除S2RI位
		 IOBufU3[IOTopU3]=S3BUF;	
		 if(IOBufU3[IOTopU3]=='#'||IOTopU3==20)	//若接收到结尾为#的指令或缓冲区超出则结束接收
		 {
			 recDoneU3=1;
			 IOBufU3[IOTopU3]='\0';
			 IOTopU3=0;
		 }
	}
	if (S3CON & S3TI)
	{
		S3CON &= ~S3TI;
		busyU3 = 0;
	}
}
/*----------------------------
串口1发送串口数据
----------------------------*/
void sendByte(unsigned char byte)
{
    while (busyU1);               //等待前面的数据发送完成
    busyU1 = 1;
    SBUF = byte;                 //写数据到UART数据寄存器
}
/*----------------------------
串口2发送串口数据
----------------------------*/
void sendByteU2(unsigned char byte)
{
    while (busyU2);               //等待前面的数据发送完成
    busyU2 = 1;
    S2BUF = byte;                 //写数据到UART数据寄存器
}
/*----------------------------
串口3发送串口数据
----------------------------*/
void sendByteU3(unsigned char byte)
{
    while (busyU3);               //等待前面的数据发送完成
    busyU3 = 1;
    S3BUF = byte;                 //写数据到UART数据寄存器
}

/*----------------------------
串口1发送字符串
----------------------------*/
void sendStr(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        sendByte(*s++);         //发送当前字符
    }
		putchar('\n');
		sendByte('\n');							//自带一个换行
}

void sendStrNNL(char *s)
{
	  while (*s)                  //检测字符串结束标志
    {
        sendByte(*s++);         //发送当前字符
    }
		sendByte(' ');
		sendByte(' ');
}
/*----------------------------
串口2发送字符串
----------------------------*/
void sendStrU2(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        sendByteU2(*s++);         //发送当前字符
    }
		sendByteU2('\n');							//自带一个换行
}
/*----------------------------
串口3发送字符串
----------------------------*/
void sendStrU3(char *s)
{
    while (*s)                  //检测字符串结束标志
    {
        sendByteU3(*s++);         //发送当前字符
    }
		sendByteU3('\n');							//自带一个换行
}
/*----------------------------
串口1发送数字，带两位小数
----------------------------*/
void sendInt(unsigned long num)
{
//	ES=0;
//	TI=1;
//	printf("%0.2f \n",num);
//	while(TI);
//	TI=0;
//	ES=1;

	char str[16];
	if(num==0)
	{
		sendStr("0");
		return;
	}
	itoa(num,str);
	sendStr(str);
}
void sendIntNNL(unsigned long num)
{
	char str[15];
	if(num==0)
	{
		sendStrNNL("0");
		return;
	}
	itoa(num,str);
	sendStrNNL(str);
	//ES=0;
	//TI=1;
	//printf();
	//while(TI);
	//TI=0;
	//ES=1;
}
/*----------------------------
串口2发送数字，带两位小数
----------------------------*/
void sendIntU2(float num)
{
	S2CON&=~S2REN;//关闭S2REN
	S2CON|=S2TI;//置位S2TI
	printf("%0.2f \n",num);
	while(S2CON&S2TI);
	S2CON&=~S2TI;
	S2CON|=S2REN;
}
/*----------------------------
串口3发送数字，带两位小数
----------------------------*/
void sendIntU3(float num)
{
	S3CON&=~S3REN;//关闭S2REN
	S3CON|=S3TI;//置位S2TI
	printf("%0.2f \n",num);
	while(S2CON&S3TI);
	S3CON&=~S3TI;
	S3CON|=S3REN;
}


/*----------------------------
串口1接收字符串
直接返回接收缓冲区指针
----------------------------*/
char* getRecived()
{
	if(!recDoneU1)
		return NULL;
	recDoneU1=0;
	sendStr("U1Recived");
	sendStr(IOBufU1);
	return IOBufU1;
}
bit hadRecive()
{
	if(!recDoneU1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/*----------------------------
串口2接收字符串
直接返回接收缓冲区指针
----------------------------*/
char* getRecivedU2()
{
	if(!recDoneU2)
		return NULL;
	recDoneU2=0;
	sendStrU2("U2Recived\n");
	return IOBufU2;
}
bit hadReciveU2()
{
	if(!recDoneU2)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
/*----------------------------
串口3接收字符串
直接返回接收缓冲区指针
----------------------------*/
char* getRecivedU3()
{
	if(!recDoneU3)
		return NULL;
	recDoneU3=0;
	sendStrU3("U3Recived\n");
	return IOBufU3;
}
bit hadReciveU3()
{
	if(!recDoneU3)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}