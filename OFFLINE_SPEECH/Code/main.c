#include "stc8.h"
#include "UART.H"

//����ԭʼ����																 len			start     12345					 x
//����   75 41 72 54 63 50 00 00 03 E9 5B 78 00 0A A1 4A 77 61 6B 65 75 70 5F 75 6E 69 
//����   75 41 72 54 63 50 49 00 03 E9 C7 1B 00 07 70 E7 6F 70 65 6E 6C 65 64 
//�ص�   75 41 72 54 63 50 50 00 03 E9 E4 7A 00 08 81 08 63 6C 6F 73 65 6C 65 64
//��һ�� 75 41 72 54 63 50 5D 00 03 E9 9F 29 00 0E E1 CE 7A 65 6E 67 6A 69 61 6C 69 61 6E 67 64 75
//��һ�� 75 41 72 54 63 50 72 00 03 E9 B0 6A 00 0F F1 EF 6A 69 61 6E 78 69 61 6F 6C 69 61 6E 67 64 75 
//������ 75 41 72 54 63 50 CC 00 03 E9 15 D8 00 0C C1 8C 64 61 6B 61 69 6B 61 69 67 75 61 6E
//�ؿ��� 75 41 72 54 63 50 D2 00 03 E9 13 EC 00 0D D1 AD 67 75 61 6E 62 69 6B 61 69 67 75 61 6E
//���յ� 75 41 72 54 63 50 77 00 03 E9 40 1D 00 0C C1 8C 6F 70 65 6E 6B 6F 6E 67 74 69 61 6F
//�ؿյ� 75 41 72 54 63 50 7C 00 03 E9 80 7E 00 0D D1 AD 63 6C 6F 73 65 6B 6F 6E 67 74 69 61 6F
//���¶� 75 41 72 54 63 50 89 00 03 E9 66 F4 00 0D D1 AD 73 68 65 6E 67 67 61 6F 77 65 6E 64 75
//���¶� 75 41 72 54 63 50 8E 00 03 E9 2F 9F 00 0B B1 6B 6A 69 61 6E 64 69 77 65 6E 64 75
//�ػ�1H 75 41 72 54 63 50 BC 00 03 E9 6C 1A 00 0E E1 CE 73 65 74 74 69 43 6F 6E 65 68 6F 6E 6F 72
//����   75 41 72 54 63 50 DF 00 03 E9 1B 16 00 07 70 E7 65 78 69 74 55 6E 69


sbit hlink_sw=P3^2;
sbit hlink_lt=P3^3;
sbit hlink_kt=P3^4;
sbit led_r=P3^5;
sbit led_1=P5^4;
sbit led_2=P1^0;
sbit hsr1=P1^7;		//����
sbit hsr2=P1^6;		//����
sbit IR=P5^5;			//���죿
sbit pwr_on=P1^1;

bit power=0;
bit light=0;
bit light2=0;

void delay(unsigned int x)
{
	while(x--);
}

void delayms(unsigned int x)
{
	unsigned int y;
	for(x;x>0;x--)
		for(y=1100;y>0;y--);
}

void main()
{
	IR=0;
	led_1=0;
	led_2=0;
	hsr1=0;
	hsr2=0;
	pwr_on=1;
	P1M0=0XC0;
	
	UartInit();
	UartSendStr("system init...\r\n");
	while(1)
	{
		led_r=0;
		if(new_flag)		//�µĴ������ݸ��±�־
		{
			//UartSendStr("received new msg...\r\n");
	
			UartSend(uart_buf[2]);		//������
			UartSend(uart_buf[3]);
			UartSend(uart_buf[4]);
			UartSend(uart_buf[5]);
			UartSend(uart_buf[6]);
			
			//ĿǰBUG:1.�رտյ��͹رյƹ���ͬһ���ظ��ʡ�����취���жϴʸ�����5�����ӵ�6��
			
			
			led_r=1;
			delayms(200);
			new_flag=0;
			if(uart_buf[2]==0x77 && uart_buf[3]==0x61 && uart_buf[4]==0x6B && uart_buf[5]==0x65 && uart_buf[6]==0x75)
			{
				//����ִ��
			}
			else if(uart_buf[2]==0x6F && uart_buf[3]==0x70 && uart_buf[4]==0x65 && uart_buf[5]==0x6E && uart_buf[6]==0x6C)
			{
				//����
				power=1;light=1;light2=0;IR=0;
			}		
			else if(uart_buf[2]==0x63 && uart_buf[3]==0x6C && uart_buf[4]==0x6F && uart_buf[5]==0x73 && uart_buf[6]==0x65)
			{
				//�ص�
				power=1;light=0;light2=0;IR=1;
			}	
			else if(uart_buf[2]==0x7A && uart_buf[3]==0x65 && uart_buf[4]==0x6E && uart_buf[5]==0x67 && uart_buf[6]==0x6A)
			{
				//��һ��
				power=1;light=1;light2=1;
			}	
			else if(uart_buf[2]==0x6A && uart_buf[3]==0x69 && uart_buf[4]==0x61 && uart_buf[5]==0x6E && uart_buf[6]==0x78)
			{
				//��һ��
				power=1;light=1;light2=0;
			}	
			else if(uart_buf[2]==0x64 && uart_buf[3]==0x61 && uart_buf[4]==0x6B && uart_buf[5]==0x61 && uart_buf[6]==0x69)
			{
				//������
				power=1;light=0;light2=0;
			}	
			else if(uart_buf[2]==0x67 && uart_buf[3]==0x75 && uart_buf[4]==0x61 && uart_buf[5]==0x6E && uart_buf[6]==0x62)
			{
				//�ؿ���
				power=0;light=0;light2=0;
			}	
			else if(uart_buf[2]==0x6F && uart_buf[3]==0x70 && uart_buf[4]==0x65 && uart_buf[5]==0x6E && uart_buf[6]==0x6B)
			{
				//���յ�6F 70 65 6E 6B 
			}				
			else if(uart_buf[2]==0x63 && uart_buf[3]==0x6C && uart_buf[4]==0x6F && uart_buf[5]==0x73 && uart_buf[6]==0x65)
			{
				//�ؿյ�63 6C 6F 73 65
			}					
			else if(uart_buf[2]==0x73 && uart_buf[3]==0x68 && uart_buf[4]==0x65 && uart_buf[5]==0x6E && uart_buf[6]==0x67)
			{
				//���¶�73 68 65 6E 67
			}				
			else if(uart_buf[2]==0x6A && uart_buf[3]==0x69 && uart_buf[4]==0x61 && uart_buf[5]==0x6E && uart_buf[6]==0x64)
			{
				//���¶�6A 69 61 6E 64
			}				
			else if(uart_buf[2]==0x73 && uart_buf[3]==0x65 && uart_buf[4]==0x74 && uart_buf[5]==0x74 && uart_buf[6]==0x69)
			{
				//һСʱ�ػ�73 65 74 74 69
			}				
			else if(uart_buf[2]==0x65&& uart_buf[3]==0x78 && uart_buf[4]==0x69 && uart_buf[5]==0x74 && uart_buf[6]==0x55)
			{
				//����65 78 69 74 55
			}					
			else
			{
				//δָ֪��
			}
			
		}
		
		if(power)
		{pwr_on=0;}
		else
			pwr_on=1;
		
		if(light)
		{led_1=1;hsr1=1;}
		else
		{led_1=0;hsr1=0;}
		
		if(light2)
		{led_2=1;hsr2=1;}
		else
		{led_2=0;hsr2=0;}
		

		delayms(10);
		
	}
}


