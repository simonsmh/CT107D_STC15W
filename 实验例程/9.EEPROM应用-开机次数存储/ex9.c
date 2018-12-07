/*******************************************************************************  
* 文件名称：开机次数存储实验,按下复位按键，或重新上电，数码管显示+1
* 实验目的：1.掌握I2C总线通讯基本特点和工作时序
*           2.掌握51单片机模拟I2C总线时序的程序设计方法
*           3.掌握EEPROM存储器的特性及AT24C02的读写操作方法
* 程序说明：使用程序时，需要在Preprocessor Symbols 中加入宏定义：EEPROM_AT24C02
* 日期版本：2012-9-4/V1.0a 
*******************************************************************************/
#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "i2c.h"    //I2C总线驱动库 
#include "absacc.h"
/*
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char dspbuf[8] = {10,10,10,10,10,0,0,0};  //显示缓冲区
unsigned char dspcom = 0;

void display(void);

//主函数
void main(void)
{ 
    unsigned char reset_cnt;  //开机次数存储 (最大存储值255)
    
    
    TMOD |= 0x01;  //配置定时器工作模式
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //打开定时器中断
    TR0 = 1;  //启动定时器

// 	write_eeprom(0x00,0x00);  //EEPROM中存储的数据需要进行初始化
 	
    reset_cnt = read_eeprom(0x00);  //从AT24C02地址0x00中读取数据
    reset_cnt++;
    write_eeprom(0x00,reset_cnt);  //向AT24C02地址0x00中写入数据

    //更新显示数据
    dspbuf[5] = reset_cnt/100;       
    dspbuf[6] = reset_cnt%100/10;       
    dspbuf[7] = reset_cnt%10;       
    XBYTE[0xa000] = 0x00;
    while(1)
    {
        ;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载 
     
    display();
}

//显示函数
void display(void)
{   
	XBYTE[0xE000] = 0xff;  //消隐
	
	XBYTE[0xC000] = (1<<dspcom);
    XBYTE[0xE000] = tab[dspbuf[dspcom]]; //段码
   
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