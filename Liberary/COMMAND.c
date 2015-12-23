#include "COMMAND.h"

unsigned char direction=0;

unsigned char getDirection()
{
	char *str;
	while(!hadRecive());
	str=getRecived();
	if(str[0]=='Y'||str[0]=='y')
		return YES;
	else if(str[0]=='N'||str[0]=='n')
		return NO;
	else if(strcmp(str,"set")==0)
		return set;
	else if(strcmp(str,"test")==0)
		return test;
	else if(strcmp(str,"stop")==0)
		return stop;
	else
		return 0;
}

unsigned long getInt()
{
	char *str;
	while(!hadRecive());
	str=getRecived();
	return atoi(str);
}

void waitForNext()
{
	sendStr("Wait for Y to NEXT.");
	while(getDirection()!=YES);
}

void interFaceWord()
{
		sendStr("Welcome to Fujiwara bean curd SmartCar InterFace Program!");
		sendStr("Send \"Y#\" to initialize All System and ready to RACE");
		sendStr("Send \"set#\" to Set the System.");
		sendStr("Send \"test#\" to test All System model./");
		sendStr(" ");
		sendStr("Wait for your Command:");
}

void interFace()
{
	initSerial(serial1);
	interFaceWord();
	while(1)
	{
		direction=getDirection();
		switch(direction)
		{
			case 1:
				initPCA0(0);
				initEngine();
				steeringSet(90);
				sendStr("Put the car in the StartLine.");
		}
	}
}				