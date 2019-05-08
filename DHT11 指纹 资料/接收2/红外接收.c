#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit led1=P1^0;
sbit led2=P1^1;
sbit led3=P1^2;
sbit in=P3^2;
uchar i,a,num;
bit fleg;
void init()
{
	fleg=1;
	in=1;
	EA=1;
	EX0=1;
	IT0=1;
}
void delayms(uchar aa)
{
	for(i=aa;i>0;i--)
	{
		_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();
	}	
}

void main()
{
	init();
	//delayms(45);//0.642ms
	//delayms(35);//0.502ms
	//delayms(115);//1.623ms
	//delayms(72);//1.02ms
	//delayms(84);//1.188ms
	//delayms(31);//0.446ms
	while(1);
}
void sieasdf() interrupt 0
{
	EX0=0;
	for(a=5;a>0;a--)
	{
		delayms(35);//延时0.5ms 判断5次  5*0.5＝2.5ms
		if(in)fleg=0;
	}
	if(fleg)
	{
		delayms(72);//延时1ms  判断是不是高电平了
		if(in)
		{
			delayms(115);//延时让它超过2ms; 2.5+1+1.623=5.123ms  开始读数据
			delayms(118);//若偏移一位，可以去掉。
			for(a=8;a>0;a--)
			{
				while(!in);
				delayms(86);//延时1.188ms 判断IO高低，从而得0或1
				num=num>>1;
				if(in)
					{
						num=num|0x80;
						delayms(31);//延时0.6ms 因为上面延时1.2ms+0.6 刚好跳过1.5ms
					}
			}	
			P2=num;
		
		}
	}
	fleg=1;
	EX0=1;
}
