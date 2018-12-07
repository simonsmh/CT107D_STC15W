/*******************************************************************************  
* 文件名称：ADC转换实验
* 实验目的：1.掌握I2C总线通讯基本特点和工作时序
*           2.掌握51单片机模拟I2C总线时序的程序设计方法
*           3.掌握PCF8591 ADC芯片的操作方法
* 程序说明：1.使用程序前在 Preprocessor Symbols 中增加宏定义ADC_PCF8591
*           2.调节电位器Rb2，观察数码管显示情况
* 日期版本：2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "i2c.h"    //I2C总线驱动库 
//#include "absacc.h"

code unsigned char LedChar[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff
                           };
unsigned char LedBuf[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};  //显示缓冲区
unsigned char dspcom = 0;
unsigned char intr;
bit adc_flag;

void Count(unsigned char val);
void LedScan();

//主函数
void main(void)
{ 
    unsigned char val;  //ADC转换数据 
		
    TMOD |= 0x01;  //配置定时器工作模式
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //打开定时器中断
    TR0 = 1;  //启动定时器

	init_pcf8591();  //PCF8591初始化      
    
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

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载 
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
	LedBuf[0] &= 0x7f;       //加小数点
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