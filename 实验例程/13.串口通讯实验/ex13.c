/*******************************************************************************  
* 文件名称：串口通讯实验
* 实验目的：1.掌握51单片机串口工作模式及相关寄存器配置方法
*           2.了解51单片机波特率的计算方法
* 程序说明：1.通过USB连接线连接PC机，在计算机硬件管理器中查看串口号
*           2.打开串口调试工具软件，并将串口通讯波特率设定为2400
* 日期版本：2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器

#define BAUDRATE     2400  //波特率
#define SYSTEMCLOCK  12000000  //系统时钟频率

void uart_sendstring(unsigned char *str);

//主函数
void main(void)
{ 

  	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 256 - (unsigned int)(SYSTEMCLOCK/BAUDRATE/384+0.5); 
	ES = 1;  //时能串口中断
	EA = 1;
	TR1 = 1;  //启动定时器1

    uart_sendstring("hello,world.\r\n");
   
    while(1); 
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