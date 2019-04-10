/*
*  RC522ʵ�鿪��������
*  CPU:STC89C52RC
*  ����:11.0592MHZ
------------------------------------------------*/
#include <reg52.h>
#include <stdio.h>
#include <string.h>
#include "main.h"
#include "RC522.h"
#include "UART.h"
#include "TIMER.h"
#include "Mini12864.h"
#include "Motor.h"
#include "DS1302.h"
#include "Infrared.h"

unsigned char g_ucTempbuf[20];
bit clsFlag=0;  // 1-��Ҫ����
unsigned char menu=0;   // 0-���˵�ģʽ 1-��ʾID
extern unsigned char clock;
bit IntrusionFlag = 0;  // 1-�Ƿ�����

void delay_ms(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x>0; x--)
	for (y = 110; y>0; y--);
}

int main()
{
    uint k;
	unsigned char status, i;
	unsigned char cardType[3]={0};
    unsigned char cardCode[5]={0};
    
    // ��һ�εĿ���Ϣ
	unsigned char lastCardType[3]={0};
    unsigned char lastCardCode[5]={0};
    
	InitUART();
    InitTimer0();
    ds1302_init_time();
    InitInfrared();
    
    MotorStop();
    
    InitLcd();
    
    // ����ָ������
    LcdCls(1, 1, 128, 64);
    
	PcdReset();
	PcdAntennaOff();
    delay_ms(100);
	PcdAntennaOn();
    
    LCDDispalyMain();
    
	while (1)
	{
		status = PcdRequest(PICC_REQALL, g_ucTempbuf);//Ѱ��
		if (status != MI_OK)
		{
			PcdReset();
			PcdAntennaOff();
			PcdAntennaOn();
		}
        else
        {
            for (i = 0; i<2; i++)
            {
                cardType[i] = g_ucTempbuf[i];
            }

            status = PcdAnticoll(g_ucTempbuf);// ����ײ
            if (status != MI_OK)
            {
                continue;
            }

            for (i = 0; i<4; i++)
            {
                cardCode[i] = g_ucTempbuf[i];
                
                menu = 1;
                clock = 0;
                clsFlag = 0;
                IntrusionFlag = 0;
            }
            
            if ((strcmp(lastCardType, cardType) != 0) || (strcmp(lastCardCode, cardCode) != 0))
            {
                strcpy(lastCardType, cardType);
                strcpy(lastCardCode, cardCode);
                
                for(k=0; k<5; k++)
                {
                    MotorCW();   //˳ʱ��ת��
                }
                
            // �������
                // ��Ƭ����
                Puts_to_SerialPort("Card Type: ");
                Putc_to_SerialPort_Hex(cardType[0]);
                Putc_to_SerialPort_Hex(cardType[1]);
                Puts_to_SerialPort("\r\n");
                
                // ��Ƭ���к�
                Puts_to_SerialPort("Card Code: ");
                Putc_to_SerialPort_Hex(cardCode[0]);
                Putc_to_SerialPort_Hex(cardCode[1]);
                Putc_to_SerialPort_Hex(cardCode[2]);
                Putc_to_SerialPort_Hex(cardCode[3]);
                Puts_to_SerialPort("\r\n");
                
                Putc_to_SerialPort_Hex(clsFlag);
                Putc_to_SerialPort_Hex(menu);
                Putc_to_SerialPort_Hex(clock);
            // OLED��ʾ
                // ����ָ������
                LcdCls(1, 1, 128, 64);
                Disp_String_8x16(1, 1, "Card Type: ");
                Disp_String_Hex(3, 1, cardType[0]);
                Disp_String_Hex(3, 17, cardType[1]);
                
                Disp_String_8x16(5, 1, "Card Code: ");
                Disp_String_Hex(7, 1, cardCode[0]);
                Disp_String_Hex(7, 17, cardCode[1]);
                Disp_String_Hex(7, 33, cardCode[2]);
                Disp_String_Hex(7, 49, cardCode[3]);
            }
        }
        
        if (clsFlag == 1)
        {
            Puts_to_SerialPort("CLS");
            memset(lastCardType, 0, 3);
            memset(lastCardType, 0, 3);
            LCDDispalyMain();
            
            MotorCCW();
            
            clsFlag = 0;
            clock = 0;
            menu = 0;
        }
        
        // �Ƿ�����
        if (IntrusionFlag == 1)
        {
            Puts_to_SerialPort("Error....IntrusionFlag");
            IntrusionFlag = 0;
        }
	}
}
