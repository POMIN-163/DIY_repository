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

	
//I2C��д����
#define FT_CMD_WR 					0X70    	/* д���� */
#define FT_CMD_RD 					0X71		/* ������ */
                                         
//FT ���ּĴ�������                   
#define DEF_FT_DEVIDE_MODE 			0x00   		/* FT5206ģʽ���ƼĴ��� */
#define DEF_FT_REG_NUM_FINGER       0x02		/* ����״̬�Ĵ��� */
                                           
#define DEF_FT_TP1_REG 				0X03	  	/* ��һ�����������ݵ�ַ */
#define DEF_FT_TP2_REG 				0X09		/* �ڶ������������ݵ�ַ */
#define DEF_FT_TP3_REG 				0X0F		/* ���������������ݵ�ַ */
#define DEF_FT_TP4_REG 				0X15		/* ���ĸ����������ݵ�ַ */
#define DEF_FT_TP5_REG 				0X1B		/* ��������������ݵ�ַ */
#define DEF_FT_TP6_REG 				0X21		/* ���������������ݵ�ַ */
#define DEF_FT_TP7_REG 				0X27		/* ���߸����������ݵ�ַ */
#define DEF_FT_TP8_REG 				0X2D		/* �ڰ˸����������ݵ�ַ */
#define DEF_FT_TP9_REG 				0X33		/* �ھŸ����������ݵ�ַ */
#define DEF_FT_TP10_REG 			0X39		/* ��ʮ�����������ݵ�ַ */

#define	DEF_FT_ID_G_LIB_VERSION		0xA1		/* �汾 */
#define DEF_FT_ID_G_MODE 			0xA4   		/* FT5206�ж�ģʽ���ƼĴ��� */
#define DEF_FT_ID_G_THGROUP			0x80   		/* ������Чֵ���üĴ��� */
#define DEF_FT_ID_G_PERIODACTIVE	0x88   		/* ����״̬�������üĴ��� */

			
/* �ⲿ�������� */	
extern POINTER TP[ 10 ];
extern CTP_INFO CTP_Info;
extern UINT8 ShareBuf[];

/* Function Define */
extern void  FT_Init(void);
extern UINT8 FT_Touch_Check( void );
extern UINT8 FT_Scan( void );


#endif

/* END OF FILE */