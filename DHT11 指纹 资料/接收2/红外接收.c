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
		delayms(35);//��ʱ0.5ms �ж�5��  5*0.5��2.5ms
		if(in)fleg=0;
	}
	if(fleg)
	{
		delayms(72);//��ʱ1ms  �ж��ǲ��Ǹߵ�ƽ��
		if(in)
		{
			delayms(115);//��ʱ��������2ms; 2.5+1+1.623=5.123ms  ��ʼ������
			delayms(118);//��ƫ��һλ������ȥ����
			for(a=8;a>0;a--)
			{
				while(!in);
				delayms(86);//��ʱ1.188ms �ж�IO�ߵͣ��Ӷ���0��1
				num=num>>1;
				if(in)
					{
						num=num|0x80;
						delayms(31);//��ʱ0.6ms ��Ϊ������ʱ1.2ms+0.6 �պ�����1.5ms
					}
			}	
			P2=num;
		
		}
	}
	fleg=1;
	EX0=1;
}
