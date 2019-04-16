#ifndef _DS1302_H_
#define _DS1302_H_

#include "main.h"
#include <intrins.h>

// 写命令输入参数:地址addr
void ds1302_write_cmd(uchar addr);

// 指定地址读数据-输入参数:地址addr-输出参数:时间dat 读字节
uchar ds1302_read_data(uchar addr);

// 指定地址写数据-输入参数:地址addr 时间dat
void WriteSet1302(uchar addr, uchar dat);

// 初始化时间
void ds1302_init_time();

// 获取时间-输出参数:时间dat
uchar ReadSet1302(uchar dat);

// 写入时间
void ds1302_write_time();

// 读取当前时间
void readCurrentTime(unsigned char * timeR);

void WriteSetYear(uchar Year);
void WriteSetMonth(uchar Month);
void WriteSetDay(uchar Day);
void WriteSetWeek(uchar Week);
void WriteSetHour(uchar Hour);
void WriteSetMinute(uchar Minute);
void WriteSetSecond(uchar Second);

#endif
