/********************************** (C) COPYRIGHT *******************************
* File Name          : CONFIG.H
* Author             : GJ
* Version            : V2.00
* Date               : 2018/07/11
* Description        : 
*******************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* 设备选择（选一）（PS:还可以通过提供的配置工具进行配置选择,这样就可以每种屏烧一套代码） */

#define USE_GT_DEVICE             //选用GT系列芯片
//#define USE_FT_DEVICE             //选用FT系列芯片
//#define USE_CY_DEVICE             //选用CYPRESS系列芯片
//#define USE_ILI_DEVICE            //选用ILI芯片  

/* 引脚定义,如更换引脚需要连同配置一起更改，特别是INT脚，如不用单片机中断脚，代码一定要改成查询的方式 */
sbit        LED_IO	=       P1^7;
sbit 		RST_PIN	=  		P1^6;
sbit		INT_PIN	= 		P3^2;
sbit        IIC_SCL =       P1^4;                       		/*模拟I2C时钟控制位*/
sbit        IIC_SDA =       P1^5;                      	    	/*模拟I2C数据传送位*/

#define 	CTP_RST( x )			( RST_PIN = x )
#define		CTP_INT( x )			( INT_PIN = x )
#define 	CTP_INT_DAT( )		    ( INT_PIN )

/* 结构体定义 */
typedef struct
{
	UINT8 Tip_Switch;
	UINT8 Contact_Identifier;
	UINT16 X_pos;		/* coordinate X */
	UINT16 Y_pos;		/* coordinate Y */
	UINT16 Resolution_Multi;
} POINTER;

typedef struct 
{
	UINT16 	Press_Flag;
	UINT16 	x_max_pos;
	UINT16 	y_max_pos;
	UINT8 	x_mirror;
	UINT8	y_mirror;
	UINT8 	x_y_swap;
	float 	X_Resolution;
	float 	Y_Resolution;	
	UINT8 	IRQ_Flag;
} CTP_INFO ;




#endif
