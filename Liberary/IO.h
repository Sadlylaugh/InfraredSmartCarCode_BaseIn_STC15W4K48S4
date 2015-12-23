#ifndef _IO_H_
#define _IO_H_
//若无特别说明,工作频率一般为24MHz
//单片机定时器工作在1T模式
//串行口1使用定时器T2的16位自动重载模式作为波特率发生器
//串行口2使用定时器T2的16位自动重载模式作为波特率发生器
//串行口2切换到P3.7和P3.6使用
//串行口3使用定时器T3的16位自动重载模式作为波特率发生器
//工作在模式1（10位无校验）
#include "W4K51.h"
#include "OWN51.h"
#include <stdio.h>

#define MAXIOBUFU 20						//串口最大接收区缓冲区大小
#define S2RI 0x01
#define S2TI 0x02
#define S2REN 0x10
#define ES2 0x01

#define S3RI 1
#define S3TI 1<<1
#define S3REN 1<<4
#define ES3 1<<3
enum UartSerial
{
	serial1,
	serial2,
	serial3,
	serial4
};
//串口1
void initSerial(enum UartSerial serial);
void sendByte(unsigned char byte);
void sendStr(char *s);
void sendInt(unsigned long num);
void sendStrNNL(char *s);
void sendIntNNL(unsigned long num);
bit hadRecive();
char* getRecived();
//串口2
void sendByteU2(unsigned char byte);
void sendStrU2(char *s);
void sendIntU2(float num);
bit hadReciveU2();
char* getRecivedU2();
//串口3
void sendByteU3(unsigned char byte);
void sendStrU3(char *s);
void sendIntU3(float num);
bit hadReciveU3();
char* getRecivedU3();
#endif