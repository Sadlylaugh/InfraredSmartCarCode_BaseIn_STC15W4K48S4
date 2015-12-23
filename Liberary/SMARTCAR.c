#include "SMARTCAR.h"
unsigned char carDirectionValue=0;
//carDirection 为1时偏左，为2时偏右
bit carDirection=0;
bit goodDirectionScan=0;

bit carOutRoad=0;

int speedProportion=8;//比例常数 Proportional Const
int speedIntegral=3;//积分常数 Integral Const
int speedDerivative=1;//微分常数 Derivative Const
int speedPrevError=0;//Error[-2] 
int speedLastError=0;//Error[-1] 
int speedError=0;//Error[0]
int speedOut=0;//outcome

int directionProportion=2;//比例常数 Proportional Const
int directionIntegral=0;//积分常数 Integral Const
int directionDerivative=1;//微分常数 Derivative Const
int directionLastError=0;//Error[-1]
int directionPrevError=0;//Error[-2] 
int directionError=0;//Error[0]
int directionOut=0;//outcome

unsigned char lastPosition;
//整车系统初始化
void carSystemInit()
{
	initSerial(serial1);
	initPCA0(capture);
	initEngine();
	sendStr("Car System OnLine!");
}

//车辆舵机方向设置
void carSteerSet(float position)
{
	sendStrNNL("car Steel Set as:");
	sendInt((long)position);
	if(position==lastPosition)
		return;
	else if(position>0)
	{             
		if(position==lastPosition)
			return;
		lastPosition=position;
		if(carDirection==0)
			steeringSet(90+position);
		else if(carDirection==1)
			steeringSet(90-position);
	}
	else if(position==0)
	{
		steeringSet(90);
	}
}
//车辆速度设置
void carSpeedSet(unsigned int speed)
{
	sendStrNNL("car Speed Set as:");
	sendInt(speed);
	engineSet(speedPID(speed));
}
//获取车辆速度，取四次结果的平均值
unsigned int getCarSpeed()
{
	unsigned char i;
	unsigned int speedNow=0;
	for(i=0;i<4;i++)
	{
		speedNow+=getSpeed();
	}
	sendStrNNL("Car Speed Now:");
	sendInt(speedNow/4);
	return speedNow/4;
}
//整车控制主程序
void carControl()
{
	unsigned char driftLevel=0;
	goodDirectionScan=0;
	sendStr("***************************");
	if(carOutRoad)
	{
		carSpeedSet(0);
		return;
	}
//	while(!goodDirectionScan)
//	{
		driftLevel=getcarDirection();
//	}
	if(carDirection==0)
		sendStr("car in left");
	else if(carDirection==1)
		sendStr("car in right");
	sendStrNNL("driftLevel");
	sendInt(driftLevel);
	//车子左右偏
	if(driftLevel>0&&driftLevel<13)
	{
		carSteerSet(atan(driftLevel/17.0)*180/Pi);
//		carSpeedSet(3000*(13-driftLevel)/13.0);
		return;
	}
	//车处于直道
	else if(driftLevel==20)
	{
		carSteerSet(0);
//		carSpeedSet(3000);
		return;
	}
	//车子处于起点
	else if(driftLevel==21)
	{
		carSteerSet(0);
//		carSpeedSet(3000);
		return;
	}
	//车子处于交叉线路上
	else if(driftLevel==22)
	{
		return;
	}
	
	else if(driftLevel==23)
	{
		carSpeedSet(0);
//		carOutRoad=1;
		return;
	}
	else
	{
		sendStr("Error");
		return;
	}
}
		
//速度的PID计算
int speedPID(expcSpeed)
{
	int speedError;
  speedError=expcSpeed-getCarSpeed();//计算偏差
     //进行增量式PID计算 
  speedOut=speedOut+
      speedProportion*(speedError-speedLastError)+
      speedIntegral*speedError+
      speedDerivative*(speedError+speedPrevError-2*speedLastError);

     //进行误差更新
  speedPrevError=speedLastError;
  speedLastError=speedError;
    
     //上下限幅处理
  if(speedOut<0)
     speedOut=0;
  if(speedOut>600)
     speedOut=600;
	sendStrNNL("SpeedPID Out Value is:");
	sendInt(speedOut);
  return speedOut;
}

//车辆方向PID计算
int directionPID(char directionNow)
{
	int directionError;
	if(carDirection==0)
   directionError=-directionNow;
	else if(carDirection==1)
		directionError=directionNow;
     //进行增量式PID计算 
  directionOut=directionOut+
      directionProportion*(directionError-directionLastError)+
      directionIntegral*directionError+
      directionDerivative*(directionError+directionPrevError-2*directionLastError);

     //进行误差更新
  directionPrevError=speedLastError;
  directionLastError=speedError;
    
     //上下限幅处理
  if(directionOut<=0&&directionOut>-36)
	{
			sendStrNNL("DirectionPID Out Value is:");
			sendInt(-directionOut);
     return -directionOut;
	}
  else if(directionOut>0&&directionOut<36)
	{
		 sendStrNNL("DirectionPID Out Value is:");
		 sendInt(-directionOut);
     return directionOut;//DA转换为8位，最大数值为255，故当大于255时，限制为255
	}
	else if(directionOut<-35||directionOut>35)
	{
		sendStrNNL("DirectionPID Out Value is:");
		sendInt(35);
		return 35;
	}
}
void carTest()
{
	unsigned int carSpeed=200;
	unsigned char steer=60;
	initSerial(serial1);
	initEngine();
//	initPCA0(capture);
	while(1)
	{
//		if(carSpeed>500)
			
//		if(steer>120)
//			steer=60;
//		engineSet(carSpeed);
		for(steer=55;steer<126;steer++)
		{
			steeringSet(steer);
			delayms(10);
		}
		for(steer=125;steer>54;steer--)
		{
			steeringSet(steer);
			delayms(10);
		}
	}
}
