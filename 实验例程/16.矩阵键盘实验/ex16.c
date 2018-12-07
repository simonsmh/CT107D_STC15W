/*******************************************************************************  
* 文件名称：矩阵键盘实验
* 实验目的：1.掌握矩阵键盘扫描的基本原理
*           2.掌握按键扫描及消除抖动的基本原理
* 程序说明：1.使用程序前，将跳线J2调整为KBD模式
*           2.按键功能可参考按键处理函数:void key_proc(unsigned char key);
* 日期版本：2012-9-4/V1.0a 
*******************************************************************************/

#include "reg52.h"  //定义51单片机特殊功能寄存器

bit key_re;
unsigned char key_press;
unsigned char key_value;

bit key_flag = 0;
unsigned char intr = 0;

unsigned char read_keyboard(void);
void key_proc(unsigned char key);

//主函数
void main(void)
{ 
    unsigned char key_temp = 0xff;

    TMOD &= 0xf0;    
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
            key_temp = read_keyboard();
        } 
        
        key_proc(key_temp);        
        key_temp = 0xff;  //清除旧键值
    }
}

//定时器中断服务函数
void isr_timer_0(void)  interrupt 1  //默认中断优先级 1
{
    TH0 = (65536-2000)/256;
    TL0 = (65536-2000)%256;  //定时器重载 
     
	if(++intr == 10)  //2ms执行一次
	{
        intr = 0;
		key_flag = 1;  //20ms按键扫描标志位置1
    }
}

//读取矩阵键盘键值
unsigned char read_keyboard(void)
{
    unsigned char key_temp;
    static unsigned char col;
      
    key_temp = (P3&0xf0);

    if(key_temp != 0xf0) //有按键按下
        key_press++;
	else
		key_press = 0;  //抖动
    
    if(key_press == 3)
    {
		key_press = 0;
		key_re = 1;
		switch(key_temp)
        {
            case 0x70:
                col = 1;  //第一列按键按下
                break;                
            case 0xb0:
                col = 2;  //第二列按键按下
                break;    
            case 0xd0:
                col = 3;  //第三列按键按下
                break;    
            case 0xe0:
                col = 4;  //第四列按键按下
                break;
        }
        
        key_temp = (P3&0x0f);
        
		switch(key_temp)
        {
            case 0x0e:
                key_value = (col-1);   //第一行按键按下
                break;                
            case 0x0d:
                key_value = (3+col);   //第二行按键按下
                break;    
            case 0x0b:
                key_value = (7+col);   //第三行按键按下
                break;    
            case 0x07:
                key_value = (11+col);  //第四行按键按下
                break;
        }
    }
    //连续三次检测到按键被按下，并且该按键已经释放

	key_temp = (P3&0x0f);
	
    if((key_re == 1) && (key_temp == 0x0f))
    {
        key_re = 0;
        return key_value;
    }
    else
     {return 0xff;}  //无按键按下或被按下的按键未被释放 
}


//按键处理函数:S7,S11
void key_proc(unsigned char key)
{
    switch(key)
    {
        case 0:  //
            P2 = ((P2&0x1f)|0x80);
			P0++;
			P2 &= 0x1f;
            break;
        case 1:  //
            P2 = ((P2&0x1f)|0x80);
			P0--;
			P2 &= 0x1f;
            break;
    }    
}