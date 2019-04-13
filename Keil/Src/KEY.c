#include "KEY.h"

extern bit KeyFlag;

// 初始化按键 外部中断1
void InitKey()
{
    EA=1;          // 全局中断开
	EX1=1;         // 外部中断0开
	IT1=1;         // 下降沿触发方式
}

void KEY_INT() interrupt 2
{
    KeyFlag = 1;
    while(!KEY_S3); // 松手检测
}
