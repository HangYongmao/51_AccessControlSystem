#include "Infrared.h"
#include "UART.h"
#include <reg52.h>

extern bit IntrusionFlag;   // 1-�Ƿ�����
extern unsigned char menu;  // 0-���˵�ģʽ 1-��ʾID

void InitInfrared()
{
    EA=1;          // ȫ���жϿ�
	EX0=1;         // �ⲿ�ж�0��
	IT0=1;         // �½��ش�����ʽ
}

void Infrared_INT() interrupt 0
{
    if (menu == 0)
        IntrusionFlag = 1;
}
