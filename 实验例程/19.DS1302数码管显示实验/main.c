#include <reg52.h>

bit flag200ms = 0;
unsigned char T0RH = 0;
unsigned char T0RL = 0;
unsigned char LedChar[] = {
	0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90
};
unsigned char LedBuf[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
struct Time 
{
	unsigned char hour;
	unsigned char mint;
	unsigned char sec;
};
struct Time Time1 = {
	 0x06,0x59,0x00
}; 
//unsigned char Time1[] = {0x00, 0x59, 0x06};

extern void Ds1302_Single_Byte_Write(unsigned char addr, unsigned char dat);
extern unsigned char Ds1302_Single_Byte_Read(unsigned char addr);
void ConfigTimer0(unsigned int ms);
void Display_Refresh();
void LedScan();

void main()
{

	EA = 1;
	ConfigTimer0(1);

	Ds1302_Single_Byte_Write(0x8e,0x00);
	Ds1302_Single_Byte_Write(0x80,0x00);
	Ds1302_Single_Byte_Write(0x82,0x59);
	Ds1302_Single_Byte_Write(0x84,0x06);

	while(1)
	{
		if(flag200ms)
		{
			flag200ms = 0;
			Time1.hour = Ds1302_Single_Byte_Read(0x85);
			Time1.mint = Ds1302_Single_Byte_Read(0x83);
			Time1.sec = Ds1302_Single_Byte_Read(0x81);
		}
		Display_Refresh();
	}
}

void ConfigTimer0(unsigned int ms)
{
	T0RH = (65536 - (11059200/12/1000*ms))/256;
	T0RL = (65536 - (11059200/12/1000*ms))%256;
	TMOD &= 0xf0;
	TMOD |= 0x01;
	TH0 = T0RH;
	TL0 = T0RL;
	ET0 = 1;
	TR0 = 1;
}

void Display_Refresh()
{

	LedBuf[0] = LedChar[(Time1.sec&0x7f)%16];
	LedBuf[1] = LedChar[(Time1.sec&0x7f)/16];
	LedBuf[2] = 0xbf;
	LedBuf[3] = LedChar[(Time1.mint)%16];
	LedBuf[4] = LedChar[(Time1.mint)/16];
	LedBuf[5] = 0xbf;
	LedBuf[6] = LedChar[(Time1.hour)%16];
	LedBuf[7] = LedChar[(Time1.hour)/16]; 
			
}

void LedScan()
{
	static unsigned char i = 0;
	
	P2 = (P2&0x1f)|0xc0;
	P0 = 0x80 >> i;
	P2 &= 0x1f;
	P2 = (P2&0x1f)|0xe0;
	P0 = LedBuf[i];
	P2 &= 0x1f;
	
	if(i < 8)
		i++;
	else 
		i = 0;		 
}

void InterruptTimer0() interrupt 1
{
	static unsigned char tr1s = 0;

	TH0 = T0RH;
	TL0 = T0RL;
	LedScan();

	if(tr1s <= 200)
		tr1s++;
	else
	{
		tr1s = 0;
		flag200ms = 1;
	}
}	