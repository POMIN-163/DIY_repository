/********************************** (C) COPYRIGHT *******************************
* File Name          : GT911.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10 
* Description        : 电容触摸IC触摸IC驱动
*******************************************************************************/

#include "GT911.H"

#ifdef USE_GT_DEVICE

const UINT16 GT911_TPX_TBL[10]=			/* 兼容所有系列，10个手指都支持 */
{			
	GT_TP1_REG,
	GT_TP2_REG,
	GT_TP3_REG,
	GT_TP4_REG,
	GT_TP5_REG,
	GT_TP6_REG,
	GT_TP7_REG, 
	GT_TP8_REG, 
	GT_TP9_REG, 
	GT_TP10_REG
};


/*******************************************************************************
* Function Name  : GT911_WR_Reg
* Description    : 向GT911写入一次数据
* Input          : reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:写数据长度
* Output         : None
* Return         : 0,成功;1,失败.
*******************************************************************************/
static UINT8 GT911_WR_Reg(UINT16 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i;
	UINT8 ret=0;
	IIC_Start();
 	IIC_Send_Byte(GT_CMD_WR);   	//发送写命令 	 
	IIC_Wait_Ack();
	IIC_Send_Byte(reg>>8);   	//发送高8位地址
	IIC_Wait_Ack();
	IIC_Send_Byte(reg&0XFF);   	//发送低8位地址
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
* Function Name  : GT911_RD_Reg
* Description    : 从GT911读出一次数据
* Input          : reg:起始寄存器地址
                   buf:数据缓缓存区
                   len:读数据长度			  
* Output         : None
* Return         : None
*******************************************************************************/
static void GT911_RD_Reg(UINT16 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();	
 	IIC_Send_Byte(GT_CMD_WR);   					/* 发送写命令 */
	IIC_Wait_Ack(); 
 	IIC_Send_Byte(reg>>8);   						/* 发送高8位地址 */
	IIC_Wait_Ack(); 
 	IIC_Send_Byte(reg&0XFF);   						/* 发送低8位地址 */
	IIC_Wait_Ack(); 
	
 	IIC_Start(); 
	IIC_Send_Byte(GT_CMD_RD);   					/* 发送读命令 */
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++) 
	{
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); 		/* 发数据 */
	} 
    IIC_Stop();										/* 产生一个停止条件 */
} 

/*******************************************************************************
* Function Name  : GT911_Config
* Description    : 获取与配置触摸IC配置信息
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 GT911_Send_Cfg(void )
{
	UINT8 i=0;

	ShareBuf[0]=0;
	GT911_WR_Reg( GT_CTRL_REG, ShareBuf, 1);                        /* 模式： 读坐标状态 */
	
	GT911_RD_Reg( GT_PID_REG, ShareBuf, 14 );                      /* 坐标属性信息 */
	CTP_Info.x_max_pos = ( (UINT16)(ShareBuf[7])<<8 ) + ShareBuf[6];
	CTP_Info.y_max_pos = ( (UINT16)(ShareBuf[9])<<8 ) + ShareBuf[8];

#if DE_PRINTF
	printf("Coordinate Info\n");
	for( i = 0; i < 14; i++ )
	{
		printf("%02x\t",(UINT16)ShareBuf[i]);
	}
	printf("\n");	
#endif
	mDelaymS(200);
	while(1)
	{
		GT911_RD_Reg( GT_GSTID_REG, ShareBuf, 1 ); 
		if(ShareBuf[0] & 0x80)
		{
			GT911_RD_Reg( GT_GSTID_REG, ShareBuf, 9 );
			if((ShareBuf[2]==0) && (ShareBuf[3]==0) && (ShareBuf[4]==0) && (ShareBuf[5]==0) )
			{
				ShareBuf[0] = 0;
				GT911_WR_Reg( GT_GSTID_REG, ShareBuf, 1); 				
			}	
			else                     //清中断
			{
				break;
			}
		}
		else
		{
			break;
		}
		mDelaymS(15);
	}
	return 0;
}

/*******************************************************************************
* Function Name  : GT911_Init
* Description    : 触摸板初始化，获取ID，确认是否工作
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GT911_Init(void)
{
#if DE_PRINTF
	printf("gt init\n");
#endif
	I2c_Init();
	/* SET ADDRESS 0x28/0x29*/
#if (GT_DEV_ADDR == 0X14)	
	CTP_RST(1);
	CTP_INT(0);
	mDelaymS(5);
	CTP_RST(0);
	mDelayuS(120);
	CTP_INT(1);
	mDelayuS(120);
	CTP_RST(1);
#elif (GT_DEV_ADDR == 0X5D)	
	CTP_RST(1);
	CTP_INT(0);
	mDelayuS(7);
	CTP_RST(0);	
	mDelayuS(120);
	CTP_RST(1);
#endif	
	mDelaymS(60);
	CTP_INT(1);
	
	/* Config system infomation */	
	GT911_Send_Cfg();
	Get_Config_Info();
//	CTP_Info.x_max_pos = 800;
//	CTP_Info.y_max_pos = 1280;
	CTP_Info.X_Resolution = 4096 /(float)CTP_Info.x_max_pos;
	CTP_Info.Y_Resolution = 4096 /(float)CTP_Info.y_max_pos;	
#if DE_PRINTF
	printf("cpt x:%d y:%d\n",CTP_Info.x_max_pos,CTP_Info.y_max_pos);
	printf("ratio: %f %f\n",CTP_Info.X_Resolution,CTP_Info.Y_Resolution);
#endif
}

/*******************************************************************************
* Function Name  : GT911_Touch_Check
* Description    : 按键检测与上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
UINT8 GT911_Touch_Check( void )
{
	if( CTP_Info.IRQ_Flag == 1 )
	{
		if( GT911_Scan() == 1 )
		{
			Absolute_Up_Pack( &TP, CTP_Info.Press_Flag );
		}
		
		CTP_Info.IRQ_Flag = 0;		
		EX0 = 1;
	}
	return 0;
}


/*******************************************************************************
* Function Name  : GT911_Scan
* Description    : 扫描触摸屏(采用查询方式)
* Input          : None
* Output         : None
* Return         : 0,触屏无触摸;1,触屏有触摸
*******************************************************************************/
UINT8 GT911_Scan( void )
{	
	UINT16 id ;                                        //当前这次ID位域
	UINT8 i;
	UINT8 temp;
	UINT8 point_num;
	
	GT911_RD_Reg(GT_GSTID_REG,&point_num,1);								//读取触摸点的状态 
	if( ( point_num & BIT7 ) == 0 )                                      /* 正常不会执行这个的,表示GT芯片是否准备好 */
	{
		return 0;
	}
	point_num = point_num&0x0F;
	id = 0;
	
//	printf("point:%d\n",(UINT16)point_num);
	if(point_num > MAX_POINT)   point_num = MAX_POINT;
	
	for( i = 0; i != point_num; i++ )
	{
		GT911_RD_Reg( GT911_TPX_TBL[i], ShareBuf, 5 );
//		printf("%02x %02x %02x %02x %02x\n",(UINT16)ShareBuf[0],(UINT16)ShareBuf[1],(UINT16)ShareBuf[2],(UINT16)ShareBuf[3],(UINT16)ShareBuf[4]);
		
		//分析是否有效按下的点，可能point_num里面有全0的点。
		if( (ShareBuf[0] != 0) || (*((UINT32 *)&ShareBuf[1]) != 0) )
		{
			id |= (1<<ShareBuf[0]);
			TP[ ShareBuf[0] ].Tip_Switch = 1;
			TP[ ShareBuf[0] ].Contact_Identifier = ShareBuf[0];
			if( CTP_Info.x_y_swap == 1 )
			{
				TP[ShareBuf[0]].Y_pos = ( ((UINT16)ShareBuf[2]<<8)+ShareBuf[1] ) * CTP_Info.X_Resolution;
				TP[ShareBuf[0]].X_pos = ( ((UINT16)ShareBuf[4]<<8)+ShareBuf[3] ) * CTP_Info.Y_Resolution;
			}
			else
			{
				TP[ShareBuf[0]].X_pos = ( ((UINT16)ShareBuf[2]<<8)+ShareBuf[1] ) * CTP_Info.X_Resolution;
				TP[ShareBuf[0]].Y_pos = ( ((UINT16)ShareBuf[4]<<8)+ShareBuf[3] ) * CTP_Info.Y_Resolution;							
			}
			TP[ShareBuf[0]].Resolution_Multi = 0x0030;
			
			if ( CTP_Info.x_mirror == 1 )
			{
				if(TP[ShareBuf[0]].X_pos > 4096)
					TP[ShareBuf[0]].X_pos = 0;
				else
					TP[ShareBuf[0]].X_pos = 4096 - TP[ShareBuf[0]].X_pos;
			}
			
			if ( CTP_Info.y_mirror == 1 )
			{
				if(TP[ShareBuf[0]].Y_pos > 4096)
					TP[ShareBuf[0]].Y_pos = 0;
				else
					TP[ShareBuf[0]].Y_pos = 4096 - TP[ShareBuf[0]].Y_pos;
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
	
	temp = 0;
	GT911_WR_Reg(GT_GSTID_REG,&temp,1);							//清标志 		
	
	return 1;
}

#endif

/* END OF FILE */