/*********
功能实现：
1>开机显示画面的内容：
	a.设置倒计时时长；
	b.设置比分初始值（默认为0:0）
	c.比分控制界面（控制比分的加减和刷新）
	d.
2>界面2：设置倒计时时长
	a.上键增加数值，下键减数值，左右键移动设置，中间键返回
3>界面3：比分初始化
	a.方案同上
4>界面4：比分控制
	a.左边比分控制：上键加，左键减，右边比分控制：右键加分，下键减分
	b.中间键返回
	c.长按上键开始倒计时，长按下键，停止倒计时
*********/
#include<reg51.h>
#include"lcd.h"
#include"interface.h"
#include"ds1302.h"
#include"usart.h"
#include"temp.h"



u8 desktop = '1';  //控制显示页面
u8 KEY = '0';
u8 ctrl = 0;
u8 finish = 0;

void main(void)
{
	UsartInit(); 
	LcdInit();   //initialize LCD
	while(1){
			if('1' == desktop){
				page_one();
			}
			else if('2' == desktop){
				page_two();
			}
			else if('3' == desktop){
				page_three();
			}
			else{
				page_four();
			}
	}
}


void Usart() interrupt 4
{
	u8 receiveData;

	receiveData=SBUF;
	if(receiveData > 47 && receiveData < 58){
		//对于页面2的特殊处理
	    if( '2'== desktop){
				//下面的if else语句不能调乱;
				if(1 == finish){ //丢弃该次的值
					
				}
				else 
					KEY = receiveData;
				
				if(ctrl == 1){
	    		finish = 1;  //特殊标记
	    	}
				
	    }
	    else 
	    	KEY = receiveData;	
	        
	}
	RI = 0;
//	while(1)
//		P2 = 0xfe;

}







