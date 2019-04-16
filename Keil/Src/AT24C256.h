#ifndef __AT24C256_H__
#define __AT24C256_H__

//有子地址读取多字节数据函数               
extern bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

//有子地址发送多字节数据函数               
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

#endif