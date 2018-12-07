/*******************************************************************************  
* �ļ����ƣ�ADCת��ʵ��
* ʵ��Ŀ�ģ�1.����I2C����ͨѶ�����ص�͹���ʱ��
*           2.����51��Ƭ��ģ��I2C����ʱ��ĳ�����Ʒ���
*           3.����PCF8591 ADCоƬ�Ĳ�������
* ����˵����1.ʹ�ó���ǰ�� Preprocessor Symbols �����Ӻ궨��ADC_PCF8591
*           2.���ڵ�λ��Rb2���۲��������ʾ���
* ���ڰ汾��2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "i2c.h"    //I2C���������� 
//#include "absacc.h"

code unsigned char LedChar[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char LedBuf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  //��ʾ������
unsigned char dspcom = 0;
unsigned char intr;
bit adc_flag;

void Count(unsigned char val);
void LedScan();

//������
void main(void)
{ 
    unsigned char val;  //ADCת������ 
		
    TMOD |= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��

	init_pcf8591();  //PCF8591��ʼ��      
    
    while(1)
    {
		if(adc_flag)
		{
			adc_flag = 0;
			val = adc_pcf8591();
			Count(val); 
		}
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������ 
    if(++intr == 50)
	{
		intr = 0;
		adc_flag = 1;
	} 
    LedScan();
}

void Count(unsigned char val)
{
	unsigned char buf[8],i;
	buf[0] = val/100%10;
	buf[1] = val/10%10;
	buf[2] = val%10;

	for(i=0; i<3; i++)
	{
		LedBuf[i] = LedChar[buf[i]];
	}
	LedBuf[0] &= 0x7f;       //��С����
}  

void LedScan()
{
	static unsigned char i = 0;

	P2 = (P2&0x1f)|0xc0;
	P0 = 0x01 << i; 
	P2 = (P2&0x1f)|0xe0;
	P0 = LedBuf[i];

	if(i < 8)
		i++;
	else
		i = 0;
}