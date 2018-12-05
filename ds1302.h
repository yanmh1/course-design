#ifndef __DS1302_H_
#define __DS1302_H_

//---包含头文件---//
#include<reg51.h>
#include<intrins.h>
#include"lcd.h"


//---定义ds1302使用的IO口---//
sbit DSIO=P3^4;
sbit RST=P3^5;
sbit SCLK=P3^6;

//---定义全局函数---//
void Ds1302Write(uchar addr, uchar dat);
uchar Ds1302Read(uchar addr);
void Ds1302Init();
void Ds1302ReadTime();
void LcdDisplay_time();
u8 bcd_to_hex(u8 n);
u8 hex_to_bcd(u8 n);

//---加入全局变量--//
extern uchar TIME[7];	//加入全局变量

#endif