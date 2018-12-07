/*******************************************************************************  
* �ļ����ƣ��������ʵ��
* ʵ��Ŀ�ģ�1.���վ������ɨ��Ļ���ԭ��
*           2.���հ���ɨ�輰���������Ļ���ԭ��
* ����˵����1.ʹ�ó���ǰ��������J2����ΪKBDģʽ
*           2.�������ܿɲο�����������:void key_proc(unsigned char key);
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���

bit key_re;
unsigned char key_press;
unsigned char key_value;

bit key_flag = 0;
unsigned char intr = 0;

unsigned char read_keyboard(void);
void key_proc(unsigned char key);

//������
void main(void)
{ 
    unsigned char key_temp = 0xff;

    TMOD &= 0xf0;    
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
            key_temp = read_keyboard();
        } 
        
        key_proc(key_temp);        
        key_temp = 0xff;  //����ɼ�ֵ
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������ 
     
	if(++intr == 10)  //2msִ��һ��
	{
        intr = 0;
		key_flag = 1;  //20ms����ɨ���־λ��1
    }
}

//��ȡ������̼�ֵ
unsigned char read_keyboard(void)
{
    unsigned char key_temp;
    static unsigned char col;
      
    key_temp = (P3&0xf0);

    if(key_temp != 0xf0) //�а�������
        key_press++;
	else
		key_press = 0;  //����
    
    if(key_press == 3)
    {
		key_press = 0;
		key_re = 1;
		switch(key_temp)
        {
            case 0x70:
                col = 1;  //��һ�а�������
                break;                
            case 0xb0:
                col = 2;  //�ڶ��а�������
                break;    
            case 0xd0:
                col = 3;  //�����а�������
                break;    
            case 0xe0:
                col = 4;  //�����а�������
                break;
        }
        
        key_temp = (P3&0x0f);
        
		switch(key_temp)
        {
            case 0x0e:
                key_value = (col-1);   //��һ�а�������
                break;                
            case 0x0d:
                key_value = (3+col);   //�ڶ��а�������
                break;    
            case 0x0b:
                key_value = (7+col);   //�����а�������
                break;    
            case 0x07:
                key_value = (11+col);  //�����а�������
                break;
        }
    }
    //�������μ�⵽���������£����Ҹð����Ѿ��ͷ�

	key_temp = (P3&0x0f);
	
    if((key_re == 1) && (key_temp == 0x0f))
    {
        key_re = 0;
        return key_value;
    }
    else
     {return 0xff;}  //�ް������»򱻰��µİ���δ���ͷ� 
}


//����������:S7,S11
void key_proc(unsigned char key)
{
    switch(key)
    {
        case 0:  //
            P2 = ((P2&0x1f)|0x80);
			P0++;
			P2 &= 0x1f;
            break;
        case 1:  //
            P2 = ((P2&0x1f)|0x80);
			P0--;
			P2 &= 0x1f;
            break;
    }    
}