#include "engine.h"
int lastpower=0;

void initEngine()
{
	initPWM(ENGINE_PWMPOINT1);
	initPWM(ENGINE_PWMPOINT2);
	INT2=0;
	P2M0|=1;
	sendStr("Engine initialize complete.");
}

//speed的范围为0-600
void engineSet(int power)
{
	if( power<-400|| power>600)
		return;
	lastpower=power;
	if(power>=0)
	{
		startPWM(ENGINE_PWMPOINT1,map(power,0,600,0,1000));
		startPWM(ENGINE_PWMPOINT2,0);
			sendStrNNL("Engine run in positive");
		sendIntNNL(map(power,0,600,0,100));
		sendStr("%");
	}
	else if(power<0)
	{
		startPWM(ENGINE_PWMPOINT2,map(-power,0,600,0,1000));
		startPWM(ENGINE_PWMPOINT1,0);
		sendStrNNL("Engine run in negative");
		sendIntNNL(map(-power,0,400,0,100));
		sendStr("%");
	}
}
//void carBrake(unsigned char level)
//{
//	switch(level)
//	{
//		case 0:
//			startPWM(ENGINE_PWMPOINT1,600);
//			startPWM(ENGINE_PWMPOINT2,600);
//		case 1:
//			startPWM(ENGINE_PWMPOINT1,0);
//			startPWM(ENGINE_PWMPOINT2,0);
//		break;
//		case 2:
//			startPWM(ENGINE_PWMPOINT1,0);
//			startPWM(ENGINE_PWMPOINT2,100);
//			delayms(1);
//			startPWM(ENGINE_PWMPOINT1,lastSpeed);
//			startPWM(ENGINE_PWMPOINT2,0);
//		break;
//		case 3:
//			startPWM(ENGINE_PWMPOINT1,0);
//			startPWM(ENGINE_PWMPOINT2,300);
//			delayms(1);
//			startPWM(ENGINE_PWMPOINT1,lastSpeed);
//			startPWM(ENGINE_PWMPOINT2,0);
//		break;
//		case 4:
//			startPWM(ENGINE_PWMPOINT1,0);
//			startPWM(ENGINE_PWMPOINT2,500);
//			delayms(1);
//			startPWM(ENGINE_PWMPOINT1,lastSpeed);
//			startPWM(ENGINE_PWMPOINT2,0);
//		break;
//		default:
//			break;
//	}
//}
	