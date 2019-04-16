#ifndef __MINI12864_H__
#define __MINI12864_H__

#include "main.h"

// mini12864发送命令
void LcdSendCmd(uchar DAT);

// mini12864发送数据
void LcdSendData(uchar DAT);

// mini12864显示数据地址
void LcdAddr(uchar page, uchar column);

// mini12864清屏
void LcdCls(uchar page, uchar column,uchar x,uchar y);

// mini12864显示16x16
void Disp_16x16(uchar page, uchar column, uchar *dp);

// mini12864显示8x16
void Disp_String_8x16(uchar page, uchar column, uchar *text);

// mini12864显示8x16
void Disp_Char_8x16(uchar page, uchar column, uchar text);

// mini12864显示8x16 HEX
void Disp_String_Hex(uchar page, uchar column, uchar c);

// mini12864显示图片
void Disp_Picture(uchar page, uchar column,uchar x,uchar y, uchar *dp);

// mini12864初始化;
void InitLcd();

// 显示主界面
void LCDDispalyMain();

// 显示 非法闯入！
void showIntrusion();

// 显示 联系管理员
void showContactAdmin();

// 显示 请管理员刷卡
void showPressCardAdmin();

// 显示 录入新卡
void showEnterNewCark();

// 显示 刷卡成功
void showEnterCardSuccess();

// 显示 录入成功
void showEnterNewCardSuccess();

// 显示 所持IC卡权限不足
void showCarkNoPermission();

#endif
