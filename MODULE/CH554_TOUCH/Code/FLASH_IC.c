/********************************** (C) COPYRIGHT *******************************
* File Name          : FLASH_IC.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018/07/10
* Description        : FLASH Read
*******************************************************************************/


/*******************************************************************************
* configure information:
* Name			Address			Data
* x_resolution  0xc000 0xc002
* y_resolution  0xc004 0xc006
* x_mirror		0xc000 bit7
* y_mirror	    0xc004 bit7
*******************************************************************************/

/* 头文件包含 */
#include "FLASH_IC.H"

UINT8 Flash_Data[ FLASH_DATA_LEN ];
/* Get flash info */
UINT8 Get_Config_Info( void )			
{
	UINT8 i;
    ROM_ADDR_H = DATA_FLASH_ADDR >> 8;
    
    for(i=0; i<FLASH_DATA_LEN; i++)
	{
		ROM_ADDR_L = i*2;                                                 
	  	ROM_CTRL = ROM_CMD_READ;
//     if ( ROM_STATUS & bROM_CMD_ERR ) return( 0xFF );                        // unknown command
	  	*(Flash_Data+i) = ROM_DATA_L;
	
	}
#if DE_PRINTF
	printf("data flash read: ");
	for(i=0; i<FLASH_DATA_LEN; i++)
	{
		printf("%02x\t",(UINT16)Flash_Data[i]);
	}
	printf("\n");
#endif 
	
	if((Flash_Data[0]==0xFF) && (Flash_Data[1]==0xFF))   //空的FLASH
	{
		CTP_Info.x_mirror = 0;
	}
	else
	{
		if( ( Flash_Data[0]&0x80 )!= 0 ) 
		{
			CTP_Info.x_mirror = 1;
			Flash_Data[0] &= 0x7F;
		}
		else
		{
			CTP_Info.x_mirror = 0;
		}
	}
	
	if((Flash_Data[2]==0xFF) && (Flash_Data[3]==0xFF))
	{
		CTP_Info.y_mirror = 0;
	}
	else
	{
		if( ( Flash_Data[2]&0x80 )!= 0 ) 
		{
			CTP_Info.y_mirror = 1;
			Flash_Data[2] &= 0x7F;
		}
		else
		{
			CTP_Info.y_mirror = 0;
		}
	}
	
	//在Flash_Data[5]为0x0a（10）的时候使用flash配置值,否则使用从屏读出的值
	if((Flash_Data[5] == 10 ) || ((CTP_Info.x_max_pos == 0) && (CTP_Info.y_max_pos==0)))
	{
		CTP_Info.x_max_pos = ( (UINT16)(Flash_Data[0])<<8 ) + Flash_Data[1];
		CTP_Info.y_max_pos = ( (UINT16)(Flash_Data[2])<<8 ) + Flash_Data[3];
	}
	
	CTP_Info.x_y_swap = ( Flash_Data[4] == 0x01) ? 1 : 0;

	return 1;
	
}

