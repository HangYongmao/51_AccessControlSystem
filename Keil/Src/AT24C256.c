#include "main.h"
#include "I2C.h"
#include "AT24C256.h"

// AT24C01��AT24C256 �Ķ�д����
// DataBuff-��д���ݻ�����ָ��
//   Length-���ж�д���ֽ���
//     Addr-EEPROM��ַ AT24256Ϊ0��32767
//  Control-EEPROM�����ֽ�
//   enumer-EEPROM����
// ����1��ʾ�˴β���ʧЧ, 0��ʾ�����ɹ�;
// 1Kλ��2Kλ��4Kλ��8Kλ��16KλоƬ����һ��8λ�����ֽڵ�ַ�룬
// ����32Kλ���ϵĲ���2��8λ�����ֽڵ�ַ��ֱ��Ѱַ��
// ��4Kλ��8Kλ��16Kλ���ҳ���ַ��Ѱַ��
bit RW24xx(unsigned char *DataBuff, unsigned char Length, unsigned int Addr,
	unsigned char Control, enum EEPROMType enumer)
{

	unsigned char data j, i = ERROR;
	bit errorflag = 1;  // �����־
	while (i--)
	{
		Start();                // ��������
		Send(Control & 0xfe);   // ��IIC����д���ݣ�������ַ
		if (RecAck()) continue; // ��д����ȷ��������ѭ��
        
        // �����������32Kλ��ʹ��16λ��ַѰַ��д��߰�λ��ַ
		if (enumer > AT2416)
		{
			Send((unsigned char)(Addr >> 8));// ����������ת��Ϊ�ַ������ݣ�����ȡ�ͣ�ֻȡ��8λ.
			if (RecAck())
                continue;
		}
		Send((unsigned char)Addr);  // ��IIC����д����
		if (RecAck())
            continue;    // ��д��ȷ��������ѭ��
		if (!(Control & 0x01))      // �ж��Ƕ���������д����
		{
			j = Length;
			errorflag = 0;          // ���������λ
			while (j--)
			{
				Send(*DataBuff++);  // ��IIC����д����
				if (!RecAck())
                    continue;       // ��д��ȷ��������ѭ��
				errorflag = 1;
				break;
			}
			if (errorflag == 1)
                continue;
			break;
		}
		else
		{
			Start();        // ��������
			Send(Control);  // ��IIC����д����
			if (RecAck())
                continue;   // ����ûӦ��������α���ѭ��
			while (--Length)// �ֽڳ�Ϊ0����
			{
				*DataBuff++ = Receive();
				Ack();      // ��IIC���߲���Ӧ��
			}
			*DataBuff = Receive(); // �����һ���ֽ�
			NoAck();        // ����IIC���߲���Ӧ��
			errorflag = 0;
			break;
		}
	}
	Stop();  // ֹͣIIC����
	if (!(Control & 0x01))
	{
		Delay(255);
		Delay(255);
		Delay(255);
	}
	return(errorflag);
}