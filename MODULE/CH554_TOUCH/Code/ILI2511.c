/********************************** (C) COPYRIGHT *******************************
* File Name          : ILI2511.C
* Author             : GJ
* Version            : V1.00
* Date               : 2018-9-25 
* Description        : 电容触摸IC触摸IC驱动
*******************************************************************************/

#include "ILI2511.H"

#ifdef USE_ILI_DEVICE 

/*******************************************************************************
* Function Name  : ILI_WR_Reg
* Description    : 向ILI写入一次数据
* Input          : reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:写数据长度
* Output         : None
* Return         : 0,成功;1,失败.
*******************************************************************************/
static UINT8 ILI_WR_Reg(UINT8 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i;
	UINT8 ret=0;
	IIC_Start();
 	IIC_Send_Byte(ILI_CMD_WR);   	//发送写命令 	 
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	        //发送8位地址
	IIC_Wait_Ack();

	for(i=0;i<len;i++)
	{
    	IIC_Send_Byte(buf[i]);  	//发数据
		ret=IIC_Wait_Ack();
		if(ret)break;  
	}
    IIC_Stop();					//产生一个停止条件	    
	return ret; 
}

/*******************************************************************************
* Function Name  : ILI_RD_Reg
* Description    : 从ILI读出一次数据
* Input          : reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:读数据长度			  
* Output         : None
* Return         : None
*******************************************************************************/
static void ILI_RD_Reg(UINT8 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();	
 	IIC_Send_Byte(ILI_CMD_WR);   					/* 发送写命令 */
	IIC_Wait_Ack(); 
 	IIC_Send_Byte(reg);   						    /* 发送8位地址 */
	IIC_Wait_Ack(); 
	IIC_Stop();										/* 产生一个停止条件 */	
	
 	IIC_Start();
//	mDelayuS(20);
	IIC_Send_Byte(ILI_CMD_RD);   					/* 发送读命令 */
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++) 
	{
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); 		/* 发数据 */
	}
    IIC_Stop();										/* 产生一个停止条件 */
}
/*******************************************************************************
* Function Name  : ILI_RD_Reg_Continue
* Description    : 从ILI再次接着读，适应于超过6点的情况
* Input          : reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:读数据长度			  
* Output         : None
* Return         : None
*******************************************************************************/
static void ILI_RD_Reg_Continue(UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();
//	mDelayuS(20);
	IIC_Send_Byte(ILI_CMD_RD);   					/* 发送读命令 */
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++) 
	{
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); 		/* 发数据 */
	} 
    IIC_Stop();										/* 产生一个停止条件 */	
}
/*******************************************************************************
* Function Name  : ILI_Check_Ver
* Description    : 检查协议版本
* Input          : None
* Output         : None
* Return         : 返回版本号（02H或03H）
*******************************************************************************/
UINT8 ILI_Check_Ver(void)
{
	UINT8 ver;
	ILI_RD_Reg(ILTK_GET_VER,&ver,1);
	return ver;
}
/*******************************************************************************
* Function Name  : ILI_GetPanelInfo
* Description    : 使用命令码从控制器读取X、Y分辨率。
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void ILI_GetPanelInfo(void)
{
	ILI_RD_Reg(ILTK_GET_DEV,&ShareBuf[0],4);
	CTP_Info.x_max_pos = ((UINT16)ShareBuf[1]<<8) + ShareBuf[0];
	CTP_Info.y_max_pos = ((UINT16)ShareBuf[3]<<8) + ShareBuf[2];
}
/*******************************************************************************
* Function Name  : ILI_Init
* Description    : 触摸板初始化,自动获取分辨率
* Input          : None
* Output         : None
* Return         : IC VERSION
*******************************************************************************/
void ILI_Init(void)
{
#if DE_PRINTF
	printf("ili2511 init\n");
#endif
	I2c_Init();
	/* 复位芯片 */
	P1_MOD_OC &= ~BIT6;                                       //必须推挽输出
	P1_DIR_PU |= BIT6;
	CTP_RST(0);
	CTP_INT(0);
	mDelayuS(10);
	CTP_INT(1);
	mDelayuS(100);
	CTP_RST(1);
	mDelaymS(200);

	ShareBuf[0] = ILI_Check_Ver();		                       /* 代码需要对版本02和03单独处理，目前只默认处理03版本 */
#if DE_PRINTF
	printf("ver:%02x\n",(UINT16)ShareBuf[0]);
#endif

	ILI_GetPanelInfo();
	Get_Config_Info();
	CTP_Info.X_Resolution = 4096 /(float)CTP_Info.x_max_pos;
	CTP_Info.Y_Resolution = 4096 /(float)CTP_Info.y_max_pos;	
#if DE_PRINTF
	printf("cpt x:%d y:%d\n",CTP_Info.x_max_pos,CTP_Info.y_max_pos);
	printf("ratio: %f %f\n",CTP_Info.X_Resolution,CTP_Info.Y_Resolution);
#endif
}
/*******************************************************************************
* Function Name  : ILI_Touch_Check
* Description    : 按键检测与上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 ILI_Touch_Check( void )
{
	if( CTP_Info.IRQ_Flag == 1 )
	{
		if( ILI_Scan() == 1 )
		{
			Absolute_Up_Pack( &TP, CTP_Info.Press_Flag );
		}
		
		CTP_Info.IRQ_Flag = 0;		
		EX0 = 1;
	}
	return 0;
}


/*******************************************************************************
* Function Name  : ILI_Scan
* Description    : 扫描触摸屏(采用查询方式)
* Input          : None
* Output         : None
* Return         : 0,触屏无触摸;1,触屏有触摸
*******************************************************************************/
UINT8 ILI_Scan( void )
{
	UINT16 id ;                                                //当前这次ID位域
	UINT8 i;
	UINT8 point_num;
	
	ILI_RD_Reg(ILTK_GET_TH,&ShareBuf[0],31);
	if(ShareBuf[0]==2)
	{
		ILI_RD_Reg_Continue(&ShareBuf[31],20);
		point_num = 10;
	}
	else
	{
		point_num = 6;
	}	
	id = 0;
//	printf("point:%d\n",(UINT16)point_num);
	if(point_num > MAX_POINT)   point_num = MAX_POINT;

	for( i = 0; i != point_num; i++ )
	{
#if 0
		printf("#%d:",(UINT16)i);
		printf("%02x ",(UINT16)ShareBuf[i*5+1]);
		printf("%02x ",(UINT16)ShareBuf[i*5+2]);
		printf("%02x ",(UINT16)ShareBuf[i*5+3]);
		printf("%02x ",(UINT16)ShareBuf[i*5+4]);
		printf("%02x ",(UINT16)ShareBuf[i*5+5]);
		printf("\n");		
#endif
		if(ShareBuf[i*5+1]&0x80)                        //按下
		{
			id |= (1<<i);
			TP[i].Tip_Switch = 1;
			TP[i].Contact_Identifier = i;
			TP[i].Resolution_Multi = 0x0030;
			if( CTP_Info.x_y_swap == 1 )
			{
				TP[i].Y_pos = ( (((UINT16)(ShareBuf[i*5+1]&0X3F)<<8))+ShareBuf[i*5+2] ) * CTP_Info.X_Resolution;
				TP[i].X_pos = ( ((UINT16)ShareBuf[i*5+3]<<8)+ShareBuf[i*5+4] ) * CTP_Info.Y_Resolution;
			}
			else
			{
				TP[i].X_pos = ( (((UINT16)(ShareBuf[i*5+1]&0X3F)<<8))+ShareBuf[i*5+2] ) * CTP_Info.X_Resolution;
				TP[i].Y_pos = ( ((UINT16)ShareBuf[i*5+3]<<8)+ShareBuf[i*5+4] ) * CTP_Info.Y_Resolution;							
			}
			if ( CTP_Info.x_mirror == 1 )
			{
				TP[i].X_pos = 4096 - TP[i].X_pos;
			}
			
			if ( CTP_Info.y_mirror == 1 )
			{
				TP[i].Y_pos = 4096 - TP[i].Y_pos;
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
