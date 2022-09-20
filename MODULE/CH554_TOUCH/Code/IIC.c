/********************************** (C) COPYRIGHT *******************************
* File Name          : IIC.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10
* Description        : Interface of I2C
*******************************************************************************/
#include <intrins.h>
#include "DEBUG.H"
#include "CONFIG.H"

/*��ʱ�ӳ���*/
/*��ʱָ��΢��ʱ��*/          				 	
#define		DELAY5uS()		mDelayuS(5)    		/* �ʵ�������ʱ���ı�IIC�ٶ� */
#define		DELAY2uS()		mDelayuS(2)			
#define		DELAY1uS()		mDelayuS(1)			
#define 	IIC_READ		(0X01)
#define 	IIC_WRITE		(0X00)
/*100K DELAY5 ����_nop_();��20��*/
/*100K DELAY2 ����_nop_();��10��*/


/*******************************����ΪIIC�������������Ӻ���*********************************************************************/
void I2c_Init( void )
{
	P1_MOD_OC &= ~(BIT4 + BIT5);
	P1_DIR_PU |= (BIT4 + BIT5);
	P1 |= BIT4 + BIT5;
}

static void I2c_SDA_Out( void )
{
	 P1_MOD_OC &= ~BIT5;
	 P1_DIR_PU |= BIT5;
}

static void I2c_SDA_In( void )
{
	 P1_MOD_OC |= BIT5;
	 P1_DIR_PU |= BIT5;
}
/*******************************************************************************
* Function Name  : IIC_Start
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************************************************************/
void IIC_Start( void )
{
	IIC_SDA=1;
	I2c_SDA_Out(); 	  
	IIC_SCL=1;
	mDelayuS(5);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	DELAY2uS();
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
}

/*******************************************************************************
* Function Name  : IIC_Stop
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Stop( void )
{
	IIC_SDA = 0;
	I2c_SDA_Out();
	IIC_SCL=1;
	DELAY2uS();
	IIC_SDA=1;//����I2C���߽����ź�  
}

/*******************************************************************************
* Function Name  : Ack_I2c
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Ack( void )
{
	IIC_SCL=0;
	I2c_SDA_Out();
	IIC_SDA=0;
	DELAY1uS();
	IIC_SCL=1;
	DELAY2uS();	
	IIC_SCL=0;
	I2c_SDA_In();
	IIC_SDA=1;	
}

/*******************************************************************************
* Function Name  : Nack_I2c
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IIC_Nack(void)
{
	IIC_SCL=0;
	I2c_SDA_Out();
	IIC_SDA=1;
	DELAY1uS();
	IIC_SCL=1;
	DELAY2uS();
	IIC_SCL=0;
	I2c_SDA_In();
}			
/*******************************************************************************
* Function Name  : Ack_I2c
* Description    :�ȴ�Ӧ���źŵ���
				  ����ֵ��1������Ӧ��ʧ��
					      0������Ӧ��ɹ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 IIC_Wait_Ack(void)
{
	UINT8 ucErrTime=0;
	IIC_SDA=1;	   
	IIC_SCL=1;
	I2c_SDA_In();
	DELAY2uS();
	while(IIC_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
		mDelayuS(1);
	}	
	IIC_SCL=0;//ʱ�����0 	   
	return 0;  
} 
/*******************************************************************************
* Function Name  : IIC_Send_Byte
* Description    :
* Input          : UINT8 c
* Output         : None
* Return         : None
*******************************************************************************/
void  IIC_Send_Byte(UINT8 txd)
{
    UINT8 t;   
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
	I2c_SDA_Out();
	DELAY2uS();
	for(t=0;t<8;t++)
    {              
        IIC_SDA=(txd&0x80)>>7;
        txd<<=1;
		DELAY1uS();
		IIC_SCL=1; 
		DELAY1uS();
		if( t == 7 )
		{
			I2c_SDA_In();
		}
		IIC_SCL=0;	
		DELAY1uS();
    }
}

/*******************************************************************************
* Function Name  : IIC_Read_Byte
* Description    :
* Input          : None
* Output         : None
* Return         : UINT8
*******************************************************************************/
UINT8  IIC_Read_Byte(UINT8 ack)
{
	UINT8 i,receive=0;
	DELAY2uS();
	I2c_SDA_In();
	for(i=0;i<8;i++ )
	{ 
		IIC_SCL=0; 	    	   
		DELAY2uS();
		IIC_SCL=1;	 
		receive<<=1;
		if(IIC_SDA)
			receive++;   
	}
	if (!ack)
		IIC_Nack();//����nACK
	else 
		IIC_Ack(); //����ACK   
 	return receive;
}
