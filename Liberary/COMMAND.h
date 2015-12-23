#ifndef _COMMAND_H_
#define _COMMAND_H_

#include "OWN51.h"
#include "IO.h"
#include "PWM.h"
#include "PCA.h"
#include "engine.h"
#include "STEERING.h"
#include "string.h"

enum command
{
	donothing=0,
	YES=1,
	NO=2,
	set=3,
	test=4,
	stop=5,
	start=6
};
	

unsigned char getDirection();
unsigned long getInt();
void waitForNext();
#endif