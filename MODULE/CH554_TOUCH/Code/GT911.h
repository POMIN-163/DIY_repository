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

	
/* GT911设备地址选择 */	
//#define		GT_DEV_ADDR								(0x14)				/* DEV_ADDR */
#define		GT_DEV_ADDR								(0x5D)			/* DEV_ADDR */

//I2C读写命令
#if (GT_DEV_ADDR == 0x14)
#define GT_CMD_WR 		0X28     	//写命令
#define GT_CMD_RD 		0X29		//读命令
#else
#define GT_CMD_WR 		0XBA     	//写命令
#define GT_CMD_RD 		0XBB		//读命令
#endif

//GT9147 部分寄存器定义 
#define GT_CTRL_REG 	0X8040   	//GT9147控制寄存器
#define GT_CFGS_REG 	0X8047   	//GT9147配置起始地址寄存器
#define GT_CHECK_REG 	0X80FF   	//GT9147校验和寄存器
#define GT_PID_REG 		0X8140   	//GT9147产品ID寄存器

#define GT_GSTID_REG 	0X814E   	//GT9147当前检测到的触摸情况
#define GT_TP1_REG 		0X814F  	//第一个触摸点数据地址
#define GT_TP2_REG 		0X8157		//第二个触摸点数据地址
#define GT_TP3_REG 		0X815F		//第三个触摸点数据地址
#define GT_TP4_REG 		0X8167		//第四个触摸点数据地址
#define GT_TP5_REG 		0X816F		//第五个触摸点数据地址  
#define GT_TP6_REG 		0X8177		//第六个触摸点数据地址  
#define GT_TP7_REG 		0X817F		//第七个触摸点数据地址  
#define GT_TP8_REG 		0X8187		//第八个触摸点数据地址  
#define GT_TP9_REG 		0X818F		//第九个触摸点数据地址  
#define GT_TP10_REG 	0X8197		//第十个触摸点数据地址  
			
/* 外部定义声明 */	
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