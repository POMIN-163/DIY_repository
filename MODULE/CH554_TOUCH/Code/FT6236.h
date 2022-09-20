/* REG INFO */
/********************************** (C) COPYRIGHT *******************************
* File Name          : FT6236.H
* Author             : GJ
* Version            : V2.0
* Date               : 2018-7-10 
* Description        : define of TP IC FT6236
*******************************************************************************/

#ifndef __FT6236_H__
#define __FT6236_H__


#include "DEBUG.H"
#include "IIC.H"
#include "FLASH_IC.H"
#include "DEVICE.H"
#include "CONFIG.H"

	
//I2C读写命令
#define FT_CMD_WR 					0X70    	/* 写命令 */
#define FT_CMD_RD 					0X71		/* 读命令 */
                                         
//FT 部分寄存器定义                   
#define DEF_FT_DEVIDE_MODE 			0x00   		/* FT5206模式控制寄存器 */
#define DEF_FT_REG_NUM_FINGER       0x02		/* 触摸状态寄存器 */
                                           
#define DEF_FT_TP1_REG 				0X03	  	/* 第一个触摸点数据地址 */
#define DEF_FT_TP2_REG 				0X09		/* 第二个触摸点数据地址 */
#define DEF_FT_TP3_REG 				0X0F		/* 第三个触摸点数据地址 */
#define DEF_FT_TP4_REG 				0X15		/* 第四个触摸点数据地址 */
#define DEF_FT_TP5_REG 				0X1B		/* 第五个触摸点数据地址 */
#define DEF_FT_TP6_REG 				0X21		/* 第六个触摸点数据地址 */
#define DEF_FT_TP7_REG 				0X27		/* 第七个触摸点数据地址 */
#define DEF_FT_TP8_REG 				0X2D		/* 第八个触摸点数据地址 */
#define DEF_FT_TP9_REG 				0X33		/* 第九个触摸点数据地址 */
#define DEF_FT_TP10_REG 			0X39		/* 第十个触摸点数据地址 */

#define	DEF_FT_ID_G_LIB_VERSION		0xA1		/* 版本 */
#define DEF_FT_ID_G_MODE 			0xA4   		/* FT5206中断模式控制寄存器 */
#define DEF_FT_ID_G_THGROUP			0x80   		/* 触摸有效值设置寄存器 */
#define DEF_FT_ID_G_PERIODACTIVE	0x88   		/* 激活状态周期设置寄存器 */

			
/* 外部定义声明 */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern void  FT_Init(void);
extern UINT8 FT_Touch_Check( void );
extern UINT8 FT_Scan( void );


#endif

/* END OF FILE */