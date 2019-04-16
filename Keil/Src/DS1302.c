#include "ds1302.h"
#include "stdio.h"

// DS1302写入命令
void ds1302_write_cmd(uchar addr)
{
	uchar i;
	D_RST = 0;
	D_SCLK = 0;
	D_RST = 1;
	for (i = 0; i<8; i++)
	{
		D_IO = addr & 0x01;
		D_SCLK = 1;
		D_SCLK = 0;
		addr >>= 1;
	}
}

// DS1302指定地址读数据
uchar ReadSet1302(uchar addr)
{
	uchar i, dat;
	ds1302_write_cmd(addr);
	for (i = 0; i<8; i++)
	{
		dat >>= 1;
		if (D_IO == 1)
			dat |= 0x80;
		D_SCLK = 1;
		D_SCLK = 0;
	}
	dat = dat / 16 * 10 + dat % 16;
	return dat;
}

// DS1302指定地址写数据
void WriteSet1302(uchar addr, uchar dat)
{
	uchar i;
	dat = dat / 10 * 16 + dat % 10;
	ds1302_write_cmd(addr);
	for (i = 0; i<8; i++)
	{
		D_IO = dat & 0x01;
		D_SCLK = 1;
		D_SCLK = 0;
		dat >>= 1;
	}
}

// DS1302初始化时间
void ds1302_init_time()
{
    // 初始化 校准时间
	uchar flag;
	flag = ReadSet1302(0x81);
    flag = 0x00;
	if (flag & 0x80)//判断时钟芯片是否关闭
	{
		WriteSet1302(0x8E, 0x00);	// 写入不保护指令
		WriteSetYear(19);
		WriteSetMonth(04);
		WriteSetDay(16);
		WriteSetWeek(0);
		WriteSetHour(19);
		WriteSetMinute(40);
		WriteSetSecond(00);
		WriteSet1302(0x90, 0xa5);	// 打开充电功能 选择2K电阻充电方式
		WriteSet1302(0x8E, 0x80);	// 打开写保护
	}
}

// DS1302写入年
void WriteSetYear(uchar Year)
{
	WriteSet1302(0x8c, Year);
}

// DS1302写入月
void WriteSetMonth(uchar Month)
{
	WriteSet1302(0x88, Month);
}

// DS1302写入日
void WriteSetDay(uchar Day)
{
	WriteSet1302(0x86, Day);
}

// DS1302写入周
void WriteSetWeek(uchar Week)
{
	WriteSet1302(0x8a, Week);
}

// DS1302写入小时
void WriteSetHour(uchar Hour)
{
	WriteSet1302(0x84, Hour);
}

// DS1302写入分
void WriteSetMinute(uchar Minute)
{
	WriteSet1302(0x82, Minute);
}

// DS1302写入秒
void WriteSetSecond(uchar Second)
{
	WriteSet1302(0x80, Second);
}

// 读取当前时间
void readCurrentTime(unsigned char * timeR)
{
    timeR[0] = '2';
    timeR[1] = '0';
    timeR[2] = ReadSet1302(0x8d)%100/10+'0';
    timeR[3] = ReadSet1302(0x8d)%100%10+'0';
    timeR[4] = '-';
    timeR[5] = ReadSet1302(0x89)%12/10+'0';
    timeR[6] = ReadSet1302(0x89)%12%10+'0';
    timeR[7] = '-';
    timeR[8] = ReadSet1302(0x87)%31/10+'0';
    timeR[9] = ReadSet1302(0x87)%31%10+'0';
    timeR[10] = ' ';
    timeR[11] = ReadSet1302(0x85)%24/10+'0';
    timeR[12] = ReadSet1302(0x85)%24%10+'0';
    timeR[13] = ':';
    timeR[14] = ReadSet1302(0x83)%60/10+'0';
    timeR[15] = ReadSet1302(0x83)%60%10+'0';
//    timeR[14] = ReadSet1302(0x81)%60/10+'0';
//    timeR[15] = ReadSet1302(0x81)%60%10+'0';
}