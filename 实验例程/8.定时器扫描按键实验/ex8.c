/*******************************************************************************  
* 文件名称：定时器扫描按键实验
* 实验目的：1.掌握通过定时器扫描按键键值的基本方法
*           2.掌握51单片机定时器工作模式和配置方法
* 程序说明：使用程序时，将跳线J2调整为BTN模式
* 日期版本：2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器
#include "absacc.h"

bit key_re;
unsigned char key_press;
unsigned char key_value;
bit key_flag;
unsigned char intr;
                           //  0    1    2    3    4    5    6    7    8    9
code unsigned char tab[] = { 0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
                             0xff};
unsigned char dspbuf[8] = {10,10,10,10,10,0,0,0};  //显示缓冲区
unsigned char dspcom = 0;

unsigned char read_key(void);
void display(void);

//主函数
void main(void)
{ 
    unsigned char key_temp;
	unsigned char num;
    
    P3 = 0xff;  //按键接口初始化
    
    TMOD |= 0x01;  //配置定时器工作模式
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  
    EA = 1;
    ET0 = 1;  //打开定时器中断
    TR0 = 1;  //启动定时器
    
    while(1)
    {
        if(key_flag)
        {
            key_flag = 0;
            key_temp = read_key();
        }        
        //进入按键处理程序部分
        switch(key_temp)
        {
            case 1:
				num++;
                break;
            case 2:
				num--;
                break;      
        }
		key_temp = 0;  //完成按键处理程序后，清除键值
		
		dspbuf[5] = num/100;
		dspbuf[6] = num%100/10;
		dspbuf[7] = num%10;
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载 
    display(); 
	if(++intr == 10)  //2ms执行一次
	{
        intr = 0;
		key_flag = 1;  //20ms按键扫描标志位置1
    }
}

//读取键值
unsigned char read_key(void)
{
	
    unsigned char key_temp;
    
	key_temp = (P3&0x0f);
	
    if(key_temp != 0x0f) //有按键按下
        key_press++;
	else
		key_press = 0;  //抖动
    
    if(key_press == 5)
    {
		key_press = 0;
		key_re = 1;
		switch(key_temp)
        {
            case 0x0e:
                key_value = 1;  //
                break;                
            case 0x0d:
                key_value = 2;  //
                break;    
            case 0x0b:
                key_value = 3;  //
                break;    
            case 0x07:
                key_value = 4;  //
                break;
        }
    }
    //连续三次检测到按键被按下，并且该按键已经释放
    if((key_re == 1) && (key_temp == 0x0f))
    {
        key_re = 0;
        return key_value;
    }
    
    return 0xff;  //无按键按下或被按下的按键未被释放    
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
}
