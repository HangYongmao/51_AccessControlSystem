#include <reg52.h>
#include "UART.h"
#include <stdio.h>
#include "main.h"

// 向串口发送字符
void Putc_to_SerialPort(unsigned char c)
{
	SBUF = c;
	while (TI == 0);
	TI = 0;
}

// 向串口发送字符串
void Puts_to_SerialPort(unsigned char *s)
{
	while (*s != '\0')
	{
		Putc_to_SerialPort(*s);
		s++;
		delay_ms(5);
	}
}

// 向串口发送十六进制字符
void Putc_to_SerialPort_Hex(unsigned char c)
{
    // 高位数据
    if ((c>>4) < 10)
        Putc_to_SerialPort((c>>4) + '0');
    else
        Putc_to_SerialPort((c>>4) - 10 + 'A');
    
    // 低位数据
    if ((c&0xf) < 10)
        Putc_to_SerialPort((c&0xf) + '0');
    else
        Putc_to_SerialPort((c&0xf) - 10 + 'A');
}

// 初始化串口
void InitUART()
{
	SCON = 0x40;		// 串口模式1 
	TMOD = 0x20;		// T1工作模式2
	TH1 = 0xfd;		    // 波特率9600
	TL1 = 0xfd;
	PCON = 0x00;		// 波特率不倍增
	TI = 0;
	TR1 = 1;
	delay_ms(200);
}