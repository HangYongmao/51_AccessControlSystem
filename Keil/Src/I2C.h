#ifndef __I2C_H__
#define __I2C_H__

// �����ߺ���
void Start_I2c();

// �������ߺ���  
void Stop_I2c();

// Ӧ���Ӻ���
void Ack_I2c(bit a);

// �ֽ����ݷ��ͺ���
void SendByte(unsigned char  c);

// ���ӵ�ַ���ֽ����ݺ���               
unsigned char RcvByte();

#endif
