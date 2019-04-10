#include "ds1302.h"
#include "stdio.h"

// DS1302д������
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

// DS1302ָ����ַ������
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

// DS1302ָ����ַд����
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

// DS1302��ʼ��ʱ��
void ds1302_init_time()
{
//	uchar flag;
//	flag = ReadSet1302(0x81);
//	if (flag & 0x80)//�ж�ʱ��оƬ�Ƿ�ر�
//	{
		WriteSet1302(0x8E, 0x00);	// д�벻����ָ��
		WriteSetYear(99);
		WriteSetMonth(01);
		WriteSetDay(01);
		WriteSetWeek(0);
		WriteSetHour(00);
		WriteSetMinute(00);
		WriteSetSecond(00);
		WriteSet1302(0x90, 0xa5);	// �򿪳�繦�� ѡ��2K�����緽ʽ
		WriteSet1302(0x8E, 0x80);	// ��д����
//	}
}

// DS1302д����
void WriteSetYear(uchar Year)
{
	WriteSet1302(0x8c, Year);
}

// DS1302д����
void WriteSetMonth(uchar Month)
{
	WriteSet1302(0x88, Month);
}

// DS1302д����
void WriteSetDay(uchar Day)
{
	WriteSet1302(0x86, Day);
}

// DS1302д����
void WriteSetWeek(uchar Week)
{
	WriteSet1302(0x8a, Week);
}

// DS1302д��Сʱ
void WriteSetHour(uchar Hour)
{
	WriteSet1302(0x84, Hour);
}

// DS1302д���
void WriteSetMinute(uchar Minute)
{
	WriteSet1302(0x82, Minute);
}

// DS1302д����
void WriteSetSecond(uchar Second)
{
	WriteSet1302(0x80, Second);
}

// ��ȡ��ǰʱ��
void readCurrentTime(char * time)
{
    // sprintf(time, "20%2d-%2d-%2d %2d:%2d", ReadSet1302(0x8d)%100, ReadSet1302(0x89)%12, ReadSet1302(0x87)%31, ReadSet1302(0x85)%24, ReadSet1302(0x83)%60);
    time[0] = '2';
    time[1] = '0';
    time[2] = ReadSet1302(0x8d)%100/10+'0';
    time[3] = ReadSet1302(0x8d)%100%10+'0';
    time[4] = '-';
    time[5] = ReadSet1302(0x89)%12/10+'0';
    time[6] = ReadSet1302(0x89)%12%10+'0';
    time[7] = '-';
    time[8] = ReadSet1302(0x87)%31/10+'0';
    time[9] = ReadSet1302(0x87)%31%10+'0';
    time[10] = ' ';
    time[11] = ReadSet1302(0x85)%24/10+'0';
    time[12] = ReadSet1302(0x85)%24%10+'0';
    time[13] = ':';
    time[14] = ReadSet1302(0x83)%60/10+'0';
    time[15] = ReadSet1302(0x83)%60%10+'0';
}