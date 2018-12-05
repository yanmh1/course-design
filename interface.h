#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include<reg51.h>

extern u8 desktop;
extern u8 KEY;
extern u8 ctrl;
extern u8 finish;
extern u8 showtime[3];
#define star_add 0x00
#define time_show_add (0x80+0x40+0)
#define TimeCodingIn(x)     (timeCoding[x] == 10) || (timeCoding[x] == 20) || (timeCoding[x] == 30) \
													|| (timeCoding[x] == 40) || (timeCoding[x] == 50) || (timeCoding[x] == 60)





void page_one();
void page_two();
void page_three();
void page_four();

#endif

