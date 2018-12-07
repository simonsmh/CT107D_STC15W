/*******************************************************************************  
* �ļ����ƣ�����ͨѶʵ��
* ʵ��Ŀ�ģ�1.����51��Ƭ�����ڹ���ģʽ����ؼĴ������÷���
*           2.�˽�51��Ƭ�������ʵļ��㷽��
*           3.���յ�Ƭ�����ڽ����жϷ���������Ʒ���
* ����˵����1.ͨ��USB����������PC�����ڼ����Ӳ���������в鿴���ں�
*           2.�򿪴��ڵ��Թ����������������ͨѶ�������趨Ϊ2400
*           3.ͨ�����ڵ��Թ�����������:(x���������ַ����ַ������Ȳ�Ҫ����20)
*                1+xxxxxxx+�س�:�򿪷�����
*                2+xxxxxxx+�س�:�رշ�����
*                3+xxxxxxx+�س�:�򿪼̵���
*                4+xxxxxxx+�س�:�رռ̵���
* ���ڰ汾��2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���

#define BAUDRATE     2400  //������
#define SYSTEMCLOCK  12000000  //ϵͳʱ��Ƶ��

sbit RELAY = P0^6;
sbit BUZZ = P0^4;

unsigned char rxbuf[20];
unsigned char rxcnt;
bit rx_over = 0;  //���������ɱ�־λ

void proc_cmd(void);

//������
void main(void)
{ 

  	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 256 - (unsigned int)(SYSTEMCLOCK/BAUDRATE/384+0.5); 
	ES = 1;  //ʱ�ܴ����ж�
	EA = 1;
	TR1 = 1;  //������ʱ��1
   
    while(1)
    {
        if(rx_over)
        {
            rx_over = 0;
            proc_cmd();
            ES = 1;  //�������ɺ󣬴򿪴���
        }
    }
}

//�����жϷ�����
void isr_uart(void) interrupt 4
{
    if(RI)
    {
        RI = 0;  //������ձ�־λ 
        rxbuf[rxcnt] = SBUF;
        if(rxbuf[rxcnt] == '\n')
        {
            rxcnt = 0;
            rx_over = 1;
            ES = 0;  //�س�Ϊ���ս�����־����⵽�س����󣬹رմ����ж�
        }      
		else
		{
			rxcnt++;
		}
    }
}

//���ڽ����������
void proc_cmd(void)
{
    switch(rxbuf[0])
    {
        case '1':
            P2 = ((P2&0x1f)|0xa0);
            P0 = 0x40;
            P2 = ((P2&0x1f)|0xa0);
            break;
        case '2':
            P2 = ((P2&0x1f)|0xa0);
            P0 = 0x00;
            P2 = ((P2&0x1f)|0xa0);
            break;
        case '3':
            P2 = ((P2&0x1f)|0xa0);
            P0 = 0x10;
            P2 = ((P2&0x1f)|0xa0);
            break;
        case '4':
            P2 = ((P2&0x1f)|0xa0);
            P0 = 0x00;
            P2 = ((P2&0x1f)|0xa0);
            break;
    }
}