#ifndef __UART_H__
#define __UART_H__

void Putc_to_SerialPort(unsigned char c);   // �򴮿ڷ����ַ�
void Puts_to_SerialPort(unsigned char *s);  // �򴮿ڷ����ַ���
void Putc_to_SerialPort_Hex(unsigned char c);// �򴮿ڷ���ʮ�������ַ�
void InitUART();    // ��ʼ������

#endif