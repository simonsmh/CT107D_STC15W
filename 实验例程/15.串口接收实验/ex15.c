/*******************************************************************************  
* 文件名称：串口通讯实验
* 实验目的：1.掌握51单片机串口工作模式及相关寄存器配置方法
*           2.了解51单片机波特率的计算方法
*           3.掌握单片机串口接收中断服务函数的设计方法
* 程序说明：1.通过USB连接线连接PC机，在计算机硬件管理器中查看串口号
*           2.打开串口调试工具软件，并将串口通讯波特率设定为2400
*           3.通过串口调试工具输入命令:(x代表任意字符，字符串长度不要超过20)
*                1+xxxxxxx+回车:打开蜂鸣器
*                2+xxxxxxx+回车:关闭蜂鸣器
*                3+xxxxxxx+回车:打开继电器
*                4+xxxxxxx+回车:关闭继电器
* 日期版本：2012-9-5/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器

#define BAUDRATE     2400  //波特率
#define SYSTEMCLOCK  12000000  //系统时钟频率

sbit RELAY = P0^6;
sbit BUZZ = P0^4;

unsigned char rxbuf[20];
unsigned char rxcnt;
bit rx_over = 0;  //命令接收完成标志位

void proc_cmd(void);

//主函数
void main(void)
{ 

  	SCON = 0x50;
	TMOD |= 0x20;
	TH1 = 256 - (unsigned int)(SYSTEMCLOCK/BAUDRATE/384+0.5); 
	ES = 1;  //时能串口中断
	EA = 1;
	TR1 = 1;  //启动定时器1
   
    while(1)
    {
        if(rx_over)
        {
            rx_over = 0;
            proc_cmd();
            ES = 1;  //命令处理完成后，打开串口
        }
    }
}

//串口中断服务函数
void isr_uart(void) interrupt 4
{
    if(RI)
    {
        RI = 0;  //清除接收标志位 
        rxbuf[rxcnt] = SBUF;
        if(rxbuf[rxcnt] == '\n')
        {
            rxcnt = 0;
            rx_over = 1;
            ES = 0;  //回车为接收结束标志，检测到回车符后，关闭串口中断
        }      
		else
		{
			rxcnt++;
		}
    }
}

//串口接收命令处理函数
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