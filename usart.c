#include"usart.h"
#include<reg51.h>

void UsartInit()
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X00;			//�����ʼӱ�
	TH1=0XFD;				//��������ʼֵ���ã�ע�Ⲩ������4800��
	TL1=0XFD;
	PS = 1;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
	P2=0xff;
}




