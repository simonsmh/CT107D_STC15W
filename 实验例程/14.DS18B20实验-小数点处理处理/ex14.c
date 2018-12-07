/*******************************************************************************  
* �ļ����ƣ�DS18B20�¶ȴ�����ʵ��-С���㴦��
* ʵ��Ŀ�ģ�1.���ճ��õ��ַ���������
*           2.����DS18B20�¶ȴ������Ĳ�������
* ����˵����1.ͨ���������ʵʱ�¶����ݣ�ʹ�ô��ڵ��Թ��ߵ��Գ���
*           2.����ͨѶ�������趨Ϊ2400
*           3.DS18B20����������DQ�뵥Ƭ��P2.7��������
* ���ڰ汾��2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "onewire.h"  //�����ߺ�����
#include <stdio.h>

#define BAUDRATE     2400  //������
#define SYSTEMCLOCK  12000000  //ϵͳʱ��Ƶ��

unsigned char intr;
bit temper_flag = 0;//�¶ȶ�ȡ��־

void uart_sendstring(unsigned char *str);

//������
void main(void)
{ 
    float temperature;
	unsigned char str[40];

  	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 256 - (unsigned int)(SYSTEMCLOCK/BAUDRATE/384+0.5); 
// 	ES = 1;  //ʱ�ܴ����ж�
    
    TMOD |= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��
	TR1 = 1;  //������ʱ��1
	
    while(1)
    {
		sprintf(str,"%s%6.3f%c%c","temperature:",temperature,'\r','\n');
		
        if(temper_flag)
		{
			temper_flag = 0;
			temperature = rd_temperature_f();  //���¶�         
			uart_sendstring(str);
        }

    }
}

//��ʱ��0�жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������  
	
	if(++intr == 250)  //2msִ��һ��
	{
        intr = 0;
		temper_flag = 1;  //500ms�¶ȶ�ȡ��־λ��1
    }
}

//ͨ�����ڷ����ַ���
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);  //�ȴ����ͱ�־λ��λ
		TI = 0;
        p++;
    }
}