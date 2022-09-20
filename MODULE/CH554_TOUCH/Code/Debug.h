/********************************** (C) COPYRIGHT *******************************
* File Name          : DEBUG.H
* Author             : RZ
* Version            : V1.00
* Date               : 2017-5-17
* Description        : 1) ���Դ�ӡ
					   2) ʱ������
					   3) �ṩprintf�ӳ������ʱ���� 
*******************************************************************************/

#ifndef	__DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <string.h>
#include "CH554.H"

//���庯������ֵ
#ifndef  SUCCESS
#define  SUCCESS  0
#endif
#ifndef  FAIL
#define  FAIL    0xFF
#endif

//���嶨ʱ����ʼ
#ifndef  START
#define  START  1
#endif
#ifndef  STOP
#define  STOP    0
#endif

#ifndef  DE_PRINTF
#define  DE_PRINTF      1
#endif
#define	 FREQ_SYS	    16000000	         //ϵͳ��Ƶ16MHz
#ifndef  UART0_BUAD
#define  UART0_BUAD    	115200
#define  UART1_BUAD   	115200
#endif

#define 	BIT0										(0x01)
#define 	BIT1										(0x02)
#define 	BIT2										(0x04)
#define 	BIT3										(0x08)
#define 	BIT4										(0x10)
#define 	BIT5										(0x20)
#define 	BIT6										(0x40)
#define 	BIT7										(0x80)


void	CfgFsys					( void );                   /* CH554ʱ��ѡ������� */
void	mDelayuS				( UINT16 n );              	/* ��uSΪ��λ��ʱ */
void	mDelaymS				( UINT16 n );              	/* ��mSΪ��λ��ʱ */
void  	CH554UART0Alter			( void );                 	/* CH554����0����ӳ�䵽P0.2/P0.3 */
void	mInitSTDIO				( void );                   /* T1��Ϊ�����ʷ����� */
UINT8 	CH554UART0RcvByte		( void );              		/* CH554 UART0��ѯ��ʽ����һ���ֽ� */
void  	CH554UART0SendByte		(UINT8 SendDat); 			/* CH554UART0����һ���ֽ� */

void	UART1Setup				( void );                   /* UART1 ���� */
UINT8 	CH554UART1RcvByte		( void );              		/* CH554 UART1��ѯ��ʽ����һ���ֽ� */
void  	CH554UART1SendByte		(UINT8 SendDat); 			/* CH554UART1����һ���ֽ� */
		                             
void 	CH554WDTModeSelect		(UINT8 mode);    			/* CH554���Ź�ģʽ���� */
void 	CH554WDTFeed			(UINT8 tim);           		/* CH554���Ź�ι�� */



#endif

/* END OF FILE */