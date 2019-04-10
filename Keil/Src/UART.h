#ifndef __UART_H__
#define __UART_H__

void Putc_to_SerialPort(unsigned char c);   // 向串口发送字符
void Puts_to_SerialPort(unsigned char *s);  // 向串口发送字符串
void Putc_to_SerialPort_Hex(unsigned char c);// 向串口发送十六进制字符
void InitUART();    // 初始化串口

#endif