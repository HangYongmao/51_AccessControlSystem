#include "Motor.h"
#include "main.h"

unsigned char phasecw[4] ={0x08,0x04,0x02,0x01};//正转 电机导通相序 D-C-B-A
unsigned char phaseccw[4]={0x01,0x02,0x04,0x08};//反转 电机导通相序 A-B-C-D

//顺时针转动
void MotorCW(void)
{
	uchar i;
	for (i = 0; i<4; i++)
	{
		MotorData = phasecw[i];
		delay_ms(4);//转速调节
	}
}
//逆时针转动
void MotorCCW(void)
{
	uchar i;
	for (i = 0; i<4; i++)
	{
		MotorData = phaseccw[i];
		delay_ms(4);//转速调节
	}
}
//停止转动
void MotorStop(void)
{
	MotorData = 0x00;
}