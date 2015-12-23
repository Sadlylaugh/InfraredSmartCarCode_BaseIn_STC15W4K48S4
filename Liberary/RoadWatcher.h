#ifndef _ROADWATCHER_H_
#define _ROADWATCHER_H_


#include "OWN51.h"
#include "IO.h"
sbit IRRight6=	P0^0;
sbit IRRight5=	P0^1;
sbit IRRight4=	P0^2;
sbit IRRight3=	P0^3;
sbit IRRight2=	P0^4;
sbit IRRight1=	P0^5;

sbit IR0_Now=	P0^6;//中间

sbit IRLeft1=	P0^7;
sbit IRLeft2=	P1^0;
sbit IRLeft3=	P1^1;
sbit IRLeft4=	P1^2;
sbit IRLeft5=	P1^3;
sbit IRLeft6=	P1^4;

void watcherScan();
unsigned char getcarDirection();

#endif
