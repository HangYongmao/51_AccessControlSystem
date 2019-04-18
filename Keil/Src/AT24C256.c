#include "main.h"
#include "I2C.h"
#include "AT24C256.h"

// AT24C01～AT24C256 的读写程序
// DataBuff-读写数据缓冲区指针
//   Length-进行读写的字节数
//     Addr-EEPROM首址 AT24256为0～32767
//  Control-EEPROM控制字节
//   enumer-EEPROM类型
// 返回1表示此次操作失效, 0表示操作成功;
// 1K位，2K位，4K位，8K位，16K位芯片采用一个8位长的字节地址码，
// 对于32K位以上的采用2个8位长的字节地址码直接寻址，
// 而4K位，8K位，16K位配合页面地址来寻址。
bit RW24xx(unsigned char *DataBuff, unsigned char Length, unsigned int Addr,
	unsigned char Control, enum EEPROMType enumer)
{

	unsigned char data j, i = ERROR;
	bit errorflag = 1;  // 出错标志
	while (i--)
	{
		Start();                // 启动总线
		Send(Control & 0xfe);   // 向IIC总线写数据，器件地址
		if (RecAck()) continue; // 如写不正确结束本次循环
        
        // 如果容量大于32K位，使用16位地址寻址，写入高八位地址
		if (enumer > AT2416)
		{
			Send((unsigned char)(Addr >> 8));// 把整型数据转换为字符型数据：弃高取低，只取低8位.
			if (RecAck())
                continue;
		}
		Send((unsigned char)Addr);  // 向IIC总线写数据
		if (RecAck())
            continue;    // 如写正确结束本次循环
		if (!(Control & 0x01))      // 判断是读器件还是写器件
		{
			j = Length;
			errorflag = 0;          // 清错误特征位
			while (j--)
			{
				Send(*DataBuff++);  // 向IIC总线写数据
				if (!RecAck())
                    continue;       // 如写正确结束本次循环
				errorflag = 1;
				break;
			}
			if (errorflag == 1)
                continue;
			break;
		}
		else
		{
			Start();        // 启动总线
			Send(Control);  // 向IIC总线写数据
			if (RecAck())
                continue;   // 器件没应答结束本次本层循环
			while (--Length)// 字节长为0结束
			{
				*DataBuff++ = Receive();
				Ack();      // 对IIC总线产生应答
			}
			*DataBuff = Receive(); // 读最后一个字节
			NoAck();        // 不对IIC总线产生应答
			errorflag = 0;
			break;
		}
	}
	Stop();  // 停止IIC总线
	if (!(Control & 0x01))
	{
		Delay(255);
		Delay(255);
		Delay(255);
	}
	return(errorflag);
}