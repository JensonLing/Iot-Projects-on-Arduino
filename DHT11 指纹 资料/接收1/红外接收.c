#include<reg52.h>     //包含头文件，一般情况不需要改动，头文件包含特殊功能寄存器的定义   
sbit LED=P0^1;  
sbit IR=P2^3;         //红外一体化接收 
sbit LED1=P0^2; 
sbit BUZZER=P2^5;     
void delayus(unsigned us) 
{ 
	unsigned int i,j;   
	for(i=0;i<us;i++)   
		for(j=0;j<101;j++); 
}  
void main()
{
	unsigned int a,b; 
	LED1=0;  
	BUZZER=0;    
	while(1)  
		{    
   
			b=0;    
			for(a=0;a<22;a++)    
			{ 
				if(IR==0)   
				b=b+1;    
				delayus(6);//对脉冲采样   
			}  
			if(b>5)  
			{
				LED=0;  
				BUZZER=0;
			}
			else   
			{
				LED=1;    
				BUZZER=1;
			}//有障碍物灯灭、蜂鸣器响           
		} 
}   