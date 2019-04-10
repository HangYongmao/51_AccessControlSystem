#include <reg52.h>
#include <intrins.h>
#include "UART.h"

unsigned char clock=0;
extern unsigned char menu;
extern bit clsFlag;

void InitTimer0()
{
    TMOD &= 0xf0;
    TH0 = (8192-4607)/32;
	TL0 = (8192-4607)%32;
	EA = 1;
	ET0 = 1;
	TR0 = 1;
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
    if ((clock == 5) && (menu != 0))
    {
        clock = 0;
        clsFlag = 1;
    }
}
