/*******************************************************************************  
* 文件名称：DS18B20温度传感器实验-小数点处理
* 实验目的：1.掌握常用的字符串处理函数
*           2.掌握DS18B20温度传感器的操作方法
* 程序说明：1.通过串口输出实时温度数据，使用串口调试工具调试程序
*           2.串口通讯波特率设定为2400
*           3.DS18B20数据线引脚DQ与单片机P2.7引脚连接
* 日期版本：2012-9-6/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "onewire.h"  //单总线函数库
#include <stdio.h>

#define BAUDRATE     2400  //波特率
#define SYSTEMCLOCK  12000000  //系统时钟频率

unsigned char intr;
bit temper_flag = 0;//温度读取标志

void uart_sendstring(unsigned char *str);

//主函数
void main(void)
{ 
    float temperature;
	unsigned char str[40];

  	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 256 - (unsigned int)(SYSTEMCLOCK/BAUDRATE/384+0.5); 
// 	ES = 1;  //时能串口中断
    
    TMOD |= 0x01;  //配置定时器工作模式
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //打开定时器中断
    TR0 = 1;  //启动定时器
	TR1 = 1;  //启动定时器1
	
    while(1)
    {
		sprintf(str,"%s%6.3f%c%c","temperature:",temperature,'\r','\n');
		
        if(temper_flag)
		{
			temper_flag = 0;
			temperature = rd_temperature_f();  //读温度         
			uart_sendstring(str);
        }

    }
}

//定时器0中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载  
	
	if(++intr == 250)  //2ms执行一次
	{
        intr = 0;
		temper_flag = 1;  //500ms温度读取标志位置1
    }
}

//通过串口发送字符串
void uart_sendstring(unsigned char *str)
{
    unsigned char *p;
    
    p = str;
    while(*p != '\0')
    {
        SBUF = *p;
		while(TI == 0);  //等待发送标志位置位
		TI = 0;
        p++;
    }
}