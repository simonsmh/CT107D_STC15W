/*******************************************************************************  
* 文件名称：外部中断实验
* 实验目的：1.掌握51单片机外部中断相关寄存器的配置方法和触发方式
*           2.掌握keil uVision 环境下外部中断服务函数的设计方法
* 程序说明：1.使用程序前，将跳线J2调整为BTN模式
*           2.按下P3.2-INT0引脚对应的按键S5，LED1状态反转
*           3.按下P3.3-INT1引脚对应的按键S4, LED2状态反转
* 日期版本：2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器

sbit LED1 = P0^0;
sbit LED2 = P0^1;

void main(void)
{
    EX0 = 1;
    IT0 = 1;  //边沿触发方式(下降沿)

    EX1 = 1;
    IT1 = 1;  //边沿触发方式(下降沿)
        
    EA = 1;  //开启总中断
    while(1);
}

//外部中断0中断服务函数
void isr_intr_0(void) interrupt 0
{
	P2 = ((P2&0x1f)|0x80);
    LED1 = ~LED1;    
	P2 = P2&0x1f;
}
                        
//外部中断1中断服务函数
void isr_intr_1(void) interrupt 2
{
	P2 = ((P2&0x1f)|0x80);
    LED2 = ~LED2;    
	P2 = P2&0x1f;
}