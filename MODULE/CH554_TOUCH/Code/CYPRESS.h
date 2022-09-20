
/* REG INFO */
/********************************** (C) COPYRIGHT *******************************
* File Name          : FT6236.H
* Author             : GJ
* Version            : V2.0
* Date               : 2018-7-10 
* Description        : define of TP IC FT6236
*******************************************************************************/

#ifndef __CYPRESS_H__
#define __CYPRESS_H__


#include "DEBUG.H"
#include "IIC.H"
#include "FLASH_IC.H"
#include "DEVICE.H"
#include "CONFIG.H"
	
#define CY_CMD_WR 					0x48    	/* 写命令 */
#define CY_CMD_RD 					0x49		/* 读命令 */

			
/* 外部定义声明 */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern void CY_Init( void );
extern void CY_RD_Reg( UINT16 reg,UINT8 *buf );
extern UINT8 CY_Touch_Check( void );	
extern UINT8 CY_Scan( void );


#endif

/* END OF FILE */