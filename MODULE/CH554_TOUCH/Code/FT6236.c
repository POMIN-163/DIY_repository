/********************************** (C) COPYRIGHT *******************************
* File Name          : FT6236.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10 
* Description        : 电容触摸IC触摸IC驱动
*******************************************************************************/

#include "FT6236.H"

#ifdef USE_FT_DEVICE

const UINT16 FT_TPX_TBL[10] =
{
	DEF_FT_TP1_REG,
	DEF_FT_TP2_REG,
	DEF_FT_TP3_REG,
	DEF_FT_TP4_REG,
	DEF_FT_TP5_REG,
	DEF_FT_TP6_REG,
	DEF_FT_TP7_REG,
	DEF_FT_TP8_REG,
	DEF_FT_TP9_REG,
	DEF_FT_TP10_REG,
};

/*******************************************************************************
* Function Name  : FT_WR_Reg
* Description    : 向FT写入一次数据
                   reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:写数据长度
                   返回值:0,成功;1,失败.
* Input          : UINT16 Addr ,UINT8 dat
* Output         : None
* Return         : bit
*******************************************************************************/
static UINT8 FT_WR_Reg(UINT16 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i;
	UINT8 ret=0;
	IIC_Start();	 
	IIC_Send_Byte(FT_CMD_WR); 
	IIC_Wait_Ack(); 
	IIC_Send_Byte(reg&0XFF); 
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++)
	{ 
    	IIC_Send_Byte(buf[i]); 
		ret=IIC_Wait_Ack();
		if(ret)break; 
	}
    IIC_Stop(); 
	return ret; 
}
/*******************************************************************************
* Function Name  : FT_RD_Reg
* Description    : 从FT读出一次数据
                   reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:读数据长度			 
* Input          : UINT16 reg,
				   UINT8 *buf,
				   UINT8 len
* Output         : None
* Return         : None
*******************************************************************************/
static void FT_RD_Reg(UINT16 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();	
 	IIC_Send_Byte(FT_CMD_WR);   	//发送写命令 	 
	IIC_Wait_Ack(); 	 										  		   
 	IIC_Send_Byte(reg&0XFF);   	    //发送低8位地址
	IIC_Wait_Ack();  
 	IIC_Start();  	 	   
	IIC_Send_Byte(FT_CMD_RD);   	//发送读命令		   
	IIC_Wait_Ack();	   
	for(i=0;i<len;i++)
	{	   
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); //发数据	  
	} 
    IIC_Stop();//产生一个停止条件     
}
/*******************************************************************************
* Function Name  : FT_Init
* Description    : 初始化FT触摸屏
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FT_Init()
{
	I2c_Init( );
#if DE_PRINTF
	printf("ft init\n");
#endif
	CTP_INT(1);
	CTP_RST(0);				//复位
	mDelaymS(50);
 	CTP_RST(1);				//释放复位		    
	mDelaymS(200);	        //必须要延时100多ms
	
	//进入正常操作模式 
	ShareBuf[0]=0;
	FT_WR_Reg(DEF_FT_DEVIDE_MODE,ShareBuf,1);	

	ShareBuf[0]=1;                                  //设置触发方式 0: Polling mode   1: Trigger mode
	FT_WR_Reg(DEF_FT_ID_G_MODE,ShareBuf,1);	
#if DE_PRINTF
	FT_RD_Reg(DEF_FT_ID_G_LIB_VERSION,&ShareBuf[0],2);  
	printf("cpt version: %02x %02x\n",(UINT16)ShareBuf[0],(UINT16)ShareBuf[1]);	
#endif	
	Get_Config_Info();
	
	CTP_Info.X_Resolution = 4096 /(float)CTP_Info.x_max_pos;
	CTP_Info.Y_Resolution = 4096 /(float)CTP_Info.y_max_pos;	
#if DE_PRINTF
	printf("cpt x:%d y:%d\n",CTP_Info.x_max_pos,CTP_Info.y_max_pos);
	printf("ratio: %f %f\n",CTP_Info.X_Resolution,CTP_Info.Y_Resolution);
#endif	
}
/*******************************************************************************
* Function Name  : FT_Touch_Check
* Description    : 按键检测与上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 FT_Touch_Check( void )
{
	if( CTP_Info.IRQ_Flag == 1 )
	{
		if( FT_Scan() == 1 )
		{
			Absolute_Up_Pack( &TP, CTP_Info.Press_Flag );
		}
		
		CTP_Info.IRQ_Flag = 0;		
		EX0 = 1;
	}
	return 0;
}
/*******************************************************************************
* Function Name  : FT_Scan
* Description    : 扫描触摸屏(采用查询方式)
* Input          : None
* Output         : None
* Return         : 0,触屏无触摸;1,触屏有触摸
*******************************************************************************/
UINT8 FT_Scan( void )
{
	UINT16 id ;                                        //当前这次ID位域
	UINT8 i;
	UINT8 point_num;
	UINT8 point_id;                                    //点的ID号
	
	FT_RD_Reg(DEF_FT_REG_NUM_FINGER,&point_num,1);	  
	id = 0;
	
//	printf("point:%d\n",(UINT16)point_num);
	if(point_num > MAX_POINT)   point_num = MAX_POINT;
	
	for( i = 0; i != point_num; i++ )
	{
		FT_RD_Reg( FT_TPX_TBL[i], ShareBuf, 4 );
//		printf("%02x %02x %02x %02x \n",(UINT16)ShareBuf[0],(UINT16)ShareBuf[1],(UINT16)ShareBuf[2],(UINT16)ShareBuf[3]);
		point_id = ShareBuf[2]>>4;
		
		//分析是否有效按下的点，可能point_num里面有全FF的点。
		if( point_id != 0x0F )
		{
			id |= (1<<point_id);
			TP[ point_id ].Tip_Switch = 1;
			TP[ point_id ].Contact_Identifier = point_id;
			if( CTP_Info.x_y_swap == 1 )
			{
				TP[point_id].Y_pos = (((UINT16)( ShareBuf[0]&0X0F)<<8)+ShareBuf[1]) * CTP_Info.X_Resolution;
				TP[point_id].X_pos = (((UINT16)( ShareBuf[2]&0X0F)<<8)+ShareBuf[3]) * CTP_Info.Y_Resolution;
			}
			else
			{
				TP[point_id].X_pos = (((UINT16)( ShareBuf[0]&0X0F)<<8)+ShareBuf[1]) * CTP_Info.X_Resolution;
				TP[point_id].Y_pos = (((UINT16)( ShareBuf[2]&0X0F)<<8)+ShareBuf[3]) * CTP_Info.Y_Resolution;							
			}
			TP[point_id].Resolution_Multi = 0x0030;
			
			if ( CTP_Info.x_mirror == 1 )
			{
				TP[point_id].X_pos = 4096 - TP[point_id].X_pos;
			}
			
			if ( CTP_Info.y_mirror == 1 )
			{
				TP[point_id].Y_pos = 4096 - TP[point_id].Y_pos;
			}
		}		
	}
	//根据id相应位为0，清零TP[].Tip_Switch
	for(i=0;i!=MAX_POINT;i++)
	{
		if((id & (1<<i)) == 0)
		{
			TP[i].Tip_Switch = 0;			
		}
	}	
	CTP_Info.Press_Flag = id;                                //按下标志位域		
	
	return 1;
}

#endif

