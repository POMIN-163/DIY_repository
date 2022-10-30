/********************************** (C) COPYRIG  HT *******************************
CH552eģ��USB��ý�����,��ת������Ϊ  EC11
2020/4/9
HID�����������ο����£�https://www.cnblogs.com/AlwaysOnLines/p/4552840.html
�ο��鼮��ȦȦ������USB��HID��;��1.12��HID1.11Э��
���ºܶ඼�Ǹ��ƴ��еĳ��򣬼���Щ�Լ������ע��
*******************************************************************************/

#include "./Public/CH552.H"
#include "./Public/DEBUG.H"
#include "keys.h"
#include <string.h>
#include <stdio.h>

#define THIS_ENDP0_SIZE  DEFAULT_ENDP0_SIZE
#define CapsLockLED 0x02

UINT8X  Ep0Buffer[8>(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;    //�˵�0 OUT&IN������,������ż��ַ
UINT8X  Ep1Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x000a;  //�˵�1 IN������,������ż��ַ
UINT8X  Ep2Buffer[64<(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x0050;  //�˵�2 IN������,������ż��ַ

UINT8   SetupReq,SetupLen,Ready,Count,FLAG,UsbConfig,LED_VALID;
PUINT8  pDescr;   //USB���ñ�־
bit Ep2InKey;
USB_SETUP_REQ   SetupReqBuf;  //�ݴ�Setup��

#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)
#define DEBUG 0
#pragma NOAREGS

/*��ͨ��������*/
UINT8 HIDKey[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*��ý�尴������*/
UINT8 HIDKeyMUL[4] = {0x00,0x00,0x00,0x00};

/*HID�౨��������*/
UINT8C KeyRepDesc[62] =
{
    /******************************************************************
    ���̷��͸�PC������ÿ��8���ֽڣ�BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8������ֱ��ǣ�
    BYTE1 --
        |--bit0:   Left Control
        |--bit1:   Left Shift
        |--bit2:   Left Alt
        |--bit3:   Left GUI
        |--bit4:   Right Control
        |--bit5:   Right Shift
        |--bit6:   Right Alt
        |--bit7:   Right GUI
    BYTE2 -- �ݲ�������еĵط�˵�Ǳ���λ
    BYTE3-BYTE8 -- ������Ϊ��ͨ����
    *******************************************************************/
    0x05, 0x01, 0x09, 0x06, 0xA1, 0x01, 0x05, 0x07,
    0x19, 0xe0, 0x29, 0xe7, 0x15, 0x00, 0x25, 0x01,
    0x75, 0x01, 0x95, 0x08, 0x81, 0x02, 0x95, 0x01,
    0x75, 0x08, 0x81, 0x01, 0x95, 0x03, 0x75, 0x01,
    0x05, 0x08, 0x19, 0x01, 0x29, 0x03, 0x91, 0x02,
    0x95, 0x05, 0x75, 0x01, 0x91, 0x01, 0x95, 0x06,
    0x75, 0x08, 0x26, 0xff, 0x00, 0x05, 0x07, 0x19,
    0x00, 0x29, 0x91, 0x81, 0x00, 0xC0
};
/*��ý����̱���������*/
UINT8C KeyMULRepDesc[105] =
{
/**********************************************************************************************
���̷��͸�PC������ÿ��4���ֽڣ�BYTE1 BYTE2 BYTE3 BYTE4
BYTE1 BYTE2 BYTE3 ��3���ֽڷֳ�24λ��ÿ��λ����һ��������1�����£�0̧��
BYTE1 --
       |--bit0:  Vol-
       |--bit1:  Vol+
       |--bit2:  Mute
       |--bit3:  Email
       |--bit4:  Media
       |--bit5:  WWW Home
       |--bit6:  Play/Pause
       |--bit7:  Scan Pre Track
BYTE2 BYTE3�������˳������ȥ��BYTE3 bit7�����һ��Usage( NULL )��
BYTE4 --
    ϵͳ���ܰ������ػ�(0x81)������(0x82�������ѣ�0x83��
***********************************************************************************************/
    0x05, 0x0C, //USAGE_PAGE ��;ҳѡ��0x0c(�û�ҳ)
    0x09, 0x01, //USAGE ��������Ӧ�ü��������û�����
    0xA1, 0x01, //COLLECTION ������
        0x15, 0x00, //LOGICAL_MINIMUM (0)
        0x25, 0x01, //LOGICAL_MAXIMUM (1)
        0x0A, 0xEA, 0x00,		/* Usage( Vol- ) */
        0x0A, 0xE9, 0x00,		/* Usage( Vol+ ) */
        0x0A, 0xE2, 0x00,		/* Usage( Mute ) */
        0x0A, 0x8A, 0x01,		/* Usage( Email ) */
        0x0A, 0x83, 0x01,		/* Usage( Media ) */
        0x0A, 0x23, 0x02,		/* Usage( WWW Home ) */
        0x0A, 0xCD, 0x00,		/* Usage( Play/Pause ) */
        0x0A, 0xB7, 0x00,		/* Usage( Stop ) */
        0x0A, 0xB6, 0x00,		/* Usage( Scan Pre Track ) */
        0x0A, 0xB5, 0x00,		/* Usage( Scan Next Track ) */
        0x0A, 0x6F, 0x00,		/* Usage( light+ ) */
        0x0A, 0x70, 0x00,		/* Usage( light- ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x00, 0x00,		/* Usage( NULL ) */
        0x0A, 0x11, 0x22,		/* Usage( NULL ) */
        0x75, 0x01, //REPORT_SIZE (1)
        0x95, 0x18, //REPORT_COUNT (24)
        0x81, 0x02, //INPUT (Data,Var,Abs)����24bit����
        0x05, 0x01, //USAGE_PAGE ��;ҳ0x01(��ͨ����)
            0x19, 0x00, //USAGE_MINIMUM ��;��Сֵ0x00(δ����)
            0x29, 0x83, //USAGE_MAXIMUM ��;���ֵ0x83(ϵͳ����)
            0x15, 0x00, //LOGICAL_MINIMUM (0)
            0x25, 0x83, //LOGICAL_MAXIMUM (83)
            0x75, 0x08, //REPORT_SIZE (8)
            0x95, 0x01, //REPORT_COUNT (1)
            0x81, 0x00, //INPUT (Data,Ary,Abs)����1�ֽ�����
    0xC0//END_COLLECTION �պϼ���
};
/*�豸������*/
UINT8C DevDesc[18] = {
   0x12,      //bLength�ֶΡ��豸�������ĳ���Ϊ18(0x12)�ֽ�
   0x01,	  //bDescriptorType�ֶΡ��豸�������ı��Ϊ0x01
   0x10,0x01, //bcdUSB�ֶΡ��������ð汾ΪUSB1.1����0x0110��
              //������С�˽ṹ�����Ե��ֽ����ȣ���0x10��0x01��
   0x00,	  //bDeviceClass�ֶΡ����ǲ����豸�������ж����豸�࣬
              //���ڽӿ��������ж����豸�࣬���Ը��ֶε�ֵΪ0��
   0x00,	  //bDeviceSubClass�ֶΡ�bDeviceClass�ֶ�Ϊ0ʱ�����ֶ�ҲΪ0��
   0x00,	  //bDeviceProtocol�ֶΡ�bDeviceClass�ֶ�Ϊ0ʱ�����ֶ�ҲΪ0��
   0x08,	  //bMaxPacketSize0�ֶΡ� �Ķ˵�0��С��8�ֽڡ�
   0x3d,0x41, //idVender�ֶ�,ע��С��ģʽ�����ֽ����ȡ�
   0x3a,0x55, //idProduct�ֶ� ��ƷID�š�ע��С��ģʽ�����ֽ�Ӧ����ǰ��
   0x00,0x00, //bcdDevice�ֶΡ�ע��С��ģʽ�����ֽ�Ӧ����ǰ��
   0x00,	  //iManufacturer�ֶΡ������ַ���������
   0x00,	  //iProduct�ֶΡ���Ʒ�ַ���������ֵ,ע���ַ�������ֵ��Ҫʹ����ͬ��ֵ��
   0x00,	  //iSerialNumber�ֶΡ��豸�����к��ַ�������ֵ��
   0x01		  //bNumConfigurations�ֶΡ����豸�����е���������
};
/*����������*/
UINT8C CfgDesc[59] =
{
 /*����������*/
    0x09, //bLength�ֶΡ������������ĳ���Ϊ9�ֽ�
    0x02, //bDescriptorType�ֶΡ��������������Ϊ0x02
    0x3b, //wTotalLength�ֶΡ��������������ϵ��ܳ���0x003b���������������������ӿ��������������������˵��������ȣ�LSB
    0x00,
    0x02, //bNumInterfaces�ֶΡ������ð����Ľӿ�����ֻ��2���ӿ�
    0x01, //bConfiguration�ֶΡ������õ�ֵΪ1
    0x01, //iConfigurationz�ֶΣ������õ��ַ���������
    0xA0, //bmAttributes�ֶ�,bit4-bit7�����豸������
    0x64, //bMaxPower�ֶΣ����豸��Ҫ������������ÿ��λ����Ϊ 2 mA
 /*�ӿ�������*/
    //�ӿ�1����ͨ����
    0x09,0x04,0x00,0x00,0x01,0x03,0x01,0x01,0x00, //�ӿ�������,����  HID�豸�Ķ�������ڽӿ���������
    0x09,0x21,0x11,0x01,0x00,0x01,0x22,0x3e,0x00, //HID��������
    0x07,0x05,0x81,0x03,0x08,0x00,0x0a, //�˵�������
    //�ӿ�2����ý�尴��
    0x09,0x04,0x01,0x00,0x01,0x03,0x00,0x00,0x00, // �ӿ�������
    0x09,0x21,0x00,0x01,0x00,0x01,0x22,0x69,0x00, // HID��������
    0x07,0x05,0x82,0x03,0x04,0x00,0x0a,	// �˵�������
};
/*******************************************************************************
* Function Name  : USBDeviceInit()
* Description    : USB�豸ģʽ����,�豸ģʽ�������շ��˵����ã��жϿ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceInit()
{
    IE_USB = 0;           // �ر�USBʹ��
    USB_CTRL = 0x00;      // ���趨USB�豸ģʽ
    UEP0_DMA = Ep0Buffer; // �˵�0���ݴ����ַ
    UEP1_DMA = Ep1Buffer; // �˵�1���ݴ����ַ
    UEP2_DMA = Ep2Buffer; // �˵�2���ݴ����ַ
    UEP4_1_MOD = ~(bUEP4_RX_EN | bUEP4_TX_EN |bUEP1_RX_EN | bUEP1_BUF_MOD) | bUEP4_TX_EN;//�˵�1��64�ֽ��շ�������,�˵�0�շ�
    UEP2_3_MOD = UEP2_3_MOD & ~bUEP2_BUF_MOD | bUEP2_TX_EN;
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;  // OUT���񷵻�ACK��IN���񷵻�NAK
    UEP1_CTRL = bUEP_T_TOG | UEP_T_RES_NAK;	    // �˵�1�ֶ���תͬ����־λ��IN���񷵻�NAK
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;  // �˵�2�Զ���תͬ����־λ��IN���񷵻�NAK

    USB_DEV_AD = 0x00;
    UDEV_CTRL = bUD_PD_DIS; // ��ֹDP/DM��������
    USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN; // ����USB�豸��DMA�����ж��ڼ��жϱ�־δ���ǰ�Զ�����NAK
    UDEV_CTRL |= bUD_PORT_EN; // ����USB�˿�
    USB_INT_FG = 0xFF; // ���жϱ�־
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    SAFE_MOD = 1;
    WAKE_CTRL |= bWAK_BY_USB;
    SAFE_MOD = 0;
    IE_USB = 1;
}
/*******************************************************************************
* Function Name  : Enp1IntIn()
* Description    : USB�豸ģʽ�˵�1���ж��ϴ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp1IntIn()
{
    // while(( UEP1_CTRL & MASK_UEP_T_RES ) == UEP_T_RES_ACK);
    memcpy( Ep1Buffer, HIDKey, sizeof(HIDKey)); //�����ϴ�����
    UEP1_T_LEN = sizeof(HIDKey); //�ϴ����ݳ���
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //������ʱ�ϴ����ݲ�Ӧ��ACK
}
/*******************************************************************************
* Function Name  : Enp2IntIn()
* Description    : USB�豸ģʽ�˵�2���ж��ϴ�
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2IntIn( )
{
    // while(( UEP2_CTRL & MASK_UEP_T_RES ) == UEP_T_RES_ACK);
    memcpy( Ep2Buffer, HIDKeyMUL, sizeof(HIDKeyMUL)); //�����ϴ�����
    UEP2_T_LEN = sizeof(HIDKeyMUL); //�ϴ����ݳ���
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //������ʱ�ϴ����ݲ�Ӧ��ACK
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB�жϴ�����
*******************************************************************************/
void DeviceInterrupt(void) interrupt INT_NO_USB using 1                        //USB�жϷ������,ʹ�üĴ�����1
{
    UINT8 len;
    if(UIF_TRANSFER)                                                            //USB������ɱ�־
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
        {
        case UIS_TOKEN_IN | 2:                                                  //endpoint 2# �ж϶˵��ϴ�
            UEP2_T_LEN = 0;                                                     //Ԥʹ�÷��ͳ���һ��Ҫ���
//            UEP1_CTRL ^= bUEP_T_TOG;                                          //����������Զ���ת����Ҫ�ֶ���ת
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //Ĭ��Ӧ��NAK
            FLAG = 1; 															/*������ɱ�־*/
            break;
        case UIS_TOKEN_IN | 1:                                                  //endpoint 1# �ж϶˵��ϴ�
            UEP1_T_LEN = 0;                                                     //Ԥʹ�÷��ͳ���һ��Ҫ���
//            UEP2_CTRL ^= bUEP_T_TOG;                                          //����������Զ���ת����Ҫ�ֶ���ת
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //Ĭ��Ӧ��NAK
            FLAG = 1;                                                           /*������ɱ�־*/
            break;
        case UIS_TOKEN_SETUP | 0:                                               //SETUP����
            len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                if(UsbSetupBuf->wLengthH || SetupLen > 0x7F )
                {
                    SetupLen = 0x7F;    // �����ܳ���
                }
                len = 0;                                                        // Ĭ��Ϊ�ɹ������ϴ�0����
                SetupReq = UsbSetupBuf->bRequest;
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )/* HID������ */
                {
                    switch( SetupReq )
                    {
                        case 0x01://GetReport
                                    break;
                        case 0x02://GetIdle
                                    break;
                        case 0x03://GetProtocol
                                    break;
                        case 0x09://SetReport
                                    break;
                        case 0x0A://SetIdle
                                    break;
                        case 0x0B://SetProtocol
                                    break;
                        default:
                                    len = 0xFF;  								 					            /*���֧��*/
                                    break;
                    }
                }
                else
                {//��׼����
                    switch(SetupReq)                                        //������
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                             //�豸������
                            pDescr = DevDesc;                               //���豸�������͵�Ҫ���͵Ļ�����
                            len = sizeof(DevDesc);
                            break;
                        case 2:                                             //����������
                            pDescr = CfgDesc;                               //���豸�������͵�Ҫ���͵Ļ�����
                            len = sizeof(CfgDesc);
                            break;
                        case 0x22:                                          //����������
                            if(UsbSetupBuf->wIndexL == 0)                   //�ӿ�0����������
                            {
                                pDescr = KeyRepDesc;                        //����׼���ϴ�
                                len = sizeof(KeyRepDesc);
                            }
                            if(UsbSetupBuf->wIndexL == 1)                   //�ӿ�0����������
                            {
                                pDescr = KeyMULRepDesc;                     //����׼���ϴ�
                                len = sizeof(KeyMULRepDesc);
                                Ready = 1;                                  //����и���ӿڣ��ñ�׼λӦ�������һ���ӿ�������ɺ���Ч
                            }
                            else
                            {
                                len = 0xff;                                 //������ֻ��2���ӿڣ���仰����������ִ��
                            }
                            break;
                        default:
                            len = 0xff;                                     //��֧�ֵ�������߳���
                            break;
                        }
                        if ( SetupLen > len )
                        {
                            SetupLen = len;    //�����ܳ���
                        }
                        len = SetupLen >= 8 ? 8 : SetupLen;                  //���δ��䳤��
                        memcpy(Ep0Buffer,pDescr,len);                        //�����ϴ�����
                        SetupLen -= len;
                        pDescr += len;
                        break;
                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                     //�ݴ�USB�豸��ַ
                        break;
                    case USB_GET_CONFIGURATION:
                        Ep0Buffer[0] = UsbConfig;
                        if ( SetupLen >= 1 )
                        {
                            len = 1;
                        }
                        break;
                    case USB_SET_CONFIGURATION:
                        UsbConfig = UsbSetupBuf->wValueL;
                        break;
                    case 0x0A:
                        break;
                    case USB_CLEAR_FEATURE:                                            //Clear Feature
                        if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// �˵�
                        {
                            switch( UsbSetupBuf->wIndexL )
                            {
                            case 0x82:
                                UEP2_CTRL = UEP2_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x81:
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                break;
                            case 0x01:
                                UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_R_TOG | MASK_UEP_R_RES ) | UEP_R_RES_ACK;
                                break;
                            default:
                                len = 0xFF;                                            // ��֧�ֵĶ˵�
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                                // ���Ƕ˵㲻֧��
                        }
                        break;
                    case USB_SET_FEATURE:                                              /* Set Feature */
                        if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )             /* �����豸 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
                            {
                                if( CfgDesc[ 7 ] & 0x20 )
                                {
                                    /* ���û���ʹ�ܱ�־ */
                                }
                                else
                                {
                                    len = 0xFF;                                        /* ����ʧ�� */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                            /* ����ʧ�� */
                            }
                        }
                        else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )        /* ���ö˵� */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
                            {
                                switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* ���ö˵�2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* ���ö˵�2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* ���ö˵�1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                               //����ʧ��
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                   //����ʧ��
                            }
                        }
                        else
                        {
                            len = 0xFF;                                      //����ʧ��
                        }
                        break;
                    case USB_GET_STATUS:
                        Ep0Buffer[0] = 0x00;
                        Ep0Buffer[1] = 0x00;
                        if ( SetupLen >= 2 )
                        {
                            len = 2;
                        }
                        else
                        {
                            len = SetupLen;
                        }
                        break;
                    default:
                        len = 0xff;                                           //����ʧ��
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                   //�����ȴ���
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= 8)                                                //�ϴ����ݻ���״̬�׶η���0���Ȱ�
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//Ĭ�����ݰ���DATA1������Ӧ��ACK
            }
            else
            {
                UEP0_T_LEN = 0;  //��Ȼ��δ��״̬�׶Σ�������ǰԤ���ϴ�0�������ݰ��Է�������ǰ����״̬�׶�
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//Ĭ�����ݰ���DATA1,����Ӧ��ACK
            }
            break;
        case UIS_TOKEN_IN | 0:                                               //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
                len = SetupLen >= 8 ? 8 : SetupLen;                          //���δ��䳤��
                memcpy( Ep0Buffer, pDescr, len );                            //�����ϴ�����
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                     //ͬ����־λ��ת
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                              //״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ���
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
            break;
        case UIS_TOKEN_OUT | 0:  // endpoint0 OUT
            len = USB_RX_LEN;
            if((SetupReq == 0x09)&& (len == 1))
            {
              LED_VALID = Ep0Buffer[0];
            }
            else if((SetupReq == 0x09) && (len == 8)){//SetReport
            }
            UEP0_T_LEN = 0;  //��Ȼ��δ��״̬�׶Σ�������ǰԤ���ϴ�0�������ݰ��Է�������ǰ����״̬�׶�
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;//Ĭ�����ݰ���DATA0,����Ӧ��ACK
            break;
        default:
            break;
        }
        UIF_TRANSFER = 0;                                                 //д0����ж�
    }
    if(UIF_BUS_RST)                                                       //�豸ģʽUSB���߸�λ�ж�
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;                                                 //���жϱ�־
    }
    if (UIF_SUSPEND)                                                     //USB���߹���/�������
    {
        UIF_SUSPEND = 0;
    }
    else {                                                               //������ж�,�����ܷ��������
        USB_INT_FG = 0xFF;                                               //���жϱ�־
    }
}
/**����HIDֵ�ϴ�����**/
void HIDValueHandle() {
    TR0 = 0;                     // ����ǰ�ض�ʱ���ж�
    FLAG = 0;                    // ���USB�жϴ�����ɱ�־��׼�����Ͱ�����������
    Enp1IntIn();                 // USB�豸ģʽ�˵�1���ж��ϴ�
    while(FLAG == 0);            // �ȴ�USB�ж����ݴ������
    FLAG = 0;                    // ���USB�жϴ�����ɱ�־��׼�����Ͱ���̧������
    memset(&HIDKey[0],0,8);      // ��HIDkey��0������0��ʾ����̧��
    Enp1IntIn();                 // USB�豸ģʽ�˵�1���ж��ϴ�
    while(FLAG == 0);            // �ȴ�USB�ж����ݴ������
    TR0 = 1;                     // ������򿪶�ʱ���ж�
}
/**��ý�尴��HIDֵ�ϴ�����**/
void HIDValueHandleMul() {
    TR0 = 0;                     // ����ǰ�ض�ʱ���ж�
    FLAG = 0;                    // ���USB�жϴ�����ɱ�־��׼�����Ͱ�����������
    Enp2IntIn();                 // USB�豸ģʽ�˵�2���ж��ϴ�
    while(FLAG == 0);            // �ȴ�USB�ж����ݴ������
    FLAG = 0;                    // ���USB�жϴ�����ɱ�־��׼�����Ͱ���̧������
    memset(&HIDKeyMUL[0],0,4);   // ��HIDKeyMUL��0������0��ʾ����̧��
    Enp2IntIn();                 // USB�豸ģʽ�˵�2���ж��ϴ�
    while(FLAG == 0);            // �ȴ�USB�ж����ݴ������
    TR0 = 1;                     // ������򿪶�ʱ���ж�
}
