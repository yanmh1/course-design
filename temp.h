#ifndef __TEMP_H_
#define __TEMP_H_

#include<reg51.h>

sbit DSPORT=P3^7;

extern u8 CNCHAR[6];


void Delay1ms(unsigned int );
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
int Ds18b20ReadTemp();
void LcdDisplay(int temp);

#endif
