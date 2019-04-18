#ifndef __AT24C256_H__
#define __AT24C256_H__

#include "main.h"

// �������ͺ�
enum EEPROMType{AT2401, AT2402, AT2404, AT2408,
                AT2416, AT2432, AT2464, AT24128, AT24256};

// AT24C01��AT24C256 �Ķ�д����
bit RW24xx(unsigned char *DataBuff, unsigned char Length, unsigned int Addr,
	unsigned char Control, enum EEPROMType enumer);

#endif