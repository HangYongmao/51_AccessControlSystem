#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

typedef signed char int8;  //8λ�з�����
typedef unsigned char INT8U;
typedef signed int  int16; //16λ�з�����
typedef unsigned char uint8;  //8λ�޷�����
typedef unsigned char uchar;  //8λ�޷�����
typedef unsigned int  uint16; //16λ�޷�����
typedef unsigned int INT16U;
typedef unsigned int  uint; //16λ�޷�����
typedef unsigned long uint32; //32λ�޷�����

void delay_ms(unsigned int z);

//////////////////////////////////
//�˿ڶ���
// RC522
sbit MF522_SDA  = P1 ^ 0; //SDA
sbit MF522_SCK  = P1 ^ 1; //SCK
sbit MF522_MOSI = P1 ^ 2; //MOSI
sbit MF522_MISO = P1 ^ 3; //MISO
sbit MF522_RST  = P1 ^ 4; //RST

// DS1302
sbit D_IO   = P3 ^ 6;
sbit D_SCLK = P3 ^ 7;
sbit D_RST  = P3 ^ 5;

// ����������ƽӿڶ���
#define MotorData P2

// LCD12864Һ������
sbit LCD12864_CS = P0 ^ 3;	//Ƭѡʹ�ܽţ� L��ʹ��Ƭѡ
sbit LCD12864_RST= P0 ^ 4;	//ϵͳ��λѡ���
sbit LCD12864_RS = P0 ^ 5;	//����/����ѡ��ţ�L:���� H������
sbit LCD12864_SCL= P0 ^ 6;	//����/����ѡ��ţ�L:���� H������
sbit LCD12864_SDA= P0 ^ 7;	//������������/�����

#endif
