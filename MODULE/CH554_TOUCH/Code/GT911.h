/* REG INFO */
/********************************** (C) COPYRIGHT *******************************
* File Name          : GT911.H
* Author             : GJ
* Version            : V2.0
* Date               : 2018-7-10 
* Description        : define of TP IC GT911
*******************************************************************************/

/******************************************************************************
* MAX FREQ 400K Hz
* DEV_ADDR_T = DEV_ADDR + W/R
* HOST WRITE 
* +-------+----------+-----------+-----+------------+-----+------------+-----+----------+-----+-----+------+
* | START | DEV_ADDR | WRITE (0) | ACK | REG_ADDR_H | ACK | REG_ADDR_L | ACK | DATA ... | ... | ACK | STOP |
* +-------+----------+-----------+-----+------------+-----+------------+-----+----------+-----+-----+------+
* HOST READ 
* +-------+----------+----------+-----+------------+-----+------------+------+-------+----------+----------+-----+------+-----+-----+-----+-----+------+
* | START | DEV_ADDR | READ (1) | ACK | REG_ADDR_H | ACK | REG_ADDR_L | STOP | START | DEV_ADDR | READ (1) | ACK | DATA | ACK | ... | ... | NAK | STOP |
* +-------+----------+----------+-----+------------+-----+------------+------+-------+----------+----------+-----+------+-----+-----+-----+-----+------+
******************************************************************************/

#ifndef __GT911_H__
#define __GT911_H__

#include "DEBUG.H"
#include "FLASH_IC.H"
#include "IIC.H"
#include "DEVICE.H"
#include "CONFIG.H"

	
/* GT911�豸��ַѡ�� */	
//#define		GT_DEV_ADDR								(0x14)				/* DEV_ADDR */
#define		GT_DEV_ADDR								(0x5D)			/* DEV_ADDR */

//I2C��д����
#if (GT_DEV_ADDR == 0x14)
#define GT_CMD_WR 		0X28     	//д����
#define GT_CMD_RD 		0X29		//������
#else
#define GT_CMD_WR 		0XBA     	//д����
#define GT_CMD_RD 		0XBB		//������
#endif

//GT9147 ���ּĴ������� 
#define GT_CTRL_REG 	0X8040   	//GT9147���ƼĴ���
#define GT_CFGS_REG 	0X8047   	//GT9147������ʼ��ַ�Ĵ���
#define GT_CHECK_REG 	0X80FF   	//GT9147У��ͼĴ���
#define GT_PID_REG 		0X8140   	//GT9147��ƷID�Ĵ���

#define GT_GSTID_REG 	0X814E   	//GT9147��ǰ��⵽�Ĵ������
#define GT_TP1_REG 		0X814F  	//��һ�����������ݵ�ַ
#define GT_TP2_REG 		0X8157		//�ڶ������������ݵ�ַ
#define GT_TP3_REG 		0X815F		//���������������ݵ�ַ
#define GT_TP4_REG 		0X8167		//���ĸ����������ݵ�ַ
#define GT_TP5_REG 		0X816F		//��������������ݵ�ַ  
#define GT_TP6_REG 		0X8177		//���������������ݵ�ַ  
#define GT_TP7_REG 		0X817F		//���߸����������ݵ�ַ  
#define GT_TP8_REG 		0X8187		//�ڰ˸����������ݵ�ַ  
#define GT_TP9_REG 		0X818F		//�ھŸ����������ݵ�ַ  
#define GT_TP10_REG 	0X8197		//��ʮ�����������ݵ�ַ  
			
/* �ⲿ�������� */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern UINT8    GT911_Send_Cfg              ( void );
extern void 	GT911_Init			        ( void );
extern UINT8 	GT911_Touch_Check			( void );
extern UINT8 	GT911_Scan					( void );


#endif

/* END OF FILE */