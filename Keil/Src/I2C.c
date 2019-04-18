// �˲���ΪI2C���ߵ���������
#include <reg52.h>
#include <intrins.h>
#include "main.h"
#include "I2C.h"

// һ������ʱ����
void Delay(unsigned char DelayCount)
{
	while (DelayCount--);
}

// �����Ƕ�IIC���ߵĲ����ӳ���
// ��������
void Start(void)
{
    SCL = 0;    // SCL���ڸߵ�ƽʱ,SDA�Ӹߵ�ƽת��͵�ƽ��ʾ
    SDA = 1;    // һ��"��ʼ"״̬,��״̬��������������֮ǰִ��
    Delay(1);
    SCL = 1;
    Delay(1);
    SDA = 0;
    Delay(1);
    SCL = 0;
    SDA = 1;
    Delay(1);
}

// ֹͣIIC����
void Stop(void)
{
	SCL = 0;    // SCL���ڸߵ�ƽʱ,SDA�ӵ͵�ƽת��ߵ�ƽ
	SDA = 0;    // ��ʾһ��"ֹͣ"״̬,��״̬��ֹ����ͨѶ
	Delay(1);
	SCL = 1;
	Delay(1);   // �ղ���
	SDA = 1;
	Delay(1);
	SCL = 0;
	Delay(1000);
}

// ���Ӧ��λ
bit RecAck(void)
{
	SCL = 0;
	SDA = 1;
	Delay(20);
	SCL = 1;
	Delay(1);
	Delay(1);
	CY = SDA;   // ��Ϊ����ֵ���Ƿ���CY�е�
	SCL = 0;
	Delay(20);
	return(CY);
}

// ��IIC���߲���Ӧ��
void Ack(void)
{
	SDA = 0;    // EEPROMͨ�����յ�ÿ����ַ������֮��,
	SCL = 1;    // ��SDA�͵�ƽ�ķ�ʽȷ�ϱ�ʾ�յ���SDA��״̬
	Delay(1);
	SCL = 0;
	Delay(1);
	SDA = 1;
}

// ����IIC���߲���Ӧ��
void NoAck(void)
{
	SDA = 1;
	SCL = 1;
	Delay(1);
	SCL = 0;
}

// ��IIC����д����
void Send(unsigned char sendbyte)
{
	unsigned char data j = 8;
	for (; j>0; j--)
	{
		SCL = 0;
		sendbyte <<= 1; // ʹCY=sendbyte^7;
		SDA = CY;       // CY ��λ��־λ
		Delay(1);
		SCL = 1;
		Delay(20);
	}
	SCL = 0;
	Delay(1);
}

// ��IIC�����϶������ӳ���
unsigned char Receive(void)
{
	register receivebyte, i = 8;
	SCL = 0;
	while (i--)
	{
		SCL = 1;
		receivebyte = (receivebyte << 1) | SDA;
		Delay(1);
		SCL = 0;
		Delay(1);
	}
	return(receivebyte);
}