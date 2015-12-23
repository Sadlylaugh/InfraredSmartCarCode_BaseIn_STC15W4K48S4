#ifndef _SMARTCAR_H_
#define _SAMRTCAR_H_
#include "STEERING.h"
#include "engine.h"
#include "PCA.h"
#include "IO.h"
#include "OWN51.h"
#include "RoadWatcher.h"
#include "COMMAND.h"
#include "math.h"
#define Pi 3.14f
#define EyesLong 17.0
void carControl();
void carSystemInit();
void carSteerSet(float position);
void carSpeedSet(unsigned int speed);
	
int speedPID(expcSpeed);
int directionPID(char directionNow);
unsigned int getCarSpeed();
void carTest();
#endif