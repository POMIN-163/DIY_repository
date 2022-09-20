/******************** (C) COPYRIGHT 2009 WCH ***********************************
* File Name          : FLASH_IC.H
* Author             : RZ
* Version            : V1.00
* Date               : 2017/04/27
* Description        :
*******************************************************************************/



#ifndef __FLASH_IC_H
#define __FLASH_IC_H

#include "DEBUG.H"
#include "CONFIG.H"

#define	FLASH_DATA_LEN 	  6
#define	FLASH_DATA_ADDR	  0x0000

extern CTP_INFO CTP_Info;

extern UINT8 Flash_Data[FLASH_DATA_LEN];

extern UINT8 Get_Config_Info( void );



#endif
/******************* (C) COPYRIGHT 2008 WCH*********************END OF FILE****/
