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
    // ��ʼ�� У׼ʱ��
	uchar flag;
//	flag = ReadSet1302(0x81);
    flag = 0x00;
	if (flag & 0x80)//�ж�ʱ��оƬ�Ƿ�ر�
	{
		WriteSet1302(0x8E, 0x00);	// д�벻����ָ��
		WriteSetYear(19);
		WriteSetMonth(04);
		WriteSetDay(18);
		WriteSetWeek(0);
		WriteSetHour(13);
		WriteSetMinute(28);
		WriteSetSecond(00);
		WriteSet1302(0x90, 0xa5);	// �򿪳�繦�� ѡ��2K�����緽ʽ
		WriteSet1302(0x8E, 0x80);	// ��д����
	}
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
void readCurrentTime(unsigned char * timeR)
{
    timeR[0] = '2';
    timeR[1] = '0';
    timeR[2] = ReadSet1302(0x8d)%100/10+'0';
    timeR[3] = ReadSet1302(0x8d)%100%10+'0';
    timeR[4] = '-';
    timeR[5] = ReadSet1302(0x89)%13/10+'0';
    timeR[6] = ReadSet1302(0x89)%13%10+'0';
    timeR[7] = '-';
    timeR[8] = ReadSet1302(0x87)%32/10+'0';
    timeR[9] = ReadSet1302(0x87)%32%10+'0';
    timeR[10] = ' ';
    timeR[11] = ReadSet1302(0x85)%24/10+'0';
    timeR[12] = ReadSet1302(0x85)%24%10+'0';
    timeR[13] = ':';
    timeR[14] = ReadSet1302(0x83)%60/10+'0';
    timeR[15] = ReadSet1302(0x83)%60%10+'0';
//    timeR[14] = ReadSet1302(0x81)%60/10+'0';
//    timeR[15] = ReadSet1302(0x81)%60%10+'0';
}