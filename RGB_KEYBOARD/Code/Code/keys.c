/********************************** (C) COPYRIG  HT *******************************
CH552e模拟USB多媒体键盘,旋转编码器为  EC11
2020/4/9
HID报告描述符参考文章：https://www.cnblogs.com/AlwaysOnLines/p/4552840.html
参考书籍：圈圈教你玩USB，HID用途表1.12，HID1.11协议
以下很多都是复制大佬的程序，加了些自己的理解注释
*******************************************************************************/

#include "./Public/CH552.H"
#include "./Public/DEBUG.H"
#include "keys.h"
#include <string.h>
#include <stdio.h>

#define THIS_ENDP0_SIZE  DEFAULT_ENDP0_SIZE
#define CapsLockLED 0x02

UINT8X  Ep0Buffer[8>(THIS_ENDP0_SIZE+2)?8:(THIS_ENDP0_SIZE+2)] _at_ 0x0000;    //端点0 OUT&IN缓冲区,必须是偶地址
UINT8X  Ep1Buffer[64>(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x000a;  //端点1 IN缓冲区,必须是偶地址
UINT8X  Ep2Buffer[64<(MAX_PACKET_SIZE+2)?64:(MAX_PACKET_SIZE+2)] _at_ 0x0050;  //端点2 IN缓冲区,必须是偶地址

UINT8   SetupReq,SetupLen,Ready,Count,FLAG,UsbConfig,LED_VALID;
PUINT8  pDescr;   //USB配置标志
bit Ep2InKey;
USB_SETUP_REQ   SetupReqBuf;  //暂存Setup包

#define UsbSetupBuf     ((PUSB_SETUP_REQ)Ep0Buffer)
#define DEBUG 0
#pragma NOAREGS

/*普通键盘数据*/
UINT8 HIDKey[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
/*多媒体按键数据*/
UINT8 HIDKeyMUL[4] = {0x00,0x00,0x00,0x00};

/*HID类报表描述符*/
UINT8C KeyRepDesc[62] =
{
    /******************************************************************
    键盘发送给PC的数据每次8个字节：BYTE1 BYTE2 BYTE3 BYTE4 BYTE5 BYTE6 BYTE7 BYTE8。定义分别是：
    BYTE1 --
        |--bit0:   Left Control
        |--bit1:   Left Shift
        |--bit2:   Left Alt
        |--bit3:   Left GUI
        |--bit4:   Right Control
        |--bit5:   Right Shift
        |--bit6:   Right Alt
        |--bit7:   Right GUI
    BYTE2 -- 暂不清楚，有的地方说是保留位
    BYTE3-BYTE8 -- 这六个为普通按键
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
/*多媒体键盘报表描述符*/
UINT8C KeyMULRepDesc[105] =
{
/**********************************************************************************************
键盘发送给PC的数据每次4个字节：BYTE1 BYTE2 BYTE3 BYTE4
BYTE1 BYTE2 BYTE3 这3个字节分成24位，每个位代表一个按键，1代表按下，0抬起。
BYTE1 --
       |--bit0:  Vol-
       |--bit1:  Vol+
       |--bit2:  Mute
       |--bit3:  Email
       |--bit4:  Media
       |--bit5:  WWW Home
       |--bit6:  Play/Pause
       |--bit7:  Scan Pre Track
BYTE2 BYTE3按下面的顺序排下去，BYTE3 bit7：最后一个Usage( NULL )。
BYTE4 --
    系统功能按键，关机(0x81)，休眠(0x82），唤醒（0x83）
***********************************************************************************************/
    0x05, 0x0C, //USAGE_PAGE 用途页选择0x0c(用户页)
    0x09, 0x01, //USAGE 接下来的应用集合用于用户控制
    0xA1, 0x01, //COLLECTION 开集合
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
        0x81, 0x02, //INPUT (Data,Var,Abs)输入24bit数据
        0x05, 0x01, //USAGE_PAGE 用途页0x01(普通桌面)
            0x19, 0x00, //USAGE_MINIMUM 用途最小值0x00(未定义)
            0x29, 0x83, //USAGE_MAXIMUM 用途最大值0x83(系统唤醒)
            0x15, 0x00, //LOGICAL_MINIMUM (0)
            0x25, 0x83, //LOGICAL_MAXIMUM (83)
            0x75, 0x08, //REPORT_SIZE (8)
            0x95, 0x01, //REPORT_COUNT (1)
            0x81, 0x00, //INPUT (Data,Ary,Abs)输入1字节数据
    0xC0//END_COLLECTION 闭合集合
};
/*设备描述符*/
UINT8C DevDesc[18] = {
   0x12,      //bLength字段。设备描述符的长度为18(0x12)字节
   0x01,	  //bDescriptorType字段。设备描述符的编号为0x01
   0x10,0x01, //bcdUSB字段。这里设置版本为USB1.1，即0x0110。
              //由于是小端结构，所以低字节在先，即0x10，0x01。
   0x00,	  //bDeviceClass字段。我们不在设备描述符中定义设备类，
              //而在接口描述符中定义设备类，所以该字段的值为0。
   0x00,	  //bDeviceSubClass字段。bDeviceClass字段为0时，该字段也为0。
   0x00,	  //bDeviceProtocol字段。bDeviceClass字段为0时，该字段也为0。
   0x08,	  //bMaxPacketSize0字段。 的端点0大小的8字节。
   0x3d,0x41, //idVender字段,注意小端模式，低字节在先。
   0x3a,0x55, //idProduct字段 产品ID号。注意小端模式，低字节应该在前。
   0x00,0x00, //bcdDevice字段。注意小端模式，低字节应该在前。
   0x00,	  //iManufacturer字段。厂商字符串的索引
   0x00,	  //iProduct字段。产品字符串的索引值,注意字符串索引值不要使用相同的值。
   0x00,	  //iSerialNumber字段。设备的序列号字符串索引值。
   0x01		  //bNumConfigurations字段。该设备所具有的配置数。
};
/*配置描述符*/
UINT8C CfgDesc[59] =
{
 /*配置描述符*/
    0x09, //bLength字段。配置描述符的长度为9字节
    0x02, //bDescriptorType字段。配置描述符编号为0x02
    0x3b, //wTotalLength字段。配置描述符集合的总长度0x003b，包括配置描述符本身、接口描述符、类描述符、端点描述符等，LSB
    0x00,
    0x02, //bNumInterfaces字段。该配置包含的接口数，只有2个接口
    0x01, //bConfiguration字段。该配置的值为1
    0x01, //iConfigurationz字段，该配置的字符串索引。
    0xA0, //bmAttributes字段,bit4-bit7描述设备的特性
    0x64, //bMaxPower字段，该设备需要的最大电流量。每单位电流为 2 mA
 /*接口描述符*/
    //接口1，普通键盘
    0x09,0x04,0x00,0x00,0x01,0x03,0x01,0x01,0x00, //接口描述符,键盘  HID设备的定义放置在接口描述符中
    0x09,0x21,0x11,0x01,0x00,0x01,0x22,0x3e,0x00, //HID类描述符
    0x07,0x05,0x81,0x03,0x08,0x00,0x0a, //端点描述符
    //接口2，多媒体按键
    0x09,0x04,0x01,0x00,0x01,0x03,0x00,0x00,0x00, // 接口描述符
    0x09,0x21,0x00,0x01,0x00,0x01,0x22,0x69,0x00, // HID类描述符
    0x07,0x05,0x82,0x03,0x04,0x00,0x0a,	// 端点描述符
};
/*******************************************************************************
* Function Name  : USBDeviceInit()
* Description    : USB设备模式配置,设备模式启动，收发端点配置，中断开启
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USBDeviceInit()
{
    IE_USB = 0;           // 关闭USB使能
    USB_CTRL = 0x00;      // 先设定USB设备模式
    UEP0_DMA = Ep0Buffer; // 端点0数据传输地址
    UEP1_DMA = Ep1Buffer; // 端点1数据传输地址
    UEP2_DMA = Ep2Buffer; // 端点2数据传输地址
    UEP4_1_MOD = ~(bUEP4_RX_EN | bUEP4_TX_EN |bUEP1_RX_EN | bUEP1_BUF_MOD) | bUEP4_TX_EN;//端点1单64字节收发缓冲区,端点0收发
    UEP2_3_MOD = UEP2_3_MOD & ~bUEP2_BUF_MOD | bUEP2_TX_EN;
    UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;  // OUT事务返回ACK，IN事务返回NAK
    UEP1_CTRL = bUEP_T_TOG | UEP_T_RES_NAK;	    // 端点1手动翻转同步标志位，IN事务返回NAK
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;  // 端点2自动翻转同步标志位，IN事务返回NAK

    USB_DEV_AD = 0x00;
    UDEV_CTRL = bUD_PD_DIS; // 禁止DP/DM下拉电阻
    USB_CTRL = bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN; // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
    UDEV_CTRL |= bUD_PORT_EN; // 允许USB端口
    USB_INT_FG = 0xFF; // 清中断标志
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    SAFE_MOD = 1;
    WAKE_CTRL |= bWAK_BY_USB;
    SAFE_MOD = 0;
    IE_USB = 1;
}
/*******************************************************************************
* Function Name  : Enp1IntIn()
* Description    : USB设备模式端点1的中断上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp1IntIn()
{
    // while(( UEP1_CTRL & MASK_UEP_T_RES ) == UEP_T_RES_ACK);
    memcpy( Ep1Buffer, HIDKey, sizeof(HIDKey)); //加载上传数据
    UEP1_T_LEN = sizeof(HIDKey); //上传数据长度
    UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //有数据时上传数据并应答ACK
}
/*******************************************************************************
* Function Name  : Enp2IntIn()
* Description    : USB设备模式端点2的中断上传
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Enp2IntIn( )
{
    // while(( UEP2_CTRL & MASK_UEP_T_RES ) == UEP_T_RES_ACK);
    memcpy( Ep2Buffer, HIDKeyMUL, sizeof(HIDKeyMUL)); //加载上传数据
    UEP2_T_LEN = sizeof(HIDKeyMUL); //上传数据长度
    UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_ACK; //有数据时上传数据并应答ACK
}

/*******************************************************************************
* Function Name  : DeviceInterrupt()
* Description    : CH559USB中断处理函数
*******************************************************************************/
void DeviceInterrupt(void) interrupt INT_NO_USB using 1                        //USB中断服务程序,使用寄存器组1
{
    UINT8 len;
    if(UIF_TRANSFER)                                                            //USB传输完成标志
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP))
        {
        case UIS_TOKEN_IN | 2:                                                  //endpoint 2# 中断端点上传
            UEP2_T_LEN = 0;                                                     //预使用发送长度一定要清空
//            UEP1_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP2_CTRL = UEP2_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            FLAG = 1; 															/*传输完成标志*/
            break;
        case UIS_TOKEN_IN | 1:                                                  //endpoint 1# 中断端点上传
            UEP1_T_LEN = 0;                                                     //预使用发送长度一定要清空
//            UEP2_CTRL ^= bUEP_T_TOG;                                          //如果不设置自动翻转则需要手动翻转
            UEP1_CTRL = UEP1_CTRL & ~ MASK_UEP_T_RES | UEP_T_RES_NAK;           //默认应答NAK
            FLAG = 1;                                                           /*传输完成标志*/
            break;
        case UIS_TOKEN_SETUP | 0:                                               //SETUP事务
            len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ)))
            {
                SetupLen = UsbSetupBuf->wLengthL;
                if(UsbSetupBuf->wLengthH || SetupLen > 0x7F )
                {
                    SetupLen = 0x7F;    // 限制总长度
                }
                len = 0;                                                        // 默认为成功并且上传0长度
                SetupReq = UsbSetupBuf->bRequest;
                if ( ( UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK ) != USB_REQ_TYP_STANDARD )/* HID类命令 */
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
                                    len = 0xFF;  								 					            /*命令不支持*/
                                    break;
                    }
                }
                else
                {//标准请求
                    switch(SetupReq)                                        //请求码
                    {
                    case USB_GET_DESCRIPTOR:
                        switch(UsbSetupBuf->wValueH)
                        {
                        case 1:                                             //设备描述符
                            pDescr = DevDesc;                               //把设备描述符送到要发送的缓冲区
                            len = sizeof(DevDesc);
                            break;
                        case 2:                                             //配置描述符
                            pDescr = CfgDesc;                               //把设备描述符送到要发送的缓冲区
                            len = sizeof(CfgDesc);
                            break;
                        case 0x22:                                          //报表描述符
                            if(UsbSetupBuf->wIndexL == 0)                   //接口0报表描述符
                            {
                                pDescr = KeyRepDesc;                        //数据准备上传
                                len = sizeof(KeyRepDesc);
                            }
                            if(UsbSetupBuf->wIndexL == 1)                   //接口0报表描述符
                            {
                                pDescr = KeyMULRepDesc;                     //数据准备上传
                                len = sizeof(KeyMULRepDesc);
                                Ready = 1;                                  //如果有更多接口，该标准位应该在最后一个接口配置完成后有效
                            }
                            else
                            {
                                len = 0xff;                                 //本程序只有2个接口，这句话正常不可能执行
                            }
                            break;
                        default:
                            len = 0xff;                                     //不支持的命令或者出错
                            break;
                        }
                        if ( SetupLen > len )
                        {
                            SetupLen = len;    //限制总长度
                        }
                        len = SetupLen >= 8 ? 8 : SetupLen;                  //本次传输长度
                        memcpy(Ep0Buffer,pDescr,len);                        //加载上传数据
                        SetupLen -= len;
                        pDescr += len;
                        break;
                    case USB_SET_ADDRESS:
                        SetupLen = UsbSetupBuf->wValueL;                     //暂存USB设备地址
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
                        if ( ( UsbSetupBuf->bRequestType & USB_REQ_RECIP_MASK ) == USB_REQ_RECIP_ENDP )// 端点
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
                                len = 0xFF;                                            // 不支持的端点
                                break;
                            }
                        }
                        else
                        {
                            len = 0xFF;                                                // 不是端点不支持
                        }
                        break;
                    case USB_SET_FEATURE:                                              /* Set Feature */
                        if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x00 )             /* 设置设备 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x01 )
                            {
                                if( CfgDesc[ 7 ] & 0x20 )
                                {
                                    /* 设置唤醒使能标志 */
                                }
                                else
                                {
                                    len = 0xFF;                                        /* 操作失败 */
                                }
                            }
                            else
                            {
                                len = 0xFF;                                            /* 操作失败 */
                            }
                        }
                        else if( ( UsbSetupBuf->bRequestType & 0x1F ) == 0x02 )        /* 设置端点 */
                        {
                            if( ( ( ( UINT16 )UsbSetupBuf->wValueH << 8 ) | UsbSetupBuf->wValueL ) == 0x00 )
                            {
                                switch( ( ( UINT16 )UsbSetupBuf->wIndexH << 8 ) | UsbSetupBuf->wIndexL )
                                {
                                case 0x82:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点2 IN STALL */
                                    break;
                                case 0x02:
                                    UEP2_CTRL = UEP2_CTRL & (~bUEP_R_TOG) | UEP_R_RES_STALL;/* 设置端点2 OUT Stall */
                                    break;
                                case 0x81:
                                    UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;/* 设置端点1 IN STALL */
                                    break;
                                default:
                                    len = 0xFF;                               //操作失败
                                    break;
                                }
                            }
                            else
                            {
                                len = 0xFF;                                   //操作失败
                            }
                        }
                        else
                        {
                            len = 0xFF;                                      //操作失败
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
                        len = 0xff;                                           //操作失败
                        break;
                    }
                }
            }
            else
            {
                len = 0xff;                                                   //包长度错误
            }
            if(len == 0xff)
            {
                SetupReq = 0xFF;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;//STALL
            }
            else if(len <= 8)                                                //上传数据或者状态阶段返回0长度包
            {
                UEP0_T_LEN = len;
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1，返回应答ACK
            }
            else
            {
                UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA1,返回应答ACK
            }
            break;
        case UIS_TOKEN_IN | 0:                                               //endpoint0 IN
            switch(SetupReq)
            {
            case USB_GET_DESCRIPTOR:
                len = SetupLen >= 8 ? 8 : SetupLen;                          //本次传输长度
                memcpy( Ep0Buffer, pDescr, len );                            //加载上传数据
                SetupLen -= len;
                pDescr += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                                     //同步标志位翻转
                break;
            case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            default:
                UEP0_T_LEN = 0;                                              //状态阶段完成中断或者是强制上传0长度数据包结束控制传输
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
            UEP0_T_LEN = 0;  //虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
            UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_ACK;//默认数据包是DATA0,返回应答ACK
            break;
        default:
            break;
        }
        UIF_TRANSFER = 0;                                                 //写0清空中断
    }
    if(UIF_BUS_RST)                                                       //设备模式USB总线复位中断
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP1_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;                                                 //清中断标志
    }
    if (UIF_SUSPEND)                                                     //USB总线挂起/唤醒完成
    {
        UIF_SUSPEND = 0;
    }
    else {                                                               //意外的中断,不可能发生的情况
        USB_INT_FG = 0xFF;                                               //清中断标志
    }
}
/**键盘HID值上传函数**/
void HIDValueHandle() {
    TR0 = 0;                     // 发送前关定时器中断
    FLAG = 0;                    // 清空USB中断传输完成标志，准备发送按键按下数据
    Enp1IntIn();                 // USB设备模式端点1的中断上传
    while(FLAG == 0);            // 等待USB中断数据传输完成
    FLAG = 0;                    // 清空USB中断传输完成标志，准备发送按键抬起数据
    memset(&HIDKey[0],0,8);      // 把HIDkey置0，发送0表示按键抬起
    Enp1IntIn();                 // USB设备模式端点1的中断上传
    while(FLAG == 0);            // 等待USB中断数据传输完成
    TR0 = 1;                     // 发送完打开定时器中断
}
/**多媒体按键HID值上传函数**/
void HIDValueHandleMul() {
    TR0 = 0;                     // 发送前关定时器中断
    FLAG = 0;                    // 清空USB中断传输完成标志，准备发送按键按下数据
    Enp2IntIn();                 // USB设备模式端点2的中断上传
    while(FLAG == 0);            // 等待USB中断数据传输完成
    FLAG = 0;                    // 清空USB中断传输完成标志，准备发送按键抬起数据
    memset(&HIDKeyMUL[0],0,4);   // 把HIDKeyMUL置0，发送0表示按键抬起
    Enp2IntIn();                 // USB设备模式端点2的中断上传
    while(FLAG == 0);            // 等待USB中断数据传输完成
    TR0 = 1;                     // 发送完打开定时器中断
}
