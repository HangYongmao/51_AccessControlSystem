#include <reg52.h>
#include <intrins.h>
#include "UART.h"
#include "main.h"

unsigned char clock=0;
extern enum MenuPage menuPage;
extern bit clsFlag;

void InitTimer0()
{
    TMOD &= 0xf0;
    TH0 = (8192-4607)/32;
	TL0 = (8192-4607)%32;
	EA = 1;
	ET0 = 1;    // 定时器0 中断打开
	TR0 = 1;    // 定时器0 打开
}

void T0_time() interrupt 1
{
    static unsigned char num;
	TH0 = (8192-4607)/32;
	TL0 = (8192-4607)%32;
	num++;
    if(num%200 == 0)
    {
        num = 0;
        clock ++;
    }
    if ((clock == 5) && (menuPage != HomePage))
    {
        clock = 0;
        clsFlag = 1;
    }
}
