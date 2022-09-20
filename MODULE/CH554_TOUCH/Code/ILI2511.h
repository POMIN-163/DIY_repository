
/********************************** (C) COPYRIGHT *******************************
* File Name          : GT911.H
* Author             : GJ
* Version            : V2.0
* Date               : 2018-9-25 
* Description        : define of TP IC ILI2511
*******************************************************************************/

#ifndef __ILI2511_H__
#define __ILI2511_H__


#include "DEBUG.H"
#include "FLASH_IC.H"
#include "IIC.H"
#include "DEVICE.H"
#include "CONFIG.H"

/* ILI2511��д�Ĵ��� */	
#define ILI_CMD_WR 		0X82     	//д����
#define ILI_CMD_RD 		0X83		//������

/* ILI2511�����Ĵ��� */
#define ILTK_GET_VER    0X42        //��ȡЭ��汾
#define ILTK_GET_DEV    0X20        //��ȡ�߽�ֵ
#define ILTK_GET_TH     0X10        //��ȡ����ֵ
	
	
/* �ⲿ�������� */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern void  ILI_Init(void);
extern UINT8 ILI_Touch_Check( void );
extern UINT8 ILI_Scan( void );


#endif

/* END OF FILE */