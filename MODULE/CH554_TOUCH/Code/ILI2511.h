
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

/* ILI2511读写寄存器 */	
#define ILI_CMD_WR 		0X82     	//写命令
#define ILI_CMD_RD 		0X83		//读命令

/* ILI2511其他寄存器 */
#define ILTK_GET_VER    0X42        //获取协议版本
#define ILTK_GET_DEV    0X20        //获取边界值
#define ILTK_GET_TH     0X10        //获取触摸值
	
	
/* 外部定义声明 */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern void  ILI_Init(void);
extern UINT8 ILI_Touch_Check( void );
extern UINT8 ILI_Scan( void );


#endif

/* END OF FILE */