#include "Mini12864.h"
#include "Mini12864Font.h"
#include "main.h"
#include "DS1302.h"

extern unsigned char time[16];

/*====================================
函数	：LcdSendCmd(uchar DAT)
参数	：DAT需要发送的命令
返回值	：无
描述	：mini12864发送命令
====================================*/
void LcdSendCmd(uchar DAT)
{
	uchar i; 
	LCD12864_CS = 0;	//片选使能脚， L：使能片选
	LCD12864_RS = 0; //命令/数据选择脚，L:命令 H：数据
	for(i=0; i<8; i++) //分别写8次，每次写1位
	{
		LCD12864_SCL = 0;//拉低时钟总线，允许SDA变化
		if(DAT & 0x80)//先写数据最高位
			LCD12864_SDA = 1;  //写1
		else
			LCD12864_SDA = 0;  //写0
		LCD12864_SCL = 1;	  //拉高时钟，让从机读SDA
		DAT <<= 1;	  //为发送下一位左移1位
	}
}

/*====================================
函数	：LcdSendData(uchar DAT)
参数	：DAT需要发送的数据
返回值	：无
描述	：mini12864发送数据
====================================*/
void LcdSendData(uchar DAT)
{
	uchar i; 
	LCD12864_CS = 0;	//片选使能脚， L：使能片选
	LCD12864_RS = 1; //命令/数据选择脚，L:命令 H：数据
	for(i=0; i<8; i++) //分别写8次，每次写1位
	{
		LCD12864_SCL = 0;//拉低时钟总线，允许SDA变化
		if(DAT & 0x80)//先写数据最高位
			LCD12864_SDA = 1;  //写1
		else
			LCD12864_SDA = 0;  //写0
		LCD12864_SCL = 1;	  //拉高时钟，让从机读SDA
		DAT <<= 1;	  //为发送下一位左移1位
	}
}
/*====================================
函数	：LcdAddr
参数	：page设置显示页地址，column设置显示列地址
返回值	：无
描述	：mini12864显示数据地址
====================================*/
void LcdAddr(uchar page, uchar column)
{
	LCD12864_CS = 0;	//使能片选
	column = column - 1; //我们平常所说的第1 列，在LCD 驱动IC 里是第0 列。所以在这里减去1.
	/*设置页地址。每页是8 行。一个画面的64 行被分成8 个页。我们平常所说的第1 页，
	在LCD 驱动IC 里是第0 页，所以在这里减去1*/
	page = page - 1;
	LcdSendCmd(0xb0+page); //设置页地址
	LcdSendCmd( ( (column >> 4) & 0x0f ) + 0x10 );//设置列地址的高4 位
	LcdSendCmd(column & 0x0f);//设置列地址的低4 位	
}

/*====================================
函数	：LcdCls
函数	：Disp_Picture
参数	：page显示区页地址，column显示区行地址，x清除显示区域宽，y清除显示区域高
描述	：mini12864清屏
====================================*/
void LcdCls(uchar page, uchar column,uchar x,uchar y)
{
	uchar i,j;
	LCD12864_CS = 0;	//使能片选
	y = y / 8;//页设置 
	for(j=0; j<y; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<x; i++)
		{
			LcdSendData(0); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
		}
	}
	LCD12864_CS = 1;	//片选关闭
}

/*====================================
函数	：Disp_16x16
参数	：page显示页地址，column显示行地址， dp显示内容
返回值	：无
描述	：mini12864显示16x16 点阵图像、汉字、生僻字或16x16 点阵的其他图标
			显示坐标设置说明16*16的字体占16行16列  对应占2页 16列
====================================*/
void Disp_16x16(uchar page, uchar column, uchar *dp)
{
	uchar i,j;
	LCD12864_CS = 0;	//使能片选
	for(j=0; j<2; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<16; i++)
		{
			LcdSendData(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
			dp++;
		}
	}
	LCD12864_CS = 1;	//片选关闭
}
/*====================================
函数	：Disp_String_8x16
参数	：page显示页地址，column显示行地址， text显示内容
返回值	：无
描述	：mini12864显示8x16 数字，字符或8x16 点阵的其他图标
			显示坐标设置说明8*16的字体占8行16列  对应占1页 16列
====================================*/
void Disp_String_8x16(uchar page, uchar column, uchar *text)
{
	uint i=0,j,k,n;
	LCD12864_CS = 0;	//使能片选
	while(text[i]>0x00)
	{
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{
				LcdAddr(page+n, column);
				for(k=0;k<8;k++)
				{
					LcdSendData(ascii_table_8x16[j][k+8*n]);/*显示8x16 的ASCII 字到LCD 上*/
				}
			}
			i++;
			column+=8;
		}
		else
			i++;
	}
	LCD12864_CS = 1;	//片选关闭
}

/*====================================
函数	：Disp_String_8x16
参数	：page显示页地址，column显示行地址， text显示内容
返回值	：无
描述	：mini12864显示8x16 数字，字符或8x16 点阵的其他图标
			显示坐标设置说明8*16的字体占8行16列  对应占1页 16列
====================================*/
void Disp_Char_8x16(uchar page, uchar column, uchar text)
{
	uint i=0,j,k,n;
	LCD12864_CS = 0;	//使能片选
    if((text>=0x20)&&(text<=0x7e))
    {
        j=text-0x20;
        for(n=0;n<2;n++)
        {
            LcdAddr(page+n, column);
            for(k=0;k<8;k++)
            {
                LcdSendData(ascii_table_8x16[j][k+8*n]);/*显示8x16 的ASCII 字到LCD 上*/
            }
        }
        i++;
        column+=8;
    }
	LCD12864_CS = 1;	//片选关闭
}

/*====================================
函数	：Disp_String_8x16 Hex
参数	：page显示页地址，column显示行地址， text显示内容
返回值	：无
描述	：mini12864显示8x16 数字，字符或8x16 点阵的其他图标
			显示坐标设置说明8*16的字体占8行16列  对应占1页 16列
            0xF0显示为F0 字符
====================================*/
void Disp_String_Hex(uchar page, uchar column, uchar c)
{
	LCD12864_CS = 0;	//使能片选
    // 高位数据
    if ((c>>4) < 10)
        Disp_Char_8x16(page, column, (c>>4) + '0');
    else
        Disp_Char_8x16(page, column, (c>>4) - 10 + 'A');
    
    // 低位数据
    if ((c&0xf) < 10)
        Disp_Char_8x16(page, column+8, (c&0xf) + '0');
    else
        Disp_Char_8x16(page, column+8, (c&0xf) - 10 + 'A');
	LCD12864_CS = 1;	//片选关闭
}

/*====================================
函数	：Disp_Picture
参数	：page显示页地址，column显示行地址，x图片宽，y图片高， dp显示内容
返回值	：无
描述	：mini12864显示图片
====================================*/
void Disp_Picture(uchar page, uchar column,uchar x,uchar y, uchar *dp)
{
	uchar i,j;
	LCD12864_CS = 0;	//使能片选
	y = y / 8;//页设置 
	for(j=0; j<y; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<x; i++)
		{
			LcdSendData(*dp); /*写数据到LCD,每写完一个8 位的数据后列地址自动加1*/
			dp++;
		}
	}
	LCD12864_CS = 1;	//片选关闭
}

/*====================================
函数	：InitLcd()
参数	：无
返回值	：无
描述	：mini12864初始化
====================================*/
void InitLcd()
{
	LCD12864_CS = 0;	//使能片选
	LCD12864_RST= 0;	//系统复位
	delay_ms(1);
	LCD12864_RST= 1; //系统复位完成
	delay_ms(1);
	LcdSendCmd(0xe2);	//软件复位，执行这条指令之后，电路进入软件复位状态，各寄存器值详见复位状态寄存器表。
	LcdSendCmd(0x2f);	//为液晶屏升压，提供所需电压
	LcdSendCmd(0x23);	//粗调对比度，可设置范围0x20～0x27
	LcdSendCmd(0x81);	//双行指令 设置EV微调对比度的值
	LcdSendCmd(0x2f);	//EV可设置范围0x00～0x3f
	LcdSendCmd(0xa2);	//1/9 偏压比（bias）全屏显示
	LcdSendCmd(0xc8);	//行扫描顺序：从上到下
	LcdSendCmd(0xa0);   //列扫描顺序：从左到右
	LcdSendCmd(0x40);	//起始行：第一行开始
	LcdSendCmd(0xaf);	//开显示
	LCD12864_CS = 1;	//片选关闭			
}

// 显示主界面
void LCDDispalyMain()
{
    int i;
    // 清屏指定区域
    LcdCls( 1, 1, 128, 64);
    Disp_Picture(1, 1, 128, 16, welcome_bmp);
    
    // 智慧车库
    for (i=0; i<4; i++)
    {
        Disp_16x16(3, 33+16*i, &MainName[32*i]);
    }
    for (i=0; i<6; i++)
    {
        Disp_16x16(5, 17+16*i, &MainName[32*i+128]);
    }
    
    // 显示时间
    readCurrentTime(time);
    Disp_String_8x16(7, 1, time);
}

// 显示 非法闯入！
void showIntrusion()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 29+16*i, &Intrusion[32*i]);
    }
}

// 显示 联系管理员
void showContactAdmin()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<8; i++)
    {
        Disp_16x16(2, 1+16*i, &ContactAdmin[32*i]);
    }
    for (i=0; i<6; i++)
    {
        Disp_16x16(4, 41+16*i, &addressAdmin[32*i]);
    }
}

// 显示 请管理员刷卡
void showPressCardAdmin()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<6; i++)
    {
        Disp_16x16(3, 17+16*i, &pressCardAdmin[32*i]);
    }
}

// 显示 录入新卡
void showEnterNewCark()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 25+16*i, &EnterNewCark[32*i]);
    }
}

// 显示 刷卡成功
void showEnterCardSuccess()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 29+16*i, &EnterCardSuccess[32*i]);
    }
}

// 显示 录入成功
void showEnterNewCardSuccess()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<4; i++)
    {
        Disp_16x16(3, 29+16*i, &EnterNewCardSuccess[32*i]);
    }
}

// 显示 所持IC卡权限不足
void showCarkNoPermission()
{
    int i;
    // 清屏指定区域
    LcdCls(1, 1, 128, 48);
    for (i=0; i<8; i++)
    {
        Disp_16x16(3, 1+16*i, &CarkNoPermission[32*i]);
    }
}