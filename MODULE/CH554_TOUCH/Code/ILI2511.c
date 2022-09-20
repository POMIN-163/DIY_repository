/********************************** (C) COPYRIGHT *******************************
* File Name          : ILI2511.C
* Author             : GJ
* Version            : V1.00
* Date               : 2018-9-25 
* Description        : ���ݴ���IC����IC����
*******************************************************************************/

#include "ILI2511.H"

#ifdef USE_ILI_DEVICE 

/*******************************************************************************
* Function Name  : ILI_WR_Reg
* Description    : ��ILIд��һ������
* Input          : reg:��ʼ�Ĵ�����ַ
                   buf:���ݻ�������
                   len:д���ݳ���
* Output         : None
* Return         : 0,�ɹ�;1,ʧ��.
*******************************************************************************/
static UINT8 ILI_WR_Reg(UINT8 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i;
	UINT8 ret=0;
	IIC_Start();
 	IIC_Send_Byte(ILI_CMD_WR);   	//����д���� 	 
	IIC_Wait_Ack();
	IIC_Send_Byte(reg);   	        //����8λ��ַ
	IIC_Wait_Ack();

	for(i=0;i<len;i++)
	{
    	IIC_Send_Byte(buf[i]);  	//������
		ret=IIC_Wait_Ack();
		if(ret)break;  
	}
    IIC_Stop();					//����һ��ֹͣ����	    
	return ret; 
}

/*******************************************************************************
* Function Name  : ILI_RD_Reg
* Description    : ��ILI����һ������
* Input          : reg:��ʼ�Ĵ�����ַ
                   buf:���ݻ�������
                   len:�����ݳ���			  
* Output         : None
* Return         : None
*******************************************************************************/
static void ILI_RD_Reg(UINT8 reg,UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();	
 	IIC_Send_Byte(ILI_CMD_WR);   					/* ����д���� */
	IIC_Wait_Ack(); 
 	IIC_Send_Byte(reg);   						    /* ����8λ��ַ */
	IIC_Wait_Ack(); 
	IIC_Stop();										/* ����һ��ֹͣ���� */	
	
 	IIC_Start();
//	mDelayuS(20);
	IIC_Send_Byte(ILI_CMD_RD);   					/* ���Ͷ����� */
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++) 
	{
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); 		/* ������ */
	}
    IIC_Stop();										/* ����һ��ֹͣ���� */
}
/*******************************************************************************
* Function Name  : ILI_RD_Reg_Continue
* Description    : ��ILI�ٴν��Ŷ�����Ӧ�ڳ���6������
* Input          : reg:��ʼ�Ĵ�����ַ
                   buf:���ݻ�������
                   len:�����ݳ���			  
* Output         : None
* Return         : None
*******************************************************************************/
static void ILI_RD_Reg_Continue(UINT8 *buf,UINT8 len)
{
	UINT8 i; 
 	IIC_Start();
//	mDelayuS(20);
	IIC_Send_Byte(ILI_CMD_RD);   					/* ���Ͷ����� */
	IIC_Wait_Ack(); 
	for(i=0;i<len;i++) 
	{
    	buf[i]=IIC_Read_Byte(i==(len-1)?0:1); 		/* ������ */
	} 
    IIC_Stop();										/* ����һ��ֹͣ���� */	
}
/*******************************************************************************
* Function Name  : ILI_Check_Ver
* Description    : ���Э��汾
* Input          : None
* Output         : None
* Return         : ���ذ汾�ţ�02H��03H��
*******************************************************************************/
UINT8 ILI_Check_Ver(void)
{
	UINT8 ver;
	ILI_RD_Reg(ILTK_GET_VER,&ver,1);
	return ver;
}
/*******************************************************************************
* Function Name  : ILI_GetPanelInfo
* Description    : ʹ��������ӿ�������ȡX��Y�ֱ��ʡ�
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
* Description    : �������ʼ��,�Զ���ȡ�ֱ���
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
	/* ��λоƬ */
	P1_MOD_OC &= ~BIT6;                                       //�����������
	P1_DIR_PU |= BIT6;
	CTP_RST(0);
	CTP_INT(0);
	mDelayuS(10);
	CTP_INT(1);
	mDelayuS(100);
	CTP_RST(1);
	mDelaymS(200);

	ShareBuf[0] = ILI_Check_Ver();		                       /* ������Ҫ�԰汾02��03��������ĿǰֻĬ�ϴ���03�汾 */
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
* Description    : ����������ϴ�
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
* Description    : ɨ�败����(���ò�ѯ��ʽ)
* Input          : None
* Output         : None
* Return         : 0,�����޴���;1,�����д���
*******************************************************************************/
UINT8 ILI_Scan( void )
{
	UINT16 id ;                                                //��ǰ���IDλ��
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
		if(ShareBuf[i*5+1]&0x80)                        //����
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
	//����id��ӦλΪ0������TP[].Tip_Switch
	for(i=0;i!=MAX_POINT;i++)
	{
		if((id & (1<<i)) == 0)
		{
			TP[i].Tip_Switch = 0;			
		}
	}
	CTP_Info.Press_Flag = id;                                //���±�־λ��

	return 1;
}

#endif
