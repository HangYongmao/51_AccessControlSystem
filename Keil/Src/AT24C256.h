#ifndef __AT24C256_H__
#define __AT24C256_H__

//���ӵ�ַ��ȡ���ֽ����ݺ���               
extern bit IRcvStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

//���ӵ�ַ���Ͷ��ֽ����ݺ���               
extern bit ISendStr(unsigned char sla,unsigned char suba,unsigned char *s,unsigned char no);

#endif