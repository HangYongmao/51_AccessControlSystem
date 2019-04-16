#ifndef __I2C_H__
#define __I2C_H__

// 起动总线函数
void Start_I2c();

// 结束总线函数  
void Stop_I2c();

// 应答子函数
void Ack_I2c(bit a);

// 字节数据发送函数
void SendByte(unsigned char  c);

// 无子地址读字节数据函数               
unsigned char RcvByte();

#endif
