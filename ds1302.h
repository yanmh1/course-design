#ifndef __DS1302_H_
#define __DS1302_H_

//---����ͷ�ļ�---//
#include<reg51.h>
#include<intrins.h>
#include"lcd.h"


//---����ds1302ʹ�õ�IO��---//
sbit DSIO=P3^4;
sbit RST=P3^5;
sbit SCLK=P3^6;

//---����ȫ�ֺ���---//
void Ds1302Write(uchar addr, uchar dat);
uchar Ds1302Read(uchar addr);
void Ds1302Init();
void Ds1302ReadTime();
void LcdDisplay_time();
u8 bcd_to_hex(u8 n);
u8 hex_to_bcd(u8 n);

//---����ȫ�ֱ���--//
extern uchar TIME[7];	//����ȫ�ֱ���

#endif