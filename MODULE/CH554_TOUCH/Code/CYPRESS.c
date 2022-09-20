/********************************** (C) COPYRIGHT *******************************
* File Name          : CYPRESS.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10 
* Description        : 电容触摸IC触摸IC驱动
*******************************************************************************/

#include "CYPRESS.H"

#ifdef USE_CY_DEVICE

/*******************************************************************************
* Function Name  : CY_RD_Reg
* Description    : 	 
* Input          : UINT16 reg,
				   UINT8 *buf,长度存数在第一个字节里面
* Output         : None
* Return         : None
*******************************************************************************/
void CY_RD_Reg(UINT16 reg,UINT8 *buf )
{
	UINT8 i;
 	IIC_Start();	
 	IIC_Send_Byte( CY_CMD_WR );
	IIC_Wait_Ack(); 	 										  		   
 	IIC_Send_Byte(reg&0xFF);       //低字节
	IIC_Wait_Ack(); 	 										  		   
 	IIC_Send_Byte(reg>>8);         //高字节
	IIC_Wait_Ack(); 	 										  		   
	IIC_Stop();

 	IIC_Start();	
 	IIC_Send_Byte( CY_CMD_RD );
	IIC_Wait_Ack(); 	 										  		   

	buf[0]=IIC_Read_Byte(1);
	for(i=1;i<buf[0];i++)
	{
		buf[i]=IIC_Read_Byte(1); 
	}
	IIC_Stop();
}

/*******************************************************************************
* Function Name  : CY_Init
* Description    : 初始化CY触摸屏
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CY_Init( void )
{
#if DE_PRINTF
	printf("cy init\n");
#endif
	I2c_Init( );
	
	CTP_INT(1);
	CTP_RST(0);				//复位
	mDelaymS(50);
 	CTP_RST(1);				//释放复位		    
	mDelaymS(200);	        //必须要延时100多ms	
	
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
UINT8 CY_Touch_Check( void )
{
	if( CY_Scan() == 1 )
	{
		Absolute_Up_Pack( &TP, CTP_Info.Press_Flag );
	}
	return 0;
}

/*******************************************************************************
* Function Name  : CY_Scan
* Description    : 扫描触摸屏(采用查询方式)
* Input          : None
* Output         : None
* Return         : 0,触屏无触摸;1,触屏有触摸
*******************************************************************************/
UINT8 CY_Scan( void )
{
	UINT16 id ;                                        //当前这次ID位域
	UINT8 i;
	UINT8 point_num;
	UINT8 point_id;                                    //点的ID号
	
	ShareBuf[0] = 0;
	CY_RD_Reg( 0x03,ShareBuf );
#if 0
	for(i=0;i!=ShareBuf[0];i++)
	printf("%02x ",(UINT16)ShareBuf[i]);
	printf("\n");
#endif
	if( (ShareBuf[0] >= 17) && (ShareBuf[0] != 0xFF) ) //一个点占10个字节
	{
		point_num = ( ShareBuf[0] - 7 )/10;
	}
	else
	{
		if(CTP_Info.Press_Flag)                        //按下不动也是没值的，就用前一次的值
		{
			return 0x01;
		}
		else
		{
			return 0;
		}
	}
	
	id = 0;	
	if(point_num > MAX_POINT)   point_num = MAX_POINT;
	
	for( i = 0; i != point_num; i++ )
	{
		point_id = ShareBuf[8+10*i] & 0x0F;

		TP[ point_id ].Tip_Switch = (ShareBuf[8+10*i] & 0x80)? 1:0;
		if( TP[ point_id ].Tip_Switch ) 
		{
			id |= (1<<point_id);
		}
		TP[ point_id ].Contact_Identifier = point_id;
		if( CTP_Info.x_y_swap == 1 )
		{
			TP[point_id].Y_pos = (ShareBuf[ 9+10*i]+((UINT16)ShareBuf[10+10*i]<<8)) * CTP_Info.X_Resolution;
			TP[point_id].X_pos = (ShareBuf[11+10*i]+((UINT16)ShareBuf[12+10*i]<<8)) * CTP_Info.Y_Resolution;
		}
		else
		{
			TP[point_id].X_pos = (ShareBuf[ 9+10*i]+((UINT16)ShareBuf[10+10*i]<<8)) * CTP_Info.X_Resolution;
			TP[point_id].Y_pos = (ShareBuf[11+10*i]+((UINT16)ShareBuf[12+10*i]<<8)) * CTP_Info.Y_Resolution;							
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
