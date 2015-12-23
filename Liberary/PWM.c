#include "PWM.h"
//PWM2:P3.7, PWM3:P2.1, PWM4:P2.2,
//PWM5:P2.3, PWM6:P1.6, PWM7:P1.7
//#if(SWITCH_PWM2)
sbit PWM2=P3^7;
sbit PWM3=P2^1;
sbit PWM4=P2^2;
sbit PWM5=P2^3;
sbit PWM6=P1^6;
sbit PWM7=P1^7;


/*------------------------------------
PWM端口初始化程序，通过指定位置进行端口初始化
--------------------------------------*/
void initPWM(unsigned char PWMPoint)
{
	P_SW2|=1<<7;
	//PWM时钟使用系统时钟，不分频
	//暂时设定PWM频率为38.5Khz,即每周期2.6us
	PWMCH=PWM_CYCLE_LOADVALUE/256;
	PWMCL=PWM_CYCLE_LOADVALUE%256;
	//初始电平为高电平
	//STC的坑爹之处，必须使用置位|才有效!!!!!!!!!!!!!!
	//PWMCFG|=0x3f;

	switch(PWMPoint)
	{
		case 0:
			break;
		case 1:
			break;
		case 2:	//参数为2时为PWM2/D28/P3.7_INT3_TxD_2
			P3M0|=1<<7;				//设P3.7为强推挽输出
			P3M1&=~(1<<7);
			PWMCFG|=1;//置位有影响，置位为正占空比
			PWM2=0;
			break;
		case 3:
			P2M0|=1<<1;				//设P2.1为强推挽输出
			P2M1&=~(1<<1);
			PWMCFG|=1<<1;//随机 置位无影响.起始为随机
			PWM3=0;
			//PWMCR|=1<<1;			//通道PWM输出使能
			break;
		case 4:
			P2M0|=1<<2;				//设P2.2为强推挽输出
			P2M1&=~(1<<2);
			//PWMCFG|=1<<2;//置位有影响，置位为正占空比
			PWM4=0;
			break;
		case 5:
			P2M0|=1<<3;				//设P2.3为强推挽输出
			P2M1&=~(1<<3);
			PWMCFG|=1<<3;//随机 置位有影响，置位为反占空比
			PWM5=0;
			break;
		case 6:
			P1M0|=1<<6;				//设P1.6为强推挽输出
			P1M1&=~(1<<6);
		  PWMCFG|=1<<4;//置位无影响，为正占空比
			PWM6=0;
			break;
		case 7:
			P1M0|=1<<7;				//设P1.7为强推挽输出
			P1M1&=~(1<<7);
			PWMCFG|=1<<5;//置位无影响，为正占空比
			PWM7=0;
			break;
		default:
			break;
	}
}
/*------------------------------------
启动PWM输出，同时设定占空比，占空比设定从0到1000
--------------------------------------*/
void startPWM(unsigned char PWMPoint,unsigned int dutyRation)
{
	if(dutyRation>1000)
		return;
	switch(PWMPoint)
	{
		case 0:
			break;
		case 1:
			break;
		//方波起始为高电平，在第一个翻转点后直接进行到周期结束
		case 2:				//P3.7
			if(dutyRation==0)
			{
				PWMCR&=~1;
				PWM2=0;
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~1;
				PWM2=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM2T2=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM2T1=PWM_CYCLE_LOADVALUE+100/1000;
				//PWMCFG|=1;
				PWMCR|=1;						//通道2PWM输出使能
				PWMCR|=1<<7;
				return;
			}
		case 3:				//P2.1
			if(dutyRation==0)
			{
				PWMCR&=~(1<<1);
				PWM3=0;
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~(1<<1);
				PWM3=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM3T1=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM3T2=PWM_CYCLE_LOADVALUE+100/1000;;
				PWMCFG|=1<<1;//置位无效
				PWMCR|=1<<1;			//通道3PWM输出使能
				PWMCR|=1<<7;
				return;
			}
			break;
		case 4://P2.2
			if(dutyRation==0)
			{
				PWMCR&=~(1<<2);
				PWM4=0;
								sendStr("yes2");
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~(1<<2);
				PWM4=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM4T1=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM4T2=PWM_CYCLE_LOADVALUE+100/1000;;
				PWMCFG|=1<<2;
				PWMCR|=1<<2;			//通道4PWM输出使能
				PWMCR|=1<<7;
				return;
			}
			break;
		case 5://P2.3
			if(dutyRation==0)
			{
				PWMCR&=~(1<<3);
				PWM5=0;
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~(1<<3);
				PWM5=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM5T1=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM5T2=PWM_CYCLE_LOADVALUE+100/1000;
				PWMCFG|=1<<3;
				PWMCR|=1<<3;			//通道5PWM输出使能
				PWMCR|=1<<7;
				return;
			}
		case 6:
			if(dutyRation==0)
			{
				PWMCR&=~(1<<4);
				PWM6=0;
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~(1<<4);
				PWM6=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM6T1=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM6T2=PWM_CYCLE_LOADVALUE+100/1000;
				PWMCFG|=1<<4;
				PWMCR|=1<<4;			//通道6PWM输出使能
				PWMCR|=1<<7;
				return;
			}
		case 7:
			if(dutyRation==0)
			{
				PWMCR&=~(1<<5);
				PWM7=0;
				return;
			}
			else if(dutyRation==1000)
			{
				PWMCR&=~(1<<5);
				PWM7=1;
				return;
			}
			else
			{
				PWMCR&=~(1<<7);
				PWM7T1=PWM_CYCLE_LOADVALUE*dutyRation/1000;
				PWM7T2=PWM_CYCLE_LOADVALUE+100/1000;
				PWMCFG|=1<<5;
				PWMCR|=1<<5;			//通道7PWM输出使能
				PWMCR|=1<<7;
				return;
			}
		default:
			break;
	}
}
	