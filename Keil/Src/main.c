/*
*  ����ISO/IEC14443 TypeA���ǻ۳����Ž�����ϵͳ�����ʵ��
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
#include "KEY.h"
#include "AT24C256.h"
#include "I2C.h"

unsigned char g_ucTempbuf[20];
bit clsFlag=0;  // 1-��Ҫ����

enum MenuPage menuPage=HomePage;
extern unsigned char clock;
unsigned char time[16];
bit IntrusionFlag = 0;  // 1-�Ƿ�����

bit KeyFlag=0;    // 1-¼���¿�

UserInfo user[20];
unsigned char userCount=0;
UserInfo adminUser={{0xAC, 0xEC, 0x45, 0xD2, 0x00}, 99, 12, 30}; // ����Ա

enum EEPROMType enumer=AT24256; // ����һ��ö�ٱ��� ��дAT24C256
unsigned char AT24C02Buff[12];      // ���ݻ�����

void delay_ms(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x>0; x--)
	for (y = 110; y>0; y--);
}

void InitUserInfo()
{
    int k=0;
    // ����Ա����
    user[0].cardCode[0] = adminUser.cardCode[0];
    user[0].cardCode[1] = adminUser.cardCode[1];
    user[0].cardCode[2] = adminUser.cardCode[2];
    user[0].cardCode[3] = adminUser.cardCode[3];
    user[0].cardCode[4] = 0x00;
    userCount++;
    
    // ��ȡAT24CXX�е�����
// ��ȡuserCount
    RW24xx(&AT24C02Buff[0], 1, 0x00, 0xA1, enumer);// ��
    Putc_to_SerialPort_Hex(AT24C02Buff[0]);
    userCount = AT24C02Buff[0];
    
// ��ȡ����
    while(k++ < userCount)
        RW24xx(&user[k].cardCode[0], 4, 8*k, 0xA1, enumer);// ��
}

char assertUserCode(unsigned char *cardCode)
{
    int i=0;
    for (i=0; i<10; i++)
    {
        if (i>=userCount)
            return -1;
        if ((user[i].cardCode[0] == cardCode[0]) && \
            (user[i].cardCode[1] == cardCode[1]) && \
            (user[i].cardCode[2] == cardCode[2]) && \
            (user[i].cardCode[3] == cardCode[3]))
        {
            return i;
        }
    }
    return -1;
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
    
    BEEP = 1;
    
	InitUART();
    InitTimer0();
    ds1302_init_time();
    InitInfrared();
    
    MotorStop();
    
    InitUserInfo();
    
    InitLcd();
    
    // ����ָ������
    LcdCls(1, 1, 128, 64);
    
	PcdReset();
	PcdAntennaOff();
    delay_ms(100);
	PcdAntennaOn();
    
    LCDDispalyMain();
    
    InitKey();
    
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
        // ����
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
                
                clock = 0;
                clsFlag = 0;
                IntrusionFlag = 0;
            }
        // ��������
            
            // �Ƿ��������  �����ٴ�ˢ��
            if (menuPage == IntrusionPage)
            {
                strcpy(lastCardType, "    ");
                strcpy(lastCardCode, "    ");
            }
            
            // ¼�����Ա��Ƭ
            if (menuPage == PressCardAdminPage)
            {
                if ((adminUser.cardCode[0] == cardCode[0]) && \
                    (adminUser.cardCode[1] == cardCode[1]) && \
                    (adminUser.cardCode[2] == cardCode[2]) && \
                    (adminUser.cardCode[3] == cardCode[3]))
                {
                // OLED��ʾ
                    // ����ָ������
                    LcdCls(1, 1, 128, 48);
                    Disp_String_8x16(1, 1, "Card Type: ");
                    Disp_String_Hex(3, 1, cardType[0]);
                    Disp_String_Hex(3, 17, cardType[1]);
                    
                    // Disp_String_8x16(5, 1, "Card Code: ");
                    Disp_String_Hex(5, 1, cardCode[0]);
                    Disp_String_Hex(5, 17, cardCode[1]);
                    Disp_String_Hex(5, 33, cardCode[2]);
                    Disp_String_Hex(5, 49, cardCode[3]);
                    
                    delay_ms(1000);
                    
                    menuPage = EnterNewCardPage;
                    // ��ʾ ¼���¿�
                    showEnterNewCark();
                    continue;
                }
                else
                {
                    // ��ʾ ����IC��Ȩ�޲���
                    showCarkNoPermission();
                    continue;
                }
            }
            // ¼���¿�
            if ((menuPage == EnterNewCardPage) || (menuPage == EnterNewCardSuccessPage))
            {
            // OLED��ʾ
                // ����ָ������
                // LcdCls(1, 1, 128, 48);
                int userNum;
                if ((userNum=assertUserCode(cardCode)) != -1)
                {
                    // ��ʾ ��ֵ�ɹ�
                    showRechargeSuccess();
                    menuPage = RechargeSuccessPage;
                }
                else
                {
                    // ��ʾ ¼��ɹ�
                    showEnterNewCardSuccess();
                    
                    //Disp_String_8x16(5, 1, "Card Code: ");
                    Disp_String_Hex(5, 1, cardCode[0]);
                    Disp_String_Hex(5, 17, cardCode[1]);
                    Disp_String_Hex(5, 33, cardCode[2]);
                    Disp_String_Hex(5, 49, cardCode[3]);
                    
                    user[userCount].cardCode[0] = cardCode[0];
                    user[userCount].cardCode[1] = cardCode[1];
                    user[userCount].cardCode[2] = cardCode[2];
                    user[userCount].cardCode[3] = cardCode[3];
                    user[userCount].cardCode[4] = 0x00;
                    
                    menuPage = EnterNewCardSuccessPage;
                }
                continue;
            }
            // ��ֵ�ɹ�����
            if (menuPage == RechargeSuccessPage)
            {
                continue;
            }
            
            // �ж��ظ�ˢ��
            if ((strcmp(lastCardType, cardType) != 0) || (strcmp(lastCardCode, cardCode) != 0))
            {
                strcpy(lastCardType, cardType);
                strcpy(lastCardCode, cardCode);
                if (assertUserCode(cardCode) != -1)
                {
                    menuPage = ShowIDPage;
                    
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
                    Putc_to_SerialPort_Hex(menuPage);
                    Putc_to_SerialPort_Hex(clock);
                // OLED��ʾ
                    // ����ָ������
                    // LcdCls(1, 1, 128, 48);
                    // ��ʾ ˢ���ɹ�
                    showEnterCardSuccess();
                    
                    // Disp_String_8x16(5, 1, "Card Code: ");
                    Disp_String_Hex(5, 1, cardCode[0]);
                    Disp_String_Hex(5, 17, cardCode[1]);
                    Disp_String_Hex(5, 33, cardCode[2]);
                    Disp_String_Hex(5, 49, cardCode[3]);
                }
                else
                {
                    Puts_to_SerialPort("assertUserCode False.\r\n");
                    // ��ʾ ��ϵ����Ա
                    showContactAdmin();
                    
                    menuPage = ContactAdminPage;
                    clock = 0;
                    clsFlag = 0;
                }
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
            menuPage = HomePage;
            TR0 = 1;    // ��ʱ��0 ��
        }
        
        // �Ƿ�����
        if (IntrusionFlag == 1)
        {
            menuPage = IntrusionPage;
            clock = 0;
            clsFlag = 0;
            
            Puts_to_SerialPort("Intrusion Error....");
            showIntrusion();
            BEEP = ~BEEP;
            delay_ms(500);
            BEEP = ~BEEP;
            IntrusionFlag = 0;
            TR0 = 1;    // ��ʱ��0 ��
        }
        
        // ¼���¿�
        if (KeyFlag == 1)
        {
            KeyFlag = 0;
            if ((menuPage == PressCardAdminPage) ||\
                (menuPage == EnterNewCardPage) ||\
                (menuPage == EnterNewCardSuccessPage) ||\
                (menuPage == RechargeSuccessPage))
            {
                clsFlag = 1;
                // ¼���¿��ɹ������ż���+1
                if (menuPage == EnterNewCardSuccessPage)
                {
                    userCount++;
                    
                    // д�����ݵ�AT24CXX��
                    AT24C02Buff[0] = userCount;
                    Putc_to_SerialPort_Hex(userCount);
                    RW24xx(&AT24C02Buff[0], 1, 0x00, 0xA0, enumer);// д userCount
                    
                    RW24xx(&user[userCount-1].cardCode[0], 4, (userCount-1)*8, 0xA0, enumer);// д user[0].cardCode
                }
                continue;
            }
            // ��ʾ �����Աˢ��
            showPressCardAdmin();
            TR0 = 0;    // �رն�ʱ��
            menuPage = PressCardAdminPage;
        }
        // ��ʾʱ��
        readCurrentTime(time);
        Disp_String_8x16(7, 1, time);
	}
}
