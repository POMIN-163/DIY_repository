/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10
* Description        : 用CH554，模拟Multi Touch设备实现多指触控功能
*******************************************************************************/

#include "GT911.H"
#include "FT6236.H"
#include "CYPRESS.H"
#include "ILI2511.H"
#include "DEBUG.H"
#include "DEVICE.H"
#include "CONFIG.H"


/* 设备结构体信息定义 */
POINTER TP[ 10 ] = { 0 };                          /* 按照最大支持的定义 */
CTP_INFO CTP_Info = { 0 };
UINT8 ShareBuf[256];


/*芯片唯一ID地址 */
#define     LAST      (*(UINT16 code *)(0x3FFA))   /* 0x3FFB 是保留字 */
#define     FIRST     (*(UINT16 code *)(0x3FFC))
#define     SECOND    (*(UINT16 code *)(0x3FFE))

/************************* 字符串描述符可修改 **********************************/
// 厂家信息
UINT8	MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// 产品信息
UINT8	MyProdInfo[] = 
{
	0x28, 0x03, 0x55,0x00,0x53,0x00,0x42,0x00,0x32,0x00,0x49,0x00,0x49,0x00,0x43,0x00,
	0x5F, 0x00, 0x43,0x00,0x54,0x00,0x50,0x00,0x5F,0x00,0x43,0x00,0x4F,0x00,0x4E,0x00,
	0x54, 0x00, 0x52,0x00,0x4F,0x00,0x4C,0x00,
};
UINT8  MyManuInfo_Len;
UINT8  MyProdInfo_Len;


/*******************************************************************************
* Function Name  : main
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void main( void )
{
    CfgFsys( );                                                           	/* CH554时钟选择配置 */
    mDelaymS(5);                                                          	/* 修改主频等待内部晶振稳定,必加 */
    mInitSTDIO( );                                                        	/* 串口0, P31调试信息输出 9600bps */
#if DE_PRINTF
    printf("CH554_HID_TP_V100 DEBUG\n DEBUG_DATA: "__DATE__""__TIME__" \n");
	printf("chip unique id:%02x %02x %02x\n",FIRST,SECOND,LAST);
#endif
	/* 结构体参数初始化 */
	memset(TP,0,sizeof(TP));
	CTP_Info.x_max_pos = 0;
	CTP_Info.y_max_pos = 0;	
	CTP_Info.IRQ_Flag = 0;
	
#ifdef USE_GT_DEVICE	
	GT911_Init();
#endif
#ifdef USE_FT_DEVICE	
	FT_Init();
#endif
#ifdef USE_CY_DEVICE	
	CY_Init();
#endif
#ifdef USE_ILI_DEVICE 
	ILI_Init();
#endif
#if 1
	MAX_POINT = 6;                                                         /* 默认6个点，当然也可以配置最大10点,但是不支持XP */
#endif

#if 0
	CTP_VID = 0x2345;                                                      /* 此处可用于修改 VID PID，如不修改使用默认值1A86 E2E3 */
	CTP_PID = 0x6789;
#endif
	
	MyManuInfo_Len = sizeof(MyManuInfo);
	MyProdInfo_Len = sizeof(MyProdInfo);
	
    USBDeviceInit();                                                     	/* USB设备模式初始化 */
	EA = 1;                                                              	/* 允许单片机中断 */
	
	while( USB_Enum_OK == 0 );                                              /* 等待USB枚举完成 */
	LED_IO = 0;                                                             /* LED亮 */
	IT0 = 1;
	EX0 = 1;                                                                /* 开启外部下降沿中断,启动检测中断 */
	
	CH554WDTModeSelect(1);                                                  /* 开启单片机内部看门狗 */
    while(1)
    {
		CH554WDTFeed(0);
#ifdef USE_GT_DEVICE			
		GT911_Touch_Check();
#endif
#ifdef USE_FT_DEVICE			
		FT_Touch_Check();
#endif
#ifdef USE_CY_DEVICE		
		CY_Touch_Check(); 
#endif
#ifdef USE_ILI_DEVICE 
		ILI_Touch_Check();
#endif
    }
}

/*******************************************************************************
* Function Name  : CTP_ISR
* Description    : 外部中断0
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  CTP_ISR( void ) interrupt INT_NO_INT0 using 1                         /* USB中断服务程序,使用寄存器组1 */
{
	EX0 = 0;
	CTP_Info.IRQ_Flag = 1;
}
