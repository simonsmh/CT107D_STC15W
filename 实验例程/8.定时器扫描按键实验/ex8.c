/*******************************************************************************  
* �ļ����ƣ���ʱ��ɨ�谴��ʵ��
* ʵ��Ŀ�ģ�1.����ͨ����ʱ��ɨ�谴����ֵ�Ļ�������
*           2.����51��Ƭ����ʱ������ģʽ�����÷���
* ����˵����ʹ�ó���ʱ��������J2����ΪBTNģʽ
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "absacc.h"

bit key_re;
unsigned char key_press;
unsigned char key_value;
bit key_flag;
unsigned char intr;
                           //  0    1    2    3    4    5    6    7    8    9
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff};
unsigned char dspbuf[8] = {10,10,10,10,10,0,0,0};  //��ʾ������
unsigned char dspcom = 0;

unsigned char read_key(void);
void display(void);

//������
void main(void)
{ 
    unsigned char key_temp;
	unsigned char num;
    
    P3 = 0xff;  //�����ӿڳ�ʼ��
    
    TMOD |= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��
    
    while(1)
    {
        if(key_flag)
        {
            key_flag = 0;
            key_temp = read_key();
        }        
        //���밴��������򲿷�
        switch(key_temp)
        {
            case 1:
				num++;
                break;
            case 2:
				num--;
                break;      
        }
		key_temp = 0;  //��ɰ����������������ֵ
		
		dspbuf[5] = num/100;
		dspbuf[6] = num%100/10;
		dspbuf[7] = num%10;
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������ 
    display(); 
	if(++intr == 10)  //2msִ��һ��
	{
        intr = 0;
		key_flag = 1;  //20ms����ɨ���־λ��1
    }
}

//��ȡ��ֵ
unsigned char read_key(void)
{
	
    unsigned char key_temp;
    
	key_temp = (P3&0x0f);
	
    if(key_temp != 0x0f) //�а�������
        key_press++;
	else
		key_press = 0;  //����
    
    if(key_press == 5)
    {
		key_press = 0;
		key_re = 1;
		switch(key_temp)
        {
            case 0x0e:
                key_value = 1;  //
                break;                
            case 0x0d:
                key_value = 2;  //
                break;    
            case 0x0b:
                key_value = 3;  //
                break;    
            case 0x07:
                key_value = 4;  //
                break;
        }
    }
    //�������μ�⵽���������£����Ҹð����Ѿ��ͷ�
    if((key_re == 1) && (key_temp == 0x0f))
    {
        key_re = 0;
        return key_value;
    }
    
    return 0xff;  //�ް������»򱻰��µİ���δ���ͷ�    
}

//��ʾ����
void display(void)
{   
	XBYTE[0xE000] = 0xff;  //����
	
	XBYTE[0xC000] = (1<<dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; //����
   
    if(++dspcom == 8){
        dspcom = 0;
    }   
}
