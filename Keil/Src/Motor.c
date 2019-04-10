#include "Motor.h"
#include "main.h"

unsigned char phasecw[4] ={0x08,0x04,0x02,0x01};//��ת �����ͨ���� D-C-B-A
unsigned char phaseccw[4]={0x01,0x02,0x04,0x08};//��ת �����ͨ���� A-B-C-D

//˳ʱ��ת��
void MotorCW(void)
{
	uchar i;
	for (i = 0; i<4; i++)
	{
		MotorData = phasecw[i];
		delay_ms(4);//ת�ٵ���
	}
}
//��ʱ��ת��
void MotorCCW(void)
{
	uchar i;
	for (i = 0; i<4; i++)
	{
		MotorData = phaseccw[i];
		delay_ms(4);//ת�ٵ���
	}
}
//ֹͣת��
void MotorStop(void)
{
	MotorData = 0x00;
}