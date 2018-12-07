/*******************************************************************************  
* �ļ����ƣ���������LEDλ��
* ʵ��Ŀ�ģ�1.���յ�Ƭ��IO�ڲ����Ļ�������
*           2.���հ���ɨ�輰�����ʱ���������Ļ���ԭ��
* ����˵����1.ʹ�ó���ǰ��������J2����ΪBTNģʽ
*           2.��������ʹ�õ������ʱ���������ή�͵�Ƭ��ϵͳ��ʵʱ�ԣ���Ŀ������
*             ��ⰴ�����������Ĺ��̺������������ں�����ʵ�������л��𲽽��ܻ���
*             ��ʱ��ɨ�������������
*           3.��ذ������ܿ��Բο�����������void key_proc(unsigned char key)
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���

unsigned char read_key(void);
void key_proc(unsigned char key);
void delay(void);

//������
void main(void)
{ 
    unsigned char temp;
    
	P0 = 0xff;  //LED�ӿڳ�ʼ��
    
    while(1)
    {
        temp = read_key();
        if(temp != 0xff)
        {
            delay();  //��������
            temp = read_key();
            if(temp != 0xff)
            {
                //���밴��������
                key_proc(temp);
                while(P3 != 0xff);  //�ȴ������ͷ�                        
            }
        }
    }
}

//����ɨ�躯��
unsigned char read_key(void)
{
    unsigned char temp;
    unsigned char key_value = 0xff;
    
    temp = P3&0x0f;
    switch(temp)
    {
        case 0x0e:
            key_value = 1;  //S7
            break;    
        case 0x0d:
            key_value = 2;  //S6
            break;    
        case 0x0b:
            key_value = 3;  //S5
            break;    
        case 0x07:
            key_value = 4;  //S4
            break;    
    }
    return key_value;
}

//����������
void key_proc(unsigned char key)
{
    switch(key)
    {
        case 1:
			P2 = (P2&0x1f|0x80);
			P0++;
			P2 &= 0x1f;
            break;
        case 2:
			P2 = (P2&0x1f|0x80);
			P0--;
			P2 &= 0x1f;
        case 3:
			P2 = (P2&0x1f|0x80);
			P0 += 2;
			P2 &= 0x1f;
            break;
        case 4:
			P2 = (P2&0x1f|0x80);
			P0 -= 2;
			P2 &= 0x1f;
            break;
    }    
}

//��ʱ����
void delay(void)
{
	unsigned char i,j;
	
	for(i=0; i<50; i++)
	{
		for(j=0; j<248; j++);
	}
}