#include "KEY.h"

extern bit KeyFlag;

// ��ʼ������ �ⲿ�ж�1
void InitKey()
{
    EA=1;          // ȫ���жϿ�
	EX1=1;         // �ⲿ�ж�0��
	IT1=1;         // �½��ش�����ʽ
}

void KEY_INT() interrupt 2
{
    KeyFlag = 1;
    while(!KEY_S3); // ���ּ��
}
