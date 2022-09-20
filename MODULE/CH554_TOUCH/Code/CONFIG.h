/********************************** (C) COPYRIGHT *******************************
* File Name          : CONFIG.H
* Author             : GJ
* Version            : V2.00
* Date               : 2018/07/11
* Description        : 
*******************************************************************************/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/* �豸ѡ��ѡһ����PS:������ͨ���ṩ�����ù��߽�������ѡ��,�����Ϳ���ÿ������һ�״��룩 */

#define USE_GT_DEVICE             //ѡ��GTϵ��оƬ
//#define USE_FT_DEVICE             //ѡ��FTϵ��оƬ
//#define USE_CY_DEVICE             //ѡ��CYPRESSϵ��оƬ
//#define USE_ILI_DEVICE            //ѡ��ILIоƬ  

/* ���Ŷ���,�����������Ҫ��ͬ����һ����ģ��ر���INT�ţ��粻�õ�Ƭ���жϽţ�����һ��Ҫ�ĳɲ�ѯ�ķ�ʽ */
sbit        LED_IO	=       P1^7;
sbit 		RST_PIN	=  		P1^6;
sbit		INT_PIN	= 		P3^2;
sbit        IIC_SCL =       P1^4;                       		/*ģ��I2Cʱ�ӿ���λ*/
sbit        IIC_SDA =       P1^5;                      	    	/*ģ��I2C���ݴ���λ*/

#define 	CTP_RST( x )			( RST_PIN = x )
#define		CTP_INT( x )			( INT_PIN = x )
#define 	CTP_INT_DAT( )		    ( INT_PIN )

/* �ṹ�嶨�� */
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
