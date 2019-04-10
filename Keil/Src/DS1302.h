#ifndef _DS1302_H_
#define _DS1302_H_

#include "main.h"
#include <intrins.h>

// д�����������:��ַaddr
void ds1302_write_cmd(uchar addr);

// ָ����ַ������-�������:��ַaddr-�������:ʱ��dat ���ֽ�
uchar ds1302_read_data(uchar addr);

// ָ����ַд����-�������:��ַaddr ʱ��dat
void WriteSet1302(uchar addr, uchar dat);

// ��ʼ��ʱ��
void ds1302_init_time();

// ��ȡʱ��-�������:ʱ��dat
uchar ReadSet1302(uchar dat);

// д��ʱ��
void ds1302_write_time();

// ��ȡ��ǰʱ��
void readCurrentTime(char * time);

void WriteSetYear(uchar Year);
void WriteSetMonth(uchar Month);
void WriteSetDay(uchar Day);
void WriteSetWeek(uchar Week);
void WriteSetHour(uchar Hour);
void WriteSetMinute(uchar Minute);
void WriteSetSecond(uchar Second);

#endif
