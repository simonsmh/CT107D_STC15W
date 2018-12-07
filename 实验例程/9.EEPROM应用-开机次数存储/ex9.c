/*******************************************************************************  
* �ļ����ƣ����������洢ʵ��,���¸�λ�������������ϵ磬�������ʾ+1
* ʵ��Ŀ�ģ�1.����I2C����ͨѶ�����ص�͹���ʱ��
*           2.����51��Ƭ��ģ��I2C����ʱ��ĳ�����Ʒ���
*           3.����EEPROM�洢�������Լ�AT24C02�Ķ�д��������
* ����˵����ʹ�ó���ʱ����Ҫ��Preprocessor Symbols �м���궨�壺EEPROM_AT24C02
* ���ڰ汾��2012-9-4/V1.0a 
*******************************************************************************/
#include "reg52.h"  //����51��Ƭ�����⹦�ܼĴ���
#include "i2c.h"    //I2C���������� 
#include "absacc.h"
/*
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char dspbuf[8] = {10,10,10,10,10,0,0,0};  //��ʾ������
unsigned char dspcom = 0;

void display(void);

//������
void main(void)
{ 
    unsigned char reset_cnt;  //���������洢 (���洢ֵ255)
    
    
    TMOD |= 0x01;  //���ö�ʱ������ģʽ
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //�򿪶�ʱ���ж�
    TR0 = 1;  //������ʱ��

// 	write_eeprom(0x00,0x00);  //EEPROM�д洢��������Ҫ���г�ʼ��
 	
    reset_cnt = read_eeprom(0x00);  //��AT24C02��ַ0x00�ж�ȡ����
    reset_cnt++;
    write_eeprom(0x00,reset_cnt);  //��AT24C02��ַ0x00��д������

    //������ʾ����
    dspbuf[5] = reset_cnt/100;       
    dspbuf[6] = reset_cnt%100/10;       
    dspbuf[7] = reset_cnt%10;       
    XBYTE[0xa000] = 0x00;
    while(1)
    {
        ;
    }
}

//��ʱ���жϷ�����
void isr_timer_0(void)  interrupt 1  //Ĭ���ж����ȼ� 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //��ʱ������ 
     
    display();
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
} */

#include <reg52.h>
#include <absacc.h>
#include <i2c.h>

sbit buzz = P0^6;

extern void EEPROM_Write(unsigned char addr,unsigned char dat);
extern unsigned char EEPROM_Read(unsigned char addr);

void main(){
	unsigned char cnt = 0xF0;
	
	P2 = P2&0x1F|0xA0;
	buzz = 0;

	cnt = EEPROM_Read(0x00);
	cnt++;
	EEPROM_Write(0x00,cnt);
	P2 = P2&0x1F|0x80;
	P0 = EEPROM_Read(0x00);
	while(1);
}