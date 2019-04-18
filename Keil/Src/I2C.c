// 此部分为I2C总线的驱动程序
#include <reg52.h>
#include <intrins.h>
#include "main.h"
#include "I2C.h"

// 一个简单延时程序
void Delay(unsigned char DelayCount)
{
	while (DelayCount--);
}

// 以下是对IIC总线的操作子程序
// 启动总线
void Start(void)
{
    SCL = 0;    // SCL处于高电平时,SDA从高电平转向低电平表示
    SDA = 1;    // 一个"开始"状态,该状态必须在其他命令之前执行
    Delay(1);
    SCL = 1;
    Delay(1);
    SDA = 0;
    Delay(1);
    SCL = 0;
    SDA = 1;
    Delay(1);
}

// 停止IIC总线
void Stop(void)
{
	SCL = 0;    // SCL处于高电平时,SDA从低电平转向高电平
	SDA = 0;    // 表示一个"停止"状态,该状态终止所有通讯
	Delay(1);
	SCL = 1;
	Delay(1);   // 空操作
	SDA = 1;
	Delay(1);
	SCL = 0;
	Delay(1000);
}

// 检查应答位
bit RecAck(void)
{
	SCL = 0;
	SDA = 1;
	Delay(20);
	SCL = 1;
	Delay(1);
	Delay(1);
	CY = SDA;   // 因为返回值总是放在CY中的
	SCL = 0;
	Delay(20);
	return(CY);
}

// 对IIC总线产生应答
void Ack(void)
{
	SDA = 0;    // EEPROM通过在收到每个地址或数据之后,
	SCL = 1;    // 置SDA低电平的方式确认表示收到读SDA口状态
	Delay(1);
	SCL = 0;
	Delay(1);
	SDA = 1;
}

// 不对IIC总线产生应答
void NoAck(void)
{
	SDA = 1;
	SCL = 1;
	Delay(1);
	SCL = 0;
}

// 向IIC总线写数据
void Send(unsigned char sendbyte)
{
	unsigned char data j = 8;
	for (; j>0; j--)
	{
		SCL = 0;
		sendbyte <<= 1; // 使CY=sendbyte^7;
		SDA = CY;       // CY 进位标志位
		Delay(1);
		SCL = 1;
		Delay(20);
	}
	SCL = 0;
	Delay(1);
}

// 从IIC总线上读数据子程序
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