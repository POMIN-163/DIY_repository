/**
 * @author: Pomin
 * @date: 2022-12-09 23:00:00
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-12 16:51:36
 * @description:
 **/
#include "common.h"

/* 取高字节 */
#define HBYTE(c)    (((c) >> 8) & 0xff)
/* 取低字节 */
#define LBYTE(c)    ((c)&0xff)
/* 小端模式下的半字转字节数组 */
#define TO2BYTES(c) LBYTE((c)), HBYTE((c))

/* VID & PID & 版本号 */
#define VID         (0X3062)
#define PID         (0X4702)
#define VER         (0X0120)

/* 端点大小 */
#define EP0_SIZE    (64)
#define EP2_SIZE    (64)

/* 端点缓冲区 */
UINT8X ep0_buff[66] _at_(0);
UINT8X ep2_buff[66] _at_(66);

/* 一些标志 & 请求参数 */
UINT8 SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;
PUINT8 pDescr;

#define UsbSetupBuf ((PUSB_SETUP_REQ)ep0_buff)  // Setup 包

/* 报告描述符 */
char code desc_report[] = {
    /* keyboared */
    0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,  // USAGE (Keyboard)
    0xa1, 0x01,  // COLLECTION (Application)
    0x85, 0x01,  // Report ID (1) <-- 之后的上报键值依赖此 ID
    0x05, 0x07,  // USAGE_PAGE (Keyboard)
    0x19, 0xe0,  // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7,  // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00,  // LOGICAL_MINIMUM (0)
    0x25, 0x01,  // LOGICAL_MAXIMUM (1)
    0x75, 0x01,  // REPORT_SIZE (1)
    0x95, 0x08,  // REPORT_COUNT (8)
    0x81, 0x02,  // INPUT (Data,Var,Abs)
    0x95, 0x01,  // REPORT_COUNT (1)
    0x75, 0x08,  // REPORT_SIZE (8)
    0x81, 0x03,  // INPUT (Cnst,Var,Abs)
    0x95, 0x05,  // REPORT_COUNT (5)
    0x75, 0x01,  // REPORT_SIZE (1)
    0x05, 0x08,  // USAGE_PAGE (LEDs)
    0x19, 0x01,  // USAGE_MINIMUM (Num Lock)
    0x29, 0x05,  // USAGE_MAXIMUM (Kana)
    0x91, 0x02,  // OUTPUT (Data,Var,Abs)
    0x95, 0x01,  // REPORT_COUNT (1)
    0x75, 0x03,  // REPORT_SIZE (3)
    0x91, 0x03,  // OUTPUT (Cnst,Var,Abs)
    0x95, 0x06,  // REPORT_COUNT (6)
    0x75, 0x08,  // REPORT_SIZE (8)
    0x15, 0x00,  // LOGICAL_MINIMUM (0)
    0x25, 0xFF,  // LOGICAL_MAXIMUM (255)
    0x05, 0x07,  // USAGE_PAGE (Keyboard)
    0x19, 0x00,  // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x65,  // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00,  // INPUT (Data,Ary,Abs)
    0xc0,        // END_COLLECTION    /* 65 */

    /* consumer */
    0x05, 0x0c,        // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,        // USAGE (Consumer Control)
    0xa1, 0x01,        // COLLECTION (Application)
    0x85, 0x02,        // REPORT_ID (2) <-- 之后的上报键值依赖此 ID
    0x19, 0x00,        // USAGE_MINIMUM (Unassigned)
    0x2a, 0x3c, 0x03,  // USAGE_MAXIMUM
    0x15, 0x00,        // LOGICAL_MINIMUM (0)
    0x26, 0x3c, 0x03,  // LOGICAL_MAXIMUM (828)
    0x95, 0x01,        // REPORT_COUNT (1)
    0x75, 0x18,        // REPORT_SIZE (24)
    0x81, 0x00,        // INPUT (Data,Ary,Abs)
    0x05, 0x01,        // USAGE_PAGE 用途页0x01(普通桌面)
    0x19, 0x00,        // USAGE_MINIMUM 用途最小值0x00(未定义)
    0x29, 0x83,        // USAGE_MAXIMUM 用途最大值0x83(系统唤醒)
    0x15, 0x00,        // LOGICAL_MINIMUM (0)
    0x25, 0x83,        // LOGICAL_MAXIMUM (83)
    0x75, 0x08,        // REPORT_SIZE (8)
    0x95, 0x01,        // REPORT_COUNT (1)
    0x81, 0x00,        // INPUT (Data,Ary,Abs)输入1字节数据    
    0xc0,              // END_COLLECTION  /* 25 */

    /* dial */
    0x05, 0x01,  // USAGE_PAGE
    0x09, 0x0e,  // USAGE (dial)
    0xa1, 0x01,  // COLLECTION (Application)
    0x85, 0x03,  // REPORT_ID (3) <-- 之后的上报键值依赖此 ID
    0x05, 0x0d,  //
    0x09, 0x21,  //
    0xa1, 0x00,  //
    0x05, 0x09,  //
    0x09, 0x01,  //
    0x95, 0x01,  //
    0x75, 0x01,  //
    0x15, 0x00,  //
    0x25, 0x01,  //
    0x81, 0x02,  //
    0x05, 0x01,  //
    0x09, 0x37,  //
    0x95, 0x01,  //
    0x75, 0x0f,  //
    0x55, 0x0f,  //
    0x65, 0x14,  //
    0x36, 0xf0,  //
    0xf1, 0x46,  //
    0x10, 0x0e,  //
    0x16, 0xf0,  //
    0xf1, 0x26,  //
    0x10, 0x0e,  //
    0x81, 0x06,  //
    0xc0, 0xC0,  /* 56 */
};

/* USB 设备描述符 */
char code desc_device[] = {
    0x12,              // bLength         :描述符大小．固定0x12
    0x01,              // bDescriptorType :设备描述符类型．固定0x01
    TO2BYTES(0x0200),  // bcdUSB          :USB版本：1.1--0x0110 2.0--0x0200
    0x00,              // bDeviceClass    :类型代码（由USB指定）
    0x00,              // bDeviceSubClass :子类型代码（由USB分配）
    0x00,              // bDeviceProtocol :协议代码（由USB分配）
    EP0_SIZE,          // bMaxPacketSize0 :端点0最大包的大小，全速64
    TO2BYTES(VID),     // idVendor VID    :供应商ID（由USB分配）
    TO2BYTES(PID),     // idProduct PID   :产品ID（由厂商分配）
    TO2BYTES(VER),  // bcdDevice          :设备出产编码．由厂家自行设置
    0x01,           // iManufacturer      :厂商描述符字符串索引
    0x02,           // iProduct           :产品描述符字符串索引
    0x03,           // iSerialNumber      :设备序列号字符串索引
    0x01,  // bNumConfigurations :配置数．定义设备以当前速度支持的配置数量
};

#define CFG_LEN (9 + 9 + 9 + 7)

/* 配置描述符 */
char code desc_config[CFG_LEN] = {
    /* 配置描述符 */
    0x09,  // bLength         :配置描述符的长度，固定0x09
    0x02,  // bDescriptorType :配置描述符编号，固定0x02
    TO2BYTES(CFG_LEN),  // wTotalLength  :配置描述符集合的总长度
    0x01,               // bNumInterfaces  :该配置包含的接口数
    0x01,  // bConfiguration  :作为Set Configuration的一个参数选择配置值
    0x01,  // iConfigurationz :该配置的字符串索引。
    0xA0,  // bmAttributes    :Bit4-0保留，D7:总线供电，D6:自供电，D5:远程唤醒
    0x5A,  // bMaxPower       :最大电流量。每单位电流 2 mA

    /* 接口描述符 1 */
    0x09,  // bLength            :设备描述符的字节数大小，为0x09
    0x04,  // bDescriptorType    :描述符类型编号，固定为0x04
    0x01,  // bInterfaceNumber   :接口的编号
    0x00,  // bAlternateSetting  :备用的接口描述符编号
    0x01,  // bNumEndpoints      :该接口使用端点数，不包括端点0
    0x03,  // bInterfaceClass    :接口类型 (HID固定为3)
    0x00,  // bInterfaceSubClass :接口子类型
    0x00,  // bInterfaceProtocol :接口所遵循的协议
    0x00,  // iInterface         :描述该接口的字符串索引值

    /* HID 描述符 1 */
    0x09,              // bLength         :描述符字节数
    0x21,              // bDescriptorType :0x21 = HID描述符
    TO2BYTES(0x0111),  // bcdHID          :HID规范版本号（BCD 1.11）
    0x00,  // bCountryCode      :硬件设备所在国家的国家代码
    0x01,  // bNumDescriptors   :类别描述符数目（至少有一个报表描述符）
    0x22,  // bDescriptorType   :该类别描述符的类型 (0x22 键盘)
    TO2BYTES(sizeof(desc_report)),  // wDescriptorLength:该类别描述符的总长度

    /* 端点描述符 1 */
    0x07,  // bLength          :设备描述符的字节数大小，为0x7
    0x05,  // bDescriptorType  :描述符类型编号，为0x05
    0x82,  // bEndpointAddress :端点地址及输入输出属性 IN 端点 2
    0x03,  // bmAttribute      :端点的传输类型属性 (0b11中断)
    TO2BYTES(EP2_SIZE),  // wMaxPacketSize   :端点收、发的最大包的大小
    0x01,                // bInterval        :主机查询端点的时间间隔
};

/* 字符串描述符[UNICODE encoding] */
/* 语言字符串 */
char code desc_lang[] = {0x04, 0x03, 0x09, 0x04};

/* 制造商字符串 */
char code desc_manuf[] = {0x0A, 0x03, 0xCB, 0x7A, 0x1B,
                          0x52, 0x00, 0x5F, 0x90, 0x6E};
/* 产品描述字符串 */
char code desc_produce[] = {
    0x1e, 0x03, 0x70, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x69, 0x00,
    0x6e, 0x00, 0x20, 0x00, 0x6b, 0x00, 0x65, 0x00, 0x79, 0x00,
    0x62, 0x00, 0x6f, 0x00, 0x61, 0x00, 0x72, 0x00, 0x64, 0x00,
};

UINT8X Ser_Des[30];  // 由单片机唯一ID生成的设备序列号字符串

uchar dec2hex(uchar num) { return (num < 10) ? num + '0' : num + '7'; }

// 发送长度为30,前两位为51或52，后面12位为内部唯一ID
void get_id_Serial(uchar *p) {
    uchar code *id;
    uchar i;
    id = ROM_CHIP_ID_HX;

    p[0] = 0x1E;
    p[1] = 0x03;

    p[2] = dec2hex(CHIP_ID >> 4);
    p[3] = 0;
    p[4] = dec2hex(CHIP_ID & 0x0f);
    p[5] = 0;
    for (i = 0; i < 6; i++) {
        p[6 + 4 * i + 0] = dec2hex(id[i] >> 4);
        p[6 + 4 * i + 1] = 0;
        p[6 + 4 * i + 2] = dec2hex(id[i] & 0x0f);
        p[6 + 4 * i + 3] = 0;
    }
}

void drv_usb_init(void) {
    get_id_Serial(Ser_Des);

    SetupReq = 0;
    SetupLen = 0;
    Ready = 0;
    Count = 0;
    UsbConfig = 0;

    IE_USB = 0;       // 关闭USB使能
    USB_CTRL = 0x06;  // 清除USB控制寄存器
    UDEV_CTRL = 0x00;

    USB_CTRL = 0x00;  // 先设定USB设备模式
    UDEV_CTRL = 0x80;
    UDEV_CTRL |= bUD_PD_DIS;  // 禁止DP/DM下拉电阻

    UDEV_CTRL &= ~bUD_LOW_SPEED;  // 选择全速12M模式，默认方式
    USB_CTRL &= ~bUC_LOW_SPEED;

    UEP0_DMA = ep0_buff;  // 端点0数据传输地址
    UEP2_DMA = ep2_buff;  // 端点2数据传输地址

    UEP0_T_LEN = 0;  // 端点0清零
    UEP2_T_LEN = 0;  // 端点2清零

    UEP2_3_MOD |= bUEP2_TX_EN;     // 端点2发送使能
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;  // 端点2单64字节发送缓冲区(IN)
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
    // 端点2自动翻转同步标志位，IN事务返回NAK，OUT返回ACK

    USB_DEV_AD = 0x00;
    USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;
    // 启动USB设备及DMA，在中断期间中断标志未清除前自动返回NAK
    UDEV_CTRL |= bUD_PORT_EN;  // 允许USB端口
    USB_INT_FG = 0xFF;         // 清中断标志
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    IE_USB = 1;
}

/**
 * @brief 上传数据到端点 2
 *
 * @param buf
 * @param len
 **/
void drv_usb_write_ep2(char *buf, unsigned char len) {
    while ((UEP2_CTRL & MASK_UEP_T_RES) == UEP_T_RES_ACK)
        ;
    len = (len > EP2_SIZE) ? EP2_SIZE : len;
    memcpy(ep2_buff, buf, len);  // 加载上传数据
    UEP2_T_LEN = len;            // 上传数据长度
    UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK;
    // 有数据时上传数据并应答ACK
}

void usb_handler(void) interrupt INT_NO_USB using 1 {
    uchar len;

    if (UIF_TRANSFER)  // USB传输完成标志
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP)) {
            case UIS_TOKEN_IN | 2:  // endpoint 2# 端点批量上传
                UEP2_T_LEN = 0;     // 预使用发送长度一定要清空
                UEP2_CTRL =
                    UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;  // 默认应答NAK
                FLAG = 1;
                break;

            case UIS_TOKEN_OUT | 2:  // endpoint 2# 端点批量下传
                if (U_TOG_OK)        // 不同步的数据包将丢弃
                {
                }
                break;

            case UIS_TOKEN_SETUP | 0:  // SETUP事务
                len = USB_RX_LEN;

                if (len == (sizeof(USB_SETUP_REQ))) {
                    if (UsbSetupBuf->wLengthH)  // 请求可能会超255
                        SetupLen = 0xff;
                    else
                        SetupLen = UsbSetupBuf->wLengthL;
                    len = 0;  // 默认为成功并且上传0长度
                    SetupReq = UsbSetupBuf->bRequest;

                    if ((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) !=
                        USB_REQ_TYP_STANDARD) /*HID类命令*/
                    {
                        switch (SetupReq) {
                            case HID_GET_REPORT:

                                break;

                            case HID_GET_IDLE:
                                break;

                            case HID_GET_PROTOCOL:
                                break;

                            case HID_SET_REPORT:

                                break;

                            case HID_SET_IDLE:
                                UEP0_T_LEN = 0;
                                break;

                            case HID_SET_PROTOCOL:
                                break;

                            default:
                                len = 0xFF; /*命令不支持*/
                                break;
                        }

                        if (SetupLen > len) {
                            SetupLen = len;  // 限制总长度
                        }
                    } else  // 标准请求
                    {
                        switch (SetupReq)  // 请求码
                        {
                            case USB_GET_DESCRIPTOR:
                                switch (UsbSetupBuf->wValueH) {
                                    case 1:  // 设备描述符
                                        pDescr =
                                            desc_device;  // 把设备描述符送到要发送的缓冲区
                                        len = sizeof(desc_device);
                                        break;

                                    case 2:  // 配置描述符
                                        pDescr =
                                            desc_config;  // 把设备描述符送到要发送的缓冲区
                                        len = sizeof(desc_config);
                                        break;

                                    case 3:
                                        switch (UsbSetupBuf->wValueL) {
                                            case 0:  // 语言描述符
                                                pDescr = desc_lang;
                                                len = sizeof(desc_lang);
                                                break;
                                            case 1:  // 制造商描述符
                                                pDescr = desc_manuf;
                                                len = sizeof(desc_manuf);
                                                break;
                                            case 2:  // 产品描述字符串
                                                pDescr = desc_produce;
                                                len = sizeof(desc_produce);
                                                break;
                                            default:  // 产品序列号
                                                pDescr = Ser_Des;
                                                len = sizeof(Ser_Des);
                                                break;
                                        }
                                        break;

                                    case 0x22:  // 报表描述符
                                        switch (UsbSetupBuf->wIndexL) {
                                            case 1:
                                                pDescr =
                                                    desc_report;  // 数据准备上传
                                                len = sizeof(desc_report);
                                                break;
                                        }

                                    default:
                                        len = 0xff;  // 不支持的命令或者出错
                                        break;
                                }

                                if (SetupLen > len) {
                                    SetupLen = len;  // 限制总长度
                                }

                                len = SetupLen >= EP0_SIZE
                                          ? EP0_SIZE
                                          : SetupLen;  // 本次传输长度
                                memcpy(ep0_buff, pDescr, len);  // 加载上传数据
                                SetupLen -= len;
                                pDescr += len;
                                break;

                            case USB_SET_ADDRESS:
                                SetupLen =
                                    UsbSetupBuf->wValueL;  // 暂存USB设备地址
                                break;

                            case USB_GET_CONFIGURATION:
                                ep0_buff[0] = UsbConfig;

                                if (SetupLen >= 1) {
                                    len = 1;
                                }

                                break;

                            case USB_SET_CONFIGURATION:
                                UsbConfig = UsbSetupBuf->wValueL;
                                if (UsbConfig) Ready = 1;  // 枚举完成
                                break;

                            case 0x0A:
                                break;

                            case USB_CLEAR_FEATURE:  // Clear Feature
                                if ((UsbSetupBuf->bRequestType &
                                     USB_REQ_RECIP_MASK) ==
                                    USB_REQ_RECIP_ENDP)  // 端点
                                {
                                    switch (UsbSetupBuf->wIndexL) {
                                        case 0x82:
                                            UEP2_CTRL =
                                                UEP2_CTRL & ~(bUEP_T_TOG |
                                                              MASK_UEP_T_RES) |
                                                UEP_T_RES_NAK;
                                            /* 设置端点2 IN  */
                                            break;
                                        case 0x02:
                                            UEP2_CTRL =
                                                UEP2_CTRL & ~(bUEP_T_TOG |
                                                              MASK_UEP_T_RES) |
                                                UEP_T_RES_NAK;
                                            /* 设置端点2 OUT */
                                            break;

                                        default:
                                            len = 0xFF;  // 不支持的端点
                                            break;
                                    }
                                } else {
                                    len = 0xFF;  // 不是端点不支持
                                }

                                break;

                            case USB_SET_FEATURE: /* Set Feature */
                                if ((UsbSetupBuf->bRequestType & 0x1F) ==
                                    0x00) /* 设置设备 */
                                {
                                    if ((((uint)UsbSetupBuf->wValueH << 8) |
                                         UsbSetupBuf->wValueL) == 0x01) {
                                        if (desc_config[7] & 0x20) {
                                            /* 设置唤醒使能标志 */
                                        } else {
                                            len = 0xFF; /* 操作失败 */
                                        }
                                    } else {
                                        len = 0xFF; /* 操作失败 */
                                    }
                                } else if ((UsbSetupBuf->bRequestType & 0x1F) ==
                                           0x02) /* 设置端点 */
                                {
                                    if ((((uint)UsbSetupBuf->wValueH << 8) |
                                         UsbSetupBuf->wValueL) == 0x00) {
                                        switch (
                                            ((uint)UsbSetupBuf->wIndexH << 8) |
                                            UsbSetupBuf->wIndexL) {
                                            case 0x82:
                                                UEP2_CTRL =
                                                    UEP2_CTRL & (~bUEP_T_TOG) |
                                                    UEP_T_RES_STALL; /* 设置端点2
                                                           IN
                                                         */
                                                break;
                                            case 0x02:
                                                UEP2_CTRL =
                                                    UEP2_CTRL & (~bUEP_R_TOG) |
                                                    UEP_R_RES_STALL; /* 设置端点2
                                                           OUT
                                                            */
                                                break;

                                            default:
                                                len = 0xFF; /* 操作失败 */
                                                break;
                                        }
                                    } else {
                                        len = 0xFF; /* 操作失败 */
                                    }
                                } else {
                                    len = 0xFF; /* 操作失败 */
                                }

                                break;

                            case USB_GET_STATUS:
                                ep0_buff[0] = 0x00;
                                ep0_buff[1] = 0x00;

                                if (SetupLen >= 2) {
                                    len = 2;
                                } else {
                                    len = SetupLen;
                                }

                                break;

                            default:
                                len = 0xff;  // 操作失败
                                break;
                        }
                    }
                } else {
                    len = 0xff;  // 包长度错误
                }

                if (len == 0xff) {
                    SetupReq = 0xFF;
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL |
                                UEP_T_RES_STALL;  // STALL
                } else if (len <= EP0_SIZE)  // 上传数据或者状态阶段返回0长度包
                {
                    UEP0_T_LEN = len;
                    UEP0_CTRL =
                        bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK |
                        UEP_T_RES_ACK;  // 默认数据包是DATA1，返回应答ACK
                } else {
                    UEP0_T_LEN =
                        0;  // 虽然尚未到状态阶段，但是提前预置上传0长度数据包以防主机提前进入状态阶段
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK |
                                UEP_T_RES_ACK;  // 默认数据包是DATA1,返回应答ACK
                }

                break;

            case UIS_TOKEN_IN | 0:  // endpoint0 IN
                switch (SetupReq) {
                    case USB_GET_DESCRIPTOR:
                    case HID_GET_REPORT:
                        len = SetupLen >= EP0_SIZE ? EP0_SIZE
                                                   : SetupLen;  // 本次传输长度
                        memcpy(ep0_buff, pDescr, len);  // 加载上传数据
                        SetupLen -= len;
                        pDescr += len;
                        UEP0_T_LEN = len;
                        UEP0_CTRL ^= bUEP_T_TOG;  // 同步标志位翻转
                        break;

                    case USB_SET_ADDRESS:
                        USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                        break;

                    default:
                        UEP0_T_LEN =
                            0;  // 状态阶段完成中断或者是强制上传0长度数据包结束控制传输
                        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                        break;
                }

                break;

            case UIS_TOKEN_OUT | 0:
                if (USB_RX_LEN == 2 &&
                    ep0_buff[0] == 1)  // 接收大小写时固定长度为2
                {
                    if (ep0_buff[1] & 1)  // 开数字键盘LED
                    {
                        num_lock = 1;
                    } else  // 关数字键盘LED
                    {
                        num_lock = 0;
                    }

                    if (ep0_buff[1] & 2)  // 开大写LED
                    {
                        caps_lock = 1;
                    } else  // 关大写LED
                    {
                        caps_lock = 0;
                    }

                    if (ep0_buff[1] & 4)  // 开scrol LED
                    {
                    } else  // 关scrol LED
                    {
                    }
                }
                UEP0_CTRL ^= bUEP_R_TOG;  // 同步标志位翻转
                break;

            default:
                break;
        }

        UIF_TRANSFER = 0;  // 清中断标志
    }

    if (UIF_BUS_RST)  // 设备模式USB总线复位中断
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_ACK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;  // 清中断标志
    }

    if (UIF_SUSPEND)  // USB总线挂起/唤醒完成
    {
        UIF_SUSPEND = 0;
    } else  // 意外的中断,不可能发生的情况
    {
        USB_INT_FG = 0xFF;  // 清中断标志
    }
}
