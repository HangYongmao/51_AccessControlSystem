#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

#define  ERROR 10   // ����ERROR��������

// һ������ʱ����
void Delay(unsigned char DelayCount);

// �����Ƕ�IIC���ߵĲ����ӳ���
// ��������
void Start(void);

// ֹͣIIC����
void Stop(void);

// ���Ӧ��λ
bit RecAck(void);

// ��IIC���߲���Ӧ��
void Ack(void);

// ����IIC���߲���Ӧ��
void NoAck(void);

// ��IIC����д����
void Send(unsigned char sendbyte);

// ��IIC�����϶������ӳ���
unsigned char Receive(void);

#endif
