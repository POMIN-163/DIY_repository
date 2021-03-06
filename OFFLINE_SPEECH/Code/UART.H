#ifndef __UART_H__
#define __UART_H__

#define bps 						115200				//������
#define FOSC            11059200UL		//����Ƶ��
#define BRT             (65536 - FOSC / bps / 4)

//sfr     AUXR        =   0x8e; 
extern unsigned char uart_buf[32];
extern bit new_flag;

void UartInit();
void UartSend(char dat);
void UartSendStr(char *p);

#endif
