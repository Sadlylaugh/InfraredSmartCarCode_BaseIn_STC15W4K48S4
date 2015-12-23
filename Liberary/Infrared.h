#ifndef _INFRARED_H_
#define _INFRARED_H_
#include "OWN51.h"
#include "IO.h"
#include "ADC.h"
#include "COMMAND.h"
void scantest();

bit initInfrared();
void initInfraredCon();
void infraredScan(unsigned char times);
void showValue(unsigned char c);
void initInfraredCon();
#endif