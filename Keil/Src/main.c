/*
*  基于ISO/IEC14443 Type A的智慧车库门禁控制系统设计与实现
*  CPU:STC89C52RC
*  晶振:11.0592MHZ
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

unsigned char g_ucTempbuf[20];
bit clsFlag=0;  // 1-需要清屏

enum MenuPage menuPage=HomePage;
extern unsigned char clock;
unsigned char time[16];
bit IntrusionFlag = 0;  // 1-非法闯入

bit KeyFlag=0;    // 1-录入新卡

UserInfo user[20];
unsigned char userCount=0;
UserInfo adminUser={{0xAC, 0xEC, 0x45, 0xD2, 0x00}, 99, 12, 30}; // 管理员

void delay_ms(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x>0; x--)
	for (y = 110; y>0; y--);
}

void InitUserInfo()
{
    user[0].cardCode[0] = 0xAC;
    user[0].cardCode[1] = 0xEC;
    user[0].cardCode[2] = 0x45;
    user[0].cardCode[3] = 0xD2;
    user[0].cardCode[4] = 0x00;
    userCount++;
}

bit assertUserCode(unsigned char *cardCode)
{
    int i=0;
    for (i=0; i<10; i++)
    {
        if (i>=userCount)
            return 0;
        if ((user[i].cardCode[0] == cardCode[0]) && \
            (user[i].cardCode[1] == cardCode[1]) && \
            (user[i].cardCode[2] == cardCode[2]) && \
            (user[i].cardCode[3] == cardCode[3]))
        {
            return 1;
        }
    }
    return 0;
}

int main()
{
    uint k;
	unsigned char status, i;
	unsigned char cardType[3]={0};
    unsigned char cardCode[5]={0};
    
    // 上一次的卡信息
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
    
    // 清屏指定区域
    LcdCls(1, 1, 128, 64);
    
	PcdReset();
	PcdAntennaOff();
    delay_ms(100);
	PcdAntennaOn();
    
    LCDDispalyMain();
    
    InitKey();
    
	while (1)
	{
		status = PcdRequest(PICC_REQALL, g_ucTempbuf);//寻卡
		if (status != MI_OK)
		{
			PcdReset();
			PcdAntennaOff();
			PcdAntennaOn();
		}
        else
        {
        // 读卡
            for (i = 0; i<2; i++)
            {
                cardType[i] = g_ucTempbuf[i];
            }

            status = PcdAnticoll(g_ucTempbuf);// 防冲撞
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
        // 读卡结束
            
            // 非法闯入界面  允许再次刷卡
            if (menuPage == IntrusionPage)
            {
                strcpy(lastCardType, "    ");
                strcpy(lastCardCode, "    ");
            }
            
            // 录入管理员卡片
            if (menuPage == PressCardAdminPage)
            {
                if ((adminUser.cardCode[0] == cardCode[0]) && \
                    (adminUser.cardCode[1] == cardCode[1]) && \
                    (adminUser.cardCode[2] == cardCode[2]) && \
                    (adminUser.cardCode[3] == cardCode[3]))
                {
                // OLED显示
                    // 清屏指定区域
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
                    // 显示 录入新卡
                    showEnterNewCark();
                    continue;
                }
                else
                {
                    // 显示 所持IC卡权限不足
                    showCarkNoPermission();
                    continue;
                }
            }
            // 录入新卡
            if ((menuPage == EnterNewCardPage) || (menuPage == EnterNewCardSuccessPage))
            {
            // OLED显示
                // 清屏指定区域
                // LcdCls(1, 1, 128, 48);
                
                // 显示 录入成功
                showEnterNewCardSuccess();
                
                // Disp_String_8x16(5, 1, "Card Code: ");
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
                
                continue;
            }
            
            // 判断重复刷卡
            if ((strcmp(lastCardType, cardType) != 0) || (strcmp(lastCardCode, cardCode) != 0))
            {
                strcpy(lastCardType, cardType);
                strcpy(lastCardCode, cardCode);
                if (assertUserCode(cardCode))
                {
                    menuPage = ShowIDPage;
                    
                    for(k=0; k<5; k++)
                    {
                        MotorCW();   //顺时针转动
                    }
                    
                // 串口输出
                    // 卡片类型
                    Puts_to_SerialPort("Card Type: ");
                    Putc_to_SerialPort_Hex(cardType[0]);
                    Putc_to_SerialPort_Hex(cardType[1]);
                    Puts_to_SerialPort("\r\n");
                    
                    // 卡片序列号
                    Puts_to_SerialPort("Card Code: ");
                    Putc_to_SerialPort_Hex(cardCode[0]);
                    Putc_to_SerialPort_Hex(cardCode[1]);
                    Putc_to_SerialPort_Hex(cardCode[2]);
                    Putc_to_SerialPort_Hex(cardCode[3]);
                    Puts_to_SerialPort("\r\n");
                    
                    Putc_to_SerialPort_Hex(clsFlag);
                    Putc_to_SerialPort_Hex(menuPage);
                    Putc_to_SerialPort_Hex(clock);
                // OLED显示
                    // 清屏指定区域
                    // LcdCls(1, 1, 128, 48);
                    // 显示 刷卡成功
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
                    // 显示 联系管理员
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
            TR0 = 1;    // 定时器0 打开
        }
        
        // 非法闯入
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
            TR0 = 1;    // 定时器0 打开
        }
        
        // 录入新卡
        if (KeyFlag == 1)
        {
            KeyFlag = 0;
            if ((menuPage == PressCardAdminPage) || (menuPage == EnterNewCardPage) || (menuPage == EnterNewCardSuccessPage))
            {
                clsFlag = 1;
                // 录入新卡成功，卡号计数+1
                if (menuPage == EnterNewCardSuccessPage)
                    userCount++;
                continue;
            }
            // 显示 请管理员刷卡
            showPressCardAdmin();
            TR0 = 0;    // 关闭定时器
            menuPage = PressCardAdminPage;
        }
        // 显示时间
        readCurrentTime(time);
        Disp_String_8x16(7, 1, time);
	}
}
