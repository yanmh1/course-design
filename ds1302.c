#include"ds1302.h"
#include"interface.h"

//---DS1302写入和读取时分秒的地址命令---//
//---秒分时日月周年 最低位读写位;-------//
uchar code READ_RTC_ADDR[7] = {0x81, 0x83, 0x85, 0x87, 0x89, 0x8b, 0x8d}; 
uchar code WRITE_RTC_ADDR[7] = {0x80, 0x82, 0x84, 0x86, 0x88, 0x8a, 0x8c};

//---DS1302时钟初始化2013年1月1日星期二12点00分00秒。---//
//---存储顺序是秒分时日月周年,存储格式是用BCD码---//
uchar TIME[7] = {0,0,0,0x27,0x11,0x02,0x18};


u8 bcd_to_hex(u8 n)
{
    u8 temp;

    temp = ((n>>4)*10 + (n&0x0f));
    return temp;
}

u8 hex_to_bcd(u8 n)
{
    u8 temp;

    temp = (((n/10)<<4) + (n%10));
    return temp;
}



/*******************************************************************************
* 函 数 名         : Ds1302Write
* 函数功能		   : 向DS1302命令（地址+数据）
* 输    入         : addr,dat
* 输    出         : 无
*******************************************************************************/

void Ds1302Write(uchar addr, uchar dat)
{
	uchar n;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1; //然后将RST(CE)置高电平。
	_nop_();

	for (n=0; n<8; n++)//开始传送八位地址命令
	{
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();
	}
	for (n=0; n<8; n++)//写入8位数据
	{
		DSIO = dat & 0x01;
		dat >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;
		_nop_();	
	}	
		 
	RST = 0;//传送数据结束
	_nop_();
}

/*******************************************************************************
* 函 数 名         : Ds1302Read
* 函数功能		   : 读取一个地址的数据
* 输    入         : addr
* 输    出         : dat
*******************************************************************************/

uchar Ds1302Read(uchar addr)
{
	uchar n,dat,dat1;
	RST = 0;
	_nop_();

	SCLK = 0;//先将SCLK置低电平。
	_nop_();
	RST = 1;//然后将RST(CE)置高电平。
	_nop_();

	for(n=0; n<8; n++)//开始传送八位地址命令
	{
		DSIO = addr & 0x01;//数据从低位开始传送
		addr >>= 1;
		SCLK = 1;//数据在上升沿时，DS1302读取数据
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}
	_nop_();
	for(n=0; n<8; n++)//读取8位数据
	{
		dat1 = DSIO;//从最低位开始接收
		dat = (dat>>1) | (dat1<<7);
		SCLK = 1;
		_nop_();
		SCLK = 0;//DS1302下降沿时，放置数据
		_nop_();
	}

	RST = 0;
	_nop_();	//以下为DS1302复位的稳定时间,必须的。
	SCLK = 1;
	_nop_();
	DSIO = 0;
	_nop_();
	DSIO = 1;
	_nop_();
	return dat;	
}

/*******************************************************************************
* 函 数 名         : Ds1302Init
* 函数功能		   : 初始化DS1302.
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302Init()
{
	uchar n;
	Ds1302Write(0x8E,0X00);		 //禁止写保护，就是关闭写保护功能
	for (n=0; n<7; n++)//写入7个字节的时钟信号：分秒时日月周年
	{
		Ds1302Write(WRITE_RTC_ADDR[n],TIME[n]);	
	}
	Ds1302Write(0x8E,0x80);		 //打开写保护功能
}

/*******************************************************************************
* 函 数 名         : Ds1302ReadTime
* 函数功能		   : 读取时钟信息
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void Ds1302ReadTime()
{
	uchar n;
	for (n=0; n<7; n++)//读取7个字节的时钟信号：分秒时日月周年
	{
		TIME[n] = Ds1302Read(READ_RTC_ADDR[n]);
	}
		
}


void LcdDisplay_time()
{
//	LcdWriteCom(0x80);
//	LcdWriteData('2');
//	LcdWriteData('0');
//	LcdWriteData('0'+TIME[6]/16);			//年
//	LcdWriteData('0'+(TIME[6]&0x0f));
//	LcdWriteData('-');
//	LcdWriteData('0'+TIME[4]/16);			//月
//	LcdWriteData('0'+(TIME[4]&0x0f));
//	LcdWriteData('-');
//	LcdWriteData('0'+TIME[3]/16);			//日
//	LcdWriteData('0'+(TIME[3]&0x0f));
//	LcdWriteCom(0x8D);
//	LcdWriteData('0'+(TIME[5]&0x07));	//星期			 
	LcdWriteCom(0x80+0X40);
	LcdWriteData('0'+showtime[2]/16);				//时
	LcdWriteData('0'+(showtime[2]&0x0f));				 
	LcdWriteData(':');
	LcdWriteData('0'+showtime[1]/16);				//分
	LcdWriteData('0'+(showtime[1]&0x0f));	
	LcdWriteData(':');
	LcdWriteData('0'+showtime[0]/16);				//秒
	LcdWriteData('0'+(showtime[0]&0x0f));

}


