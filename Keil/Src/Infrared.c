#include "Infrared.h"
#include "UART.h"
#include <reg52.h>
#include "main.h"

extern bit IntrusionFlag;   // 1-非法闯入
extern enum MenuPage menuPage;  // 0-主菜单模式 1-显示ID

void InitInfrared()
{
    EA=1;          // 全局中断开
	EX0=1;         // 外部中断0开
	IT0=1;         // 下降沿触发方式
}

void Infrared_INT() interrupt 0
{
    if (menuPage != ShowIDPage)
        IntrusionFlag = 1;
}
