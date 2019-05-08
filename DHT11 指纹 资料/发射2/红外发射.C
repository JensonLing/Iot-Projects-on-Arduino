#include<reg51.h>
#include<intrins.h>
#define uchar unsigned char
#define uint unsigned int
sbit key1=P3^3;
sbit key2=P3^4;
sbit key3=P3^5;
sbit LED=P1^0; //发射指示灯
sbit out=P3^7;
uchar i,a,num1;
void init()//初始化作用
{
	key1=1;
	key2=1;
	key3=1;
}
void delay(uchar aa)
{
	uchar bb,cc;
	for(bb=aa;bb>0;bb--)
		for(cc=200;cc>0;cc--);
}
void delayms(uchar aa)//延时程序
{
	for(a=aa;a>0;a--)
	{
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
		_nop_();
	}
}

void khz(uchar aa)//是发射38KHZ的程序
{
	for(a=aa;a>0;a--)   //这个for语句可以得到准确的26.3波特率
		{
			out=0;
			i=7;		//低了17us
			while(i>0)i--;		//   38kHZ  
			out=1;
				//高了9us      17+9=26us   比26.3快一点点
		}

}
	//khz(116);//3.028ms   精确的时间
	//khz(64);//2.006ms
	//khz(40); //1.052ms
	//delayms(125);//2.012ms      这里是一些时间的介绍
	//delayms(65);//1.054ms
	//delayms(93);//1.5ms
void fashu(uchar num)
{
	khz(116);//发射3ms 38khz
	delayms(125);
	for(num1=8;num1>0;num1--) //原来用的是a  后来出错，肯定在这里！
	{
		khz(40);
		if(num&0x01)
			delayms(93);//delay 1.5ms
		else
			delayms(65);//delay 1ms
		num=num>>1;
	}
	khz(20);

}
void tishi()
{
	  	LED=0;
		delay(50);
		LED=1;
		delay(50);
		LED=0;
		delay(50);
		LED=1;
}
void keyscan()//按键扫描
{
	if(key1==0)
	{
		_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();
		if(key1==0)
		{
			while(!key1);
			fashu(0xf3);
			tishi();
		}

	}
	if(key2==0)
	{
		_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();
		if(key2==0)
		{
			while(!key2);
			fashu(0x3f);
			tishi();
			
		}

	}
	if(key3==0)
	{
		_nop_();_nop_();_nop_();_nop_();
		_nop_();_nop_();_nop_();_nop_();
		if(key3==0)
		{
			while(!key3);
			fashu(0xf5);
			tishi();
		}

	}	
}
void main()
{
	init();
	while(1)
	{
		keyscan();
	}
}
