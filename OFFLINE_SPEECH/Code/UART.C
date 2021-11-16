#include "stc8.h"
#include "stdio.h"
#include "UART.H"

bit busy;
char uart_wptr;
//char uart_rptr;
unsigned char uart_buf[32]=0;
//char uart_str;
unsigned char state=0,rec_num=0,rec_max=1;		//监测状态,接收数,最大接收数
bit rec_flag,new_flag=0;		//可以接收标志和新接收标志

void UartIsr() interrupt 4
{
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
    if (RI)
    {
        RI = 0;
			if(rec_flag)		//如果有接收标志
			{
				uart_buf[rec_num++]=SBUF;				//接收
				if(rec_max==4 && rec_num==4)
				{
					rec_max=uart_buf[3];
					rec_num=0;
				}
				else if(rec_num>=rec_max)
				{
					rec_flag=0;
					rec_num=0;
					new_flag=1;
				}
			}

			if(state==0 && SBUF==0X00)				//特定开头开始接收
				state=1;
			else if(state==1 && SBUF==0X03)
				state=2;
			else if(state==2 && SBUF==0XE9)
				{state=0;rec_flag=1;rec_num=0;rec_max=4;}			
			else
				state=0;

    }
}

void UartInit()
{
    SCON = 0x50;
    T2L = BRT;
    T2H = BRT >> 8;
    AUXR = 0x15;
    uart_wptr = 0x00;
    //uart_rptr = 0x00;
    busy = 0;
		TI=1;
		ES=1;
		EA=1;
}

void UartSend(char dat)
{
    while (busy);
    busy = 1;
    SBUF = dat;
}

void UartSendStr(char *p)
{
    while (*p)
    {
        UartSend(*p++);
    }
}

