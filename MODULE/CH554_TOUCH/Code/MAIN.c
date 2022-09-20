/********************************** (C) COPYRIGHT *******************************
* File Name          : MAIN.C
* Author             : GJ
* Version            : V2.00
* Date               : 2018-7-10
* Description        : ��CH554��ģ��Multi Touch�豸ʵ�ֶ�ָ���ع���
*******************************************************************************/

#include "GT911.H"
#include "FT6236.H"
#include "CYPRESS.H"
#include "ILI2511.H"
#include "DEBUG.H"
#include "DEVICE.H"
#include "CONFIG.H"


/* �豸�ṹ����Ϣ���� */
POINTER TP[ 10 ] = { 0 };                          /* �������֧�ֵĶ��� */
CTP_INFO CTP_Info = { 0 };
UINT8 ShareBuf[256];


/*оƬΨһID��ַ */
#define     LAST      (*(UINT16 code *)(0x3FFA))   /* 0x3FFB �Ǳ����� */
#define     FIRST     (*(UINT16 code *)(0x3FFC))
#define     SECOND    (*(UINT16 code *)(0x3FFE))

/************************* �ַ������������޸� **********************************/
// ������Ϣ
UINT8	MyManuInfo[] = { 0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0 };
// ��Ʒ��Ϣ
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
    CfgFsys( );                                                           	/* CH554ʱ��ѡ������ */
    mDelaymS(5);                                                          	/* �޸���Ƶ�ȴ��ڲ������ȶ�,�ؼ� */
    mInitSTDIO( );                                                        	/* ����0, P31������Ϣ��� 9600bps */
#if DE_PRINTF
    printf("CH554_HID_TP_V100 DEBUG\n DEBUG_DATA: "__DATE__""__TIME__" \n");
	printf("chip unique id:%02x %02x %02x\n",FIRST,SECOND,LAST);
#endif
	/* �ṹ�������ʼ�� */
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
	MAX_POINT = 6;                                                         /* Ĭ��6���㣬��ȻҲ�����������10��,���ǲ�֧��XP */
#endif

#if 0
	CTP_VID = 0x2345;                                                      /* �˴��������޸� VID PID���粻�޸�ʹ��Ĭ��ֵ1A86 E2E3 */
	CTP_PID = 0x6789;
#endif
	
	MyManuInfo_Len = sizeof(MyManuInfo);
	MyProdInfo_Len = sizeof(MyProdInfo);
	
    USBDeviceInit();                                                     	/* USB�豸ģʽ��ʼ�� */
	EA = 1;                                                              	/* ����Ƭ���ж� */
	
	while( USB_Enum_OK == 0 );                                              /* �ȴ�USBö����� */
	LED_IO = 0;                                                             /* LED�� */
	IT0 = 1;
	EX0 = 1;                                                                /* �����ⲿ�½����ж�,��������ж� */
	
	CH554WDTModeSelect(1);                                                  /* ������Ƭ���ڲ����Ź� */
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
* Description    : �ⲿ�ж�0
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  CTP_ISR( void ) interrupt INT_NO_INT0 using 1                         /* USB�жϷ������,ʹ�üĴ�����1 */
{
	EX0 = 0;
	CTP_Info.IRQ_Flag = 1;
}
