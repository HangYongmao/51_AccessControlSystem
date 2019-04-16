#ifndef __MINI12864_H__
#define __MINI12864_H__

#include "main.h"

// mini12864��������
void LcdSendCmd(uchar DAT);

// mini12864��������
void LcdSendData(uchar DAT);

// mini12864��ʾ���ݵ�ַ
void LcdAddr(uchar page, uchar column);

// mini12864����
void LcdCls(uchar page, uchar column,uchar x,uchar y);

// mini12864��ʾ16x16
void Disp_16x16(uchar page, uchar column, uchar *dp);

// mini12864��ʾ8x16
void Disp_String_8x16(uchar page, uchar column, uchar *text);

// mini12864��ʾ8x16
void Disp_Char_8x16(uchar page, uchar column, uchar text);

// mini12864��ʾ8x16 HEX
void Disp_String_Hex(uchar page, uchar column, uchar c);

// mini12864��ʾͼƬ
void Disp_Picture(uchar page, uchar column,uchar x,uchar y, uchar *dp);

// mini12864��ʼ��;
void InitLcd();

// ��ʾ������
void LCDDispalyMain();

// ��ʾ �Ƿ����룡
void showIntrusion();

// ��ʾ ��ϵ����Ա
void showContactAdmin();

// ��ʾ �����Աˢ��
void showPressCardAdmin();

// ��ʾ ¼���¿�
void showEnterNewCark();

// ��ʾ ˢ���ɹ�
void showEnterCardSuccess();

// ��ʾ ¼��ɹ�
void showEnterNewCardSuccess();

// ��ʾ ����IC��Ȩ�޲���
void showCarkNoPermission();

#endif
