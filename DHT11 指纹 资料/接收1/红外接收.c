#include<reg52.h>     //����ͷ�ļ���һ���������Ҫ�Ķ���ͷ�ļ��������⹦�ܼĴ����Ķ���   
sbit LED=P0^1;  
sbit IR=P2^3;         //����һ�廯���� 
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
				delayus(6);//���������   
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
			}//���ϰ�����𡢷�������           
		} 
}   