/*******************************************************************************  
* 文件名称：超声波测距实验
* 实验目的：1.掌握超声波测距原理
*           2.掌握51单片机定时器工作模式和配置方法
* 程序说明：无
* 日期版本：2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "intrins.h"
#include "absacc.h"

#define somenop {_nop_();_nop_();_nop_();_nop_();_nop_();\
                 _nop_();_nop_();_nop_();_nop_(); _nop_();}
sbit TX = P1^0;  //发射引脚
sbit RX = P1^1;  //接收引脚

code unsigned tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90
	};
unsigned char dspbuf[8] = {10,10,10,10,10,10,10,10};  //显示缓冲区
unsigned char dspcom = 0;

unsigned int intr = 0;
bit s_flag;
unsigned int t = 0;

void ConfigTimer0(unsigned int ms);
void display(void);
void send_wave(void);

void main(void)
{
    unsigned int distance;

/*	P2 = (P2&0x1f)|0xa0;
	P0 &= 0xbf;
	P2 &= 0x1f;
*/
    TMOD |= 0x11;  //配置定时器工作模式
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    TH1 = 0;
    TL1 = 0;  
  
    EA = 1;
    ET0 = 1;  //打开定时器0中断
	ET1 = 1;
    TR0 = 1;  //启动定时器   

    while(1)
    {
        /** 200毫秒更新一次数据 */
		if(s_flag)
        {
            s_flag = 0;
            /** 关闭定时器0中断:计算超声波发送到返回的时间 */
			//ET0 = 0;
            send_wave();  //发送方波信号
            TR1 = 1;  //启动计时
			while((RX == 1) && (TF1 == 0));  //等待收到脉冲
			TR1 = 0;  //关闭计时

			//发生溢出
			if(TF1 == 1)
			{
				TF1 = 0;
				distance = 9999;  //无返回
			}
			else
			{
				/**  计算时间  */
				t = TH1;
				t <<= 8;
				t |= TL1;
				distance = (unsigned int)(t*0.017);  //计算距离				
			}
			TH1 = 0;
			TL1 = 0;
        }
		/** 数据处理                      */
                     
		dspbuf[5] = distance/100;
		dspbuf[6] = distance%100/10;
		dspbuf[7] = distance%10;       
    }
}

//定时器0中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载 
     
	display();  //2ms执行一次

	if(++intr == 200)
	{
        s_flag = 1;
        intr = 0;
    }
}


//显示函数  
void display(void)
{
 	XBYTE[0xE000] = 0xff;  //去除鬼影

 	XBYTE[0xC000] = (1<<dspcom);
 	XBYTE[0xE000] = tab[dspbuf[dspcom]]; 
	
	if(++dspcom == 8){
 		dspcom = 0;
	}	
}


//TX引脚发送40KHz方波信号驱动超声波发送探头
void send_wave(void)
{
	unsigned char i = 8;  //发送8个脉冲
	
	do
	{
		TX = 1;
		somenop;
		TX = 0;
		somenop;
	}
	while(i--);
}
