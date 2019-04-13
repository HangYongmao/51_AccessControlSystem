#include "Infrared.h"
#include "UART.h"
#include <reg52.h>
#include "main.h"

extern bit IntrusionFlag;   // 1-�Ƿ�����
extern enum MenuPage menuPage;  // 0-���˵�ģʽ 1-��ʾID

void InitInfrared()
{
    EA=1;          // ȫ���жϿ�
	EX0=1;         // �ⲿ�ж�0��
	IT0=1;         // �½��ش�����ʽ
}

void Infrared_INT() interrupt 0
{
    if (menuPage != ShowIDPage)
        IntrusionFlag = 1;
}
