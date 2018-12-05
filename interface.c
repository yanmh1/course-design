#include"interface.h"
#include"lcd.h"
#include"ds1302.h"
#include"temp.h"

u8 timeOut = 0;
u8 stopTime = 0;
u8 saveStopTime[7] = {0};

u8 page_one_s1[] = "1-set countdown";
u8 page_one_s2[] = "2-set Score";
u8 page_one_s3[] = "3-score init";

u8 page_two_s1[] = "set Countdown";
u8 page_two_s2[] = "00:00:00";
u8 num = 0; //用于修改显示的时间
u8 setTime = 0;
u8 timeCoding[3] = {0}; //用于存放输入的初始化时间
u8 finishSetTime = 0;
u8 showtime[3] = {0};
u16 over = 0;
u16 sumSec = 0;
u16 sumTIME = 0;

u8 page_three_s1[] = "000:000";
u8 page_three_s2[] = "set score";

//用于页面三显示比分
u8 left_s = 48;
u8 left_ten = 48;
u8 right_s = 48;
u8 right_ten = 48;

struct SMH{
	u8 Single;
	u8 Ten;
}timecnt[3]={0};

void page_one(){
	u8 i = 0;
	LcdWriteCom(0x01);//清屏指令
	LcdShowStr(0,0,page_one_s1);	
	LcdShowStr(0,1,page_one_s2);
	LcdShowStr(0x12,0,page_one_s3);
	
	
	//只要还没选择其其它页面都在这个页面死循环
	while(desktop == '1'){
		//如果按下按键1
		if(KEY == '1'){
			
			desktop = '2';  //选择进入第二个页面
			KEY = '0';
		}
		if(KEY == '2'){
			
			LcdWriteCom(0x18);
			KEY = '0';
		}
		if(KEY == '3'){
			
			desktop = '3'; //选择进入第三个页面
			KEY = '0';
		}
		if(KEY == '4'){
			
			LcdWriteCom(0x1c);
			KEY = '0';
		}
		if(KEY == '5'){
			
			desktop = '4';
			KEY = '0';
		}
	}
	
}



void page_two(){
	u8 i = 0;
	u8 c = 0;
	u8 token = 0; //设置时间时减小数值
	int count = 0; //用于记录光标移动的位置
	
	LcdWriteCom(0x01);//清屏指令
	if(1 == stopTime){
		LcdShowStr(4,0,"Countdown");
		LcdDisplay_time();
		LcdShowStr(10,1,"stop!");
	}		

	
	if(0 == finishSetTime){
		LcdShowStr(0,0,page_two_s1);	
		LcdShowStr(0,1,page_two_s2);
	}
	
	
	//只要还没选择其其它页面都在这个页面死循环
	while(desktop == '2'){	
		if(1 == finishSetTime && 0 == timeOut && 0 == stopTime){
			//显示倒计时的算法及显示程序
			ctrl = 1; //对于这段时间作出特殊标记
			Ds1302ReadTime();
			sumTIME = 0;
			sumTIME += bcd_to_hex(TIME[0]);
			sumTIME += (bcd_to_hex(TIME[1]) * 60);
			sumTIME += (bcd_to_hex(TIME[2]) * 3600);
			over = sumSec - sumTIME;
			if(over == 0){
				timeOut = 1; //时间到达
			}
			showtime[2] = hex_to_bcd(over / 3600);
			showtime[1] = hex_to_bcd((over - (showtime[2] * 3600))/ 60);
			showtime[0] = hex_to_bcd(over % 60);
			LcdDisplay_time();
			ctrl = 0;			
		}
		else if(1 == timeOut)		LcdShowStr(0,1,"Time is over!");
		else if(1 == stopTime)	LcdShowStr(10,1,"stop!");
		
		//如果按下按键1
		if(KEY == '1'){
			if(1 == finishSetTime && 0 == stopTime){
				stopTime = 1;
				for(i = 0;i < 7;i ++){
					saveStopTime[i] = TIME[i];
				}
			}
			else if(1 == finishSetTime && 1 == stopTime){
				stopTime = 0;
				for(i = 0;i < 7;i ++){
					TIME[i] = saveStopTime[i];
				}
				Ds1302Init();
				LcdShowStr(10,1,"     ");//清除stop
			}
			else{
				timeCoding[1] += 1;
				timecnt[1].Single += 1;
				if(TimeCodingIn(1)){
					timecnt[1].Single = 0;
					timecnt[1].Ten += 1;
					LcdWriteCom(time_show_add + 3);
					LcdWriteData(timecnt[1].Ten + '0');
				}
				LcdWriteCom(time_show_add + 4);
				LcdWriteData(timecnt[1].Single + '0');
				token = 1;
			}				
			KEY = '0';
			finish = 0;
		}
		if(KEY == '2'){
			if(1 == finishSetTime){
				desktop = '3';
			}
			else{
				timeCoding[0] += 1;
				timecnt[0].Single += 1;
				if(TimeCodingIn(0)){
					timecnt[0].Single = 0;
					timecnt[0].Ten += 1;
					LcdWriteCom(time_show_add + 6);
					LcdWriteData(timecnt[0].Ten + '0');
				}
				LcdWriteCom(time_show_add + 7);
				LcdWriteData(timecnt[0].Single + '0');
			}
					
			token = 2;
			KEY = '0';
			finish = 0;
		}
		if(KEY == '3'){
			if(token == 1){
				timeCoding[1] -= 1;
				timecnt[1].Single -= 1;
				if(TimeCodingIn(1)){
					timecnt[1].Single = 0;
					timecnt[1].Ten -= 1;
					LcdWriteCom(time_show_add + 3);
					LcdWriteData(timecnt[1].Ten + '0');
				}
				LcdWriteCom(time_show_add + 4);
				LcdWriteData(timecnt[1].Single + '0');
			}
			else if(token == 2){
				timeCoding[0] -= 1;
				timecnt[0].Single -= 1;
				if(TimeCodingIn(0)){
					timecnt[0].Single = 0;
					timecnt[0].Ten -= 1;
					LcdWriteCom(time_show_add + 6);
					LcdWriteData(timecnt[0].Ten + '0');
				}
				LcdWriteCom(time_show_add + 7);
				LcdWriteData(timecnt[0].Single + '0');
			}
			else if(token == 4){
				timeCoding[2] -= 1;
				timecnt[2].Single -= 1;
				if(TimeCodingIn(2)){
					timecnt[2].Single = 0;
					timecnt[2].Ten -= 1;
					LcdWriteCom(time_show_add + 0);
					LcdWriteData(timecnt[2].Ten + '0');
				}
				LcdWriteCom(time_show_add + 1);
				LcdWriteData(timecnt[2].Single + '0');
			}
			
			LcdWriteCom(0xc8 + count);
			LcdWriteData(num + '0');
			finish = 0;
			KEY = '0';
		}
		if(KEY == '4'){
			if(0 == finishSetTime){
				timeCoding[2] += 1;
				timecnt[2].Single += 1;
				if(TimeCodingIn(2)){
					timecnt[2].Single = 0;
					timecnt[2].Ten += 1;
					LcdWriteCom(time_show_add + 0);
					LcdWriteData(timecnt[2].Ten + '0');
				}
				LcdWriteCom(time_show_add + 1);
				LcdWriteData(timecnt[2].Single + '0');
			}
			else{
				desktop = '1';
			}
			
			token = 4;
			KEY = '0';
			finish = 0;
		}		
		
		if(KEY == '5'){
			if(0 == finishSetTime){
				Ds1302Init();
				LcdWriteCom(0x01);//清屏指令
				finishSetTime = 1;
				//将时间全部化为秒
				sumSec += timeCoding[0];
				sumSec += timeCoding[1] * 60;
				sumSec += timeCoding[2] * 3600;				
			}
			finish = 0;
			KEY = '0';
				
		}
		finish = 0;
	}
		
}


void page_three(){
	u8 i = 0;
	u8 token = 0;
	LcdWriteCom(0x01);//清屏指令
	LcdShowStr(star_add,0,page_three_s1);	
	if(1 == stopTime){
		LcdWriteCom(0x80 + 0x08);
		LcdWriteData(right_ten);
		LcdWriteCom(0x80 + 0x09);
		LcdWriteData(right_s);
		LcdWriteCom(0x80 + 0x04);
		LcdWriteData(left_ten);
		LcdWriteCom(0x80 + 0x05);
		LcdWriteData(left_s);
		LcdDisplay_time();
	}

	LcdDisplay(Ds18b20ReadTemp());

	//只要还没选择其其它页面都在这个页面死循环
	while(desktop == '3'){
		if(1 == finishSetTime && 0 == timeOut && 0 == stopTime){
			//显示倒计时的算法及显示程序
			ctrl = 1; //对于这段时间作出特殊标记
			Ds1302ReadTime();
			sumTIME = 0;
			sumTIME += bcd_to_hex(TIME[0]);
			sumTIME += (bcd_to_hex(TIME[1]) * 60);
			sumTIME += (bcd_to_hex(TIME[2]) * 3600);
			over = sumSec - sumTIME;
			if(over == 0){
				timeOut = 1; //时间到达
			}
			showtime[2] = hex_to_bcd(over / 3600);
			showtime[1] = hex_to_bcd((over - (showtime[2] * 3600))/ 60);
			showtime[0] = hex_to_bcd(over % 60);
			LcdDisplay_time();
			ctrl = 0;			
		}
		else if(1 == timeOut)		LcdShowStr(0,1,"Time is over!");
		else if(1 == stopTime)	LcdShowStr(10,1,"stop!");
		
		
		//如果按下按键1
		if(KEY == '1'){
			if(1 == finishSetTime && 0 == stopTime){
				stopTime = 1;
				for(i = 0;i < 7;i ++){
					saveStopTime[i] = TIME[i];
				}
			}
			else if(1 == finishSetTime && 1 == stopTime){
				stopTime = 0;
				for(i = 0;i < 7;i ++){
					TIME[i] = saveStopTime[i];
				}
				Ds1302Init();
				LcdShowStr(10,1,"     ");//清除stop
			}
			else	desktop = '2';  //选择进入第二个页面
			KEY = '0';
			finish = 0;
		}
		if(KEY == '2'){
			token = 2;
			right_s += 1;
			if(right_s == 48 + 10){
				right_s = 48;
				right_ten += 1;
				LcdWriteCom(0x80 + star_add + 0x05);
				LcdWriteData(right_ten);
			}	
			LcdWriteCom(0x80 + star_add + 0x06);
			LcdWriteData(right_s);
			KEY = '0';
			finish = 0;
		}
		if(KEY == '3'){
			if(2 == token){
				right_s -= 1;
				if(right_s == '/'){
					right_s = '9';
					right_ten -= 1;
					LcdWriteCom(0x80 + star_add + 0x05);
					LcdWriteData(right_ten);
				}	
			  LcdWriteCom(0x80 + star_add + 0x06);
			  LcdWriteData(right_s);
			}
			else if(4 == token){
				left_s -= 1;
				if(left_s == '/'){
					left_s = '9';
					left_ten -= 1;
					LcdWriteCom(0x80 + star_add + 0x01);
					LcdWriteData(left_ten);
				}	
				LcdWriteCom(0x80 + star_add + 0x02);
				LcdWriteData(left_s);
			}
			KEY = '0';
			finish = 0;
		}	
			
		if(KEY == '4'){
			token = 4;
			left_s += 1;
			if(left_s == 48 + 10){
				left_s = 48;
				left_ten += 1;
				LcdWriteCom(0x80 + star_add + 0x01);
				LcdWriteData(left_ten);
			}	
			LcdWriteCom(0x80 + star_add + 0x02);
			LcdWriteData(left_s);
			KEY = '0';
			finish = 0;
		}
		if(KEY == '5'){
			desktop = '1';  //返回页面1
			page_three_s1[6] = right_s;
			page_three_s1[5] = right_ten;
			page_three_s1[2] = left_s;
			page_three_s1[1] = left_ten;
			KEY = '0';
			finish = 0;
		}
		finish = 0;
	}
	
}

void page_four(){

}




