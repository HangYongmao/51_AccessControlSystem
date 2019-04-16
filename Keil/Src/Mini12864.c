#include "Mini12864.h"
#include "Mini12864Font.h"
#include "main.h"
#include "DS1302.h"

extern unsigned char time[16];

/*====================================
����	��LcdSendCmd(uchar DAT)
����	��DAT��Ҫ���͵�����
����ֵ	����
����	��mini12864��������
====================================*/
void LcdSendCmd(uchar DAT)
{
	uchar i; 
	LCD12864_CS = 0;	//Ƭѡʹ�ܽţ� L��ʹ��Ƭѡ
	LCD12864_RS = 0; //����/����ѡ��ţ�L:���� H������
	for(i=0; i<8; i++) //�ֱ�д8�Σ�ÿ��д1λ
	{
		LCD12864_SCL = 0;//����ʱ�����ߣ�����SDA�仯
		if(DAT & 0x80)//��д�������λ
			LCD12864_SDA = 1;  //д1
		else
			LCD12864_SDA = 0;  //д0
		LCD12864_SCL = 1;	  //����ʱ�ӣ��ôӻ���SDA
		DAT <<= 1;	  //Ϊ������һλ����1λ
	}
}

/*====================================
����	��LcdSendData(uchar DAT)
����	��DAT��Ҫ���͵�����
����ֵ	����
����	��mini12864��������
====================================*/
void LcdSendData(uchar DAT)
{
	uchar i; 
	LCD12864_CS = 0;	//Ƭѡʹ�ܽţ� L��ʹ��Ƭѡ
	LCD12864_RS = 1; //����/����ѡ��ţ�L:���� H������
	for(i=0; i<8; i++) //�ֱ�д8�Σ�ÿ��д1λ
	{
		LCD12864_SCL = 0;//����ʱ�����ߣ�����SDA�仯
		if(DAT & 0x80)//��д�������λ
			LCD12864_SDA = 1;  //д1
		else
			LCD12864_SDA = 0;  //д0
		LCD12864_SCL = 1;	  //����ʱ�ӣ��ôӻ���SDA
		DAT <<= 1;	  //Ϊ������һλ����1λ
	}
}
/*====================================
����	��LcdAddr
����	��page������ʾҳ��ַ��column������ʾ�е�ַ
����ֵ	����
����	��mini12864��ʾ���ݵ�ַ
====================================*/
void LcdAddr(uchar page, uchar column)
{
	LCD12864_CS = 0;	//ʹ��Ƭѡ
	column = column - 1; //����ƽ����˵�ĵ�1 �У���LCD ����IC ���ǵ�0 �С������������ȥ1.
	/*����ҳ��ַ��ÿҳ��8 �С�һ�������64 �б��ֳ�8 ��ҳ������ƽ����˵�ĵ�1 ҳ��
	��LCD ����IC ���ǵ�0 ҳ�������������ȥ1*/
	page = page - 1;
	LcdSendCmd(0xb0+page); //����ҳ��ַ
	LcdSendCmd( ( (column >> 4) & 0x0f ) + 0x10 );//�����е�ַ�ĸ�4 λ
	LcdSendCmd(column & 0x0f);//�����е�ַ�ĵ�4 λ	
}

/*====================================
����	��LcdCls
����	��Disp_Picture
����	��page��ʾ��ҳ��ַ��column��ʾ���е�ַ��x�����ʾ�����y�����ʾ�����
����	��mini12864����
====================================*/
void LcdCls(uchar page, uchar column,uchar x,uchar y)
{
	uchar i,j;
	LCD12864_CS = 0;	//ʹ��Ƭѡ
	y = y / 8;//ҳ���� 
	for(j=0; j<y; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<x; i++)
		{
			LcdSendData(0); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
		}
	}
	LCD12864_CS = 1;	//Ƭѡ�ر�
}

/*====================================
����	��Disp_16x16
����	��page��ʾҳ��ַ��column��ʾ�е�ַ�� dp��ʾ����
����ֵ	����
����	��mini12864��ʾ16x16 ����ͼ�񡢺��֡���Ƨ�ֻ�16x16 ���������ͼ��
			��ʾ��������˵��16*16������ռ16��16��  ��Ӧռ2ҳ 16��
====================================*/
void Disp_16x16(uchar page, uchar column, uchar *dp)
{
	uchar i,j;
	LCD12864_CS = 0;	//ʹ��Ƭѡ
	for(j=0; j<2; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<16; i++)
		{
			LcdSendData(*dp); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	LCD12864_CS = 1;	//Ƭѡ�ر�
}
/*====================================
����	��Disp_String_8x16
����	��page��ʾҳ��ַ��column��ʾ�е�ַ�� text��ʾ����
����ֵ	����
����	��mini12864��ʾ8x16 ���֣��ַ���8x16 ���������ͼ��
			��ʾ��������˵��8*16������ռ8��16��  ��Ӧռ1ҳ 16��
====================================*/
void Disp_String_8x16(uchar page, uchar column, uchar *text)
{
	uint i=0,j,k,n;
	LCD12864_CS = 0;	//ʹ��Ƭѡ
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
					LcdSendData(ascii_table_8x16[j][k+8*n]);/*��ʾ8x16 ��ASCII �ֵ�LCD ��*/
				}
			}
			i++;
			column+=8;
		}
		else
			i++;
	}
	LCD12864_CS = 1;	//Ƭѡ�ر�
}

/*====================================
����	��Disp_String_8x16
����	��page��ʾҳ��ַ��column��ʾ�е�ַ�� text��ʾ����
����ֵ	����
����	��mini12864��ʾ8x16 ���֣��ַ���8x16 ���������ͼ��
			��ʾ��������˵��8*16������ռ8��16��  ��Ӧռ1ҳ 16��
====================================*/
void Disp_Char_8x16(uchar page, uchar column, uchar text)
{
	uint i=0,j,k,n;
	LCD12864_CS = 0;	//ʹ��Ƭѡ
    if((text>=0x20)&&(text<=0x7e))
    {
        j=text-0x20;
        for(n=0;n<2;n++)
        {
            LcdAddr(page+n, column);
            for(k=0;k<8;k++)
            {
                LcdSendData(ascii_table_8x16[j][k+8*n]);/*��ʾ8x16 ��ASCII �ֵ�LCD ��*/
            }
        }
        i++;
        column+=8;
    }
	LCD12864_CS = 1;	//Ƭѡ�ر�
}

/*====================================
����	��Disp_String_8x16 Hex
����	��page��ʾҳ��ַ��column��ʾ�е�ַ�� text��ʾ����
����ֵ	����
����	��mini12864��ʾ8x16 ���֣��ַ���8x16 ���������ͼ��
			��ʾ��������˵��8*16������ռ8��16��  ��Ӧռ1ҳ 16��
            0xF0��ʾΪF0 �ַ�
====================================*/
void Disp_String_Hex(uchar page, uchar column, uchar c)
{
	LCD12864_CS = 0;	//ʹ��Ƭѡ
    // ��λ����
    if ((c>>4) < 10)
        Disp_Char_8x16(page, column, (c>>4) + '0');
    else
        Disp_Char_8x16(page, column, (c>>4) - 10 + 'A');
    
    // ��λ����
    if ((c&0xf) < 10)
        Disp_Char_8x16(page, column+8, (c&0xf) + '0');
    else
        Disp_Char_8x16(page, column+8, (c&0xf) - 10 + 'A');
	LCD12864_CS = 1;	//Ƭѡ�ر�
}

/*====================================
����	��Disp_Picture
����	��page��ʾҳ��ַ��column��ʾ�е�ַ��xͼƬ��yͼƬ�ߣ� dp��ʾ����
����ֵ	����
����	��mini12864��ʾͼƬ
====================================*/
void Disp_Picture(uchar page, uchar column,uchar x,uchar y, uchar *dp)
{
	uchar i,j;
	LCD12864_CS = 0;	//ʹ��Ƭѡ
	y = y / 8;//ҳ���� 
	for(j=0; j<y; j++)
	{
		LcdAddr(page+j, column);
		for (i=0; i<x; i++)
		{
			LcdSendData(*dp); /*д���ݵ�LCD,ÿд��һ��8 λ�����ݺ��е�ַ�Զ���1*/
			dp++;
		}
	}
	LCD12864_CS = 1;	//Ƭѡ�ر�
}

/*====================================
����	��InitLcd()
����	����
����ֵ	����
����	��mini12864��ʼ��
====================================*/
void InitLcd()
{
	LCD12864_CS = 0;	//ʹ��Ƭѡ
	LCD12864_RST= 0;	//ϵͳ��λ
	delay_ms(1);
	LCD12864_RST= 1; //ϵͳ��λ���
	delay_ms(1);
	LcdSendCmd(0xe2);	//�����λ��ִ������ָ��֮�󣬵�·���������λ״̬�����Ĵ���ֵ�����λ״̬�Ĵ�����
	LcdSendCmd(0x2f);	//ΪҺ������ѹ���ṩ�����ѹ
	LcdSendCmd(0x23);	//�ֵ��Աȶȣ������÷�Χ0x20��0x27
	LcdSendCmd(0x81);	//˫��ָ�� ����EV΢���Աȶȵ�ֵ
	LcdSendCmd(0x2f);	//EV�����÷�Χ0x00��0x3f
	LcdSendCmd(0xa2);	//1/9 ƫѹ�ȣ�bias��ȫ����ʾ
	LcdSendCmd(0xc8);	//��ɨ��˳�򣺴��ϵ���
	LcdSendCmd(0xa0);   //��ɨ��˳�򣺴�����
	LcdSendCmd(0x40);	//��ʼ�У���һ�п�ʼ
	LcdSendCmd(0xaf);	//����ʾ
	LCD12864_CS = 1;	//Ƭѡ�ر�			
}

// ��ʾ������
void LCDDispalyMain()
{
    int i;
    // ����ָ������
    LcdCls( 1, 1, 128, 64);
    Disp_Picture(1, 1, 128, 16, welcome_bmp);
    
    // �ǻ۳���
    for (i=0; i<4; i++)
    {
        Disp_16x16(3, 33+16*i, &MainName[32*i]);
    }
    for (i=0; i<6; i++)
    {
        Disp_16x16(5, 17+16*i, &MainName[32*i+128]);
    }
    
    // ��ʾʱ��
    readCurrentTime(time);
    Disp_String_8x16(7, 1, time);
}

// ��ʾ �Ƿ����룡
void showIntrusion()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 29+16*i, &Intrusion[32*i]);
    }
}

// ��ʾ ��ϵ����Ա
void showContactAdmin()
{
    int i;
    // ����ָ������
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

// ��ʾ �����Աˢ��
void showPressCardAdmin()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<6; i++)
    {
        Disp_16x16(3, 17+16*i, &pressCardAdmin[32*i]);
    }
}

// ��ʾ ¼���¿�
void showEnterNewCark()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 25+16*i, &EnterNewCark[32*i]);
    }
}

// ��ʾ ˢ���ɹ�
void showEnterCardSuccess()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<5; i++)
    {
        Disp_16x16(3, 29+16*i, &EnterCardSuccess[32*i]);
    }
}

// ��ʾ ¼��ɹ�
void showEnterNewCardSuccess()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<4; i++)
    {
        Disp_16x16(3, 29+16*i, &EnterNewCardSuccess[32*i]);
    }
}

// ��ʾ ����IC��Ȩ�޲���
void showCarkNoPermission()
{
    int i;
    // ����ָ������
    LcdCls(1, 1, 128, 48);
    for (i=0; i<8; i++)
    {
        Disp_16x16(3, 1+16*i, &CarkNoPermission[32*i]);
    }
}