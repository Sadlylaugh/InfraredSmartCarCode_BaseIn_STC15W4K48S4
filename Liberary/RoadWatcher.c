#include "RoadWatcher.h"

unsigned char IRValueLeft[7]=0;
unsigned char	IRValueRight[7]=0;
bit IR0;
extern unsigned char carDirectionValue;

extern bit carDirection;
extern bit goodDirectionScan;


void watcherScan()
{
//	sendStr("IRWatcher is Scaning..");
	IRValueLeft[6]=	IRLeft6;
	IRValueLeft[5]=	IRLeft5;
	IRValueLeft[4]=	IRLeft4;
	IRValueLeft[3]=	IRLeft3;
	IRValueLeft[2]=	IRLeft2;
	IRValueLeft[1]=	IRLeft1;
	
	IR0=IR0_Now;
	
	IRValueRight[1]=	IRRight1;
	IRValueRight[2]=	IRRight2;
	IRValueRight[3]=	IRRight3;
	IRValueRight[4]=	IRRight4;
	IRValueRight[5]=	IRRight5;
	IRValueRight[6]=	IRRight6;
}

unsigned char getcarDirection()
{
	unsigned char i=0;
	unsigned char 
		IRCountLeft=0,//左边传感器的权值
		IRCountRight=0,//右边传感器的权值
		IRLightCountLeft=0,//左边传感器感应的数量
		IRLightCountRight=0;//右边传感器感应的数量
	watcherScan();
	for(i=1;i<7;i++)//取左边的传感器的值
	{
		if(IRValueLeft[i]==1)
			IRLightCountLeft++;
		IRCountLeft+=(IRValueLeft[i]*i*2);
	}

	for(i=1;i<7;i++)//取右边的传感器的值
	{
		if(IRValueRight[i]==1)
			IRLightCountRight++;
		IRCountRight+=(IRValueRight[i]*i*2);
	}
//	sendStr("*****************");
//	sendIntNNL(IRCountLeft);
//	sendStr("IRCountLeft");
//	sendIntNNL(IRCountRight);
//	sendStr("IRCountRight");
//	for(i=6;i>0;i--)
//			sendIntNNL(IRValueLeft[i]);
//		sendStr("Left");

//	for(i=1;i<7;i++)
//			sendIntNNL(IRValueRight[i]);
//		sendStr("Right");
//	sendIntNNL(IR0);
//	sendStr("IR0");
//	sendInt(IRLightCountLeft);
//	sendInt(IRLightCountRight);
//	sendStr("*****************");
	
	//车子处于起始点
	if(IRLightCountLeft==4&&IRLightCountRight==4&&IR0==1)
	{

		goodDirectionScan=1;
		return 21;
	}
	
	//车子处于交叉线路上
	else if(IRLightCountLeft==6&&IRLightCountRight==6&&IR0==1)
	{
		goodDirectionScan=1;
		return 22;
	}
	
	//车子悬空或冲出跑道
	else if(IRLightCountLeft==0&&IRLightCountRight==0&&IR0==0)
	{
		goodDirectionScan=1;
		return 23;
	}
	
	//左边有感应且右边无感应
	else if(IRLightCountLeft>0&&IRLightCountRight==0)
	{
		if(IR0==1)
		{
			goodDirectionScan=1;
			carDirection=0;//车子左偏
			return IRCountLeft/(IRLightCountLeft+1);
		}
		else
		{
			goodDirectionScan=1;
			carDirection=0;//车子左偏
			return IRCountLeft/IRLightCountLeft;
		}
	}
	
	//右边有感应且左边无感应
	else if(IRLightCountRight>0&&IRLightCountLeft==0)
	{
		if(IR0==1)//有两个传感器有感应
		{
			goodDirectionScan=1;
			carDirection=1;//车子右偏
			return IRCountRight/(IRLightCountRight+1);	
		}
		else
		{
			goodDirectionScan=1;
			carDirection=1;//车子左偏
			return IRCountRight/IRLightCountRight;
		}
	}
	
	//车子完全直行
	else if(IRCountRight==2&&IRCountLeft==2&&IR0==1)
	{
		goodDirectionScan=1;
		return 20;//返回一个特殊值
	}
	else if(IRCountRight==0&&IRCountLeft==0&&IR0==1)
	{
		goodDirectionScan=1;
		return 20;//返回一个特殊值
	}
	
	//不符合以上三种情况
	else
	{
		goodDirectionScan=0;
		return 0;
	}
}