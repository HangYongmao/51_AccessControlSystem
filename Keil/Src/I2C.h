#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"

#define  ERROR 10   // 允许ERROR的最大次数

// 一个简单延时程序
void Delay(unsigned char DelayCount);

// 以下是对IIC总线的操作子程序
// 启动总线
void Start(void);

// 停止IIC总线
void Stop(void);

// 检查应答位
bit RecAck(void);

// 对IIC总线产生应答
void Ack(void);

// 不对IIC总线产生应答
void NoAck(void);

// 向IIC总线写数据
void Send(unsigned char sendbyte);

// 从IIC总线上读数据子程序
unsigned char Receive(void);

#endif
