/**
 * @author: Pomin
 * @date: 2022-12-09 23:00:00
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-12 16:51:36
 * @description:
 **/
#include "common.h"

/* ȡ���ֽ� */
#define HBYTE(c)    (((c) >> 8) & 0xff)
/* ȡ���ֽ� */
#define LBYTE(c)    ((c)&0xff)
/* С��ģʽ�µİ���ת�ֽ����� */
#define TO2BYTES(c) LBYTE((c)), HBYTE((c))

/* VID & PID & �汾�� */
#define VID         (0X3062)
#define PID         (0X4702)
#define VER         (0X0120)

/* �˵��С */
#define EP0_SIZE    (64)
#define EP2_SIZE    (64)

/* �˵㻺���� */
UINT8X ep0_buff[66] _at_(0);
UINT8X ep2_buff[66] _at_(66);

/* һЩ��־ & ������� */
UINT8 SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;
PUINT8 pDescr;

#define UsbSetupBuf ((PUSB_SETUP_REQ)ep0_buff)  // Setup ��

/* ���������� */
char code desc_report[] = {
    /* keyboared */
    0x05, 0x01,  // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,  // USAGE (Keyboard)
    0xa1, 0x01,  // COLLECTION (Application)
    0x85, 0x01,  // Report ID (1) <-- ֮����ϱ���ֵ������ ID
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
    0x85, 0x02,        // REPORT_ID (2) <-- ֮����ϱ���ֵ������ ID
    0x19, 0x00,        // USAGE_MINIMUM (Unassigned)
    0x2a, 0x3c, 0x03,  // USAGE_MAXIMUM
    0x15, 0x00,        // LOGICAL_MINIMUM (0)
    0x26, 0x3c, 0x03,  // LOGICAL_MAXIMUM (828)
    0x95, 0x01,        // REPORT_COUNT (1)
    0x75, 0x18,        // REPORT_SIZE (24)
    0x81, 0x00,        // INPUT (Data,Ary,Abs)
    0x05, 0x01,        // USAGE_PAGE ��;ҳ0x01(��ͨ����)
    0x19, 0x00,        // USAGE_MINIMUM ��;��Сֵ0x00(δ����)
    0x29, 0x83,        // USAGE_MAXIMUM ��;���ֵ0x83(ϵͳ����)
    0x15, 0x00,        // LOGICAL_MINIMUM (0)
    0x25, 0x83,        // LOGICAL_MAXIMUM (83)
    0x75, 0x08,        // REPORT_SIZE (8)
    0x95, 0x01,        // REPORT_COUNT (1)
    0x81, 0x00,        // INPUT (Data,Ary,Abs)����1�ֽ�����    
    0xc0,              // END_COLLECTION  /* 25 */

    /* dial */
    0x05, 0x01,  // USAGE_PAGE
    0x09, 0x0e,  // USAGE (dial)
    0xa1, 0x01,  // COLLECTION (Application)
    0x85, 0x03,  // REPORT_ID (3) <-- ֮����ϱ���ֵ������ ID
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

/* USB �豸������ */
char code desc_device[] = {
    0x12,              // bLength         :��������С���̶�0x12
    0x01,              // bDescriptorType :�豸���������ͣ��̶�0x01
    TO2BYTES(0x0200),  // bcdUSB          :USB�汾��1.1--0x0110 2.0--0x0200
    0x00,              // bDeviceClass    :���ʹ��루��USBָ����
    0x00,              // bDeviceSubClass :�����ʹ��루��USB���䣩
    0x00,              // bDeviceProtocol :Э����루��USB���䣩
    EP0_SIZE,          // bMaxPacketSize0 :�˵�0�����Ĵ�С��ȫ��64
    TO2BYTES(VID),     // idVendor VID    :��Ӧ��ID����USB���䣩
    TO2BYTES(PID),     // idProduct PID   :��ƷID���ɳ��̷��䣩
    TO2BYTES(VER),  // bcdDevice          :�豸�������룮�ɳ�����������
    0x01,           // iManufacturer      :�����������ַ�������
    0x02,           // iProduct           :��Ʒ�������ַ�������
    0x03,           // iSerialNumber      :�豸���к��ַ�������
    0x01,  // bNumConfigurations :�������������豸�Ե�ǰ�ٶ�֧�ֵ���������
};

#define CFG_LEN (9 + 9 + 9 + 7)

/* ���������� */
char code desc_config[CFG_LEN] = {
    /* ���������� */
    0x09,  // bLength         :�����������ĳ��ȣ��̶�0x09
    0x02,  // bDescriptorType :������������ţ��̶�0x02
    TO2BYTES(CFG_LEN),  // wTotalLength  :�������������ϵ��ܳ���
    0x01,               // bNumInterfaces  :�����ð����Ľӿ���
    0x01,  // bConfiguration  :��ΪSet Configuration��һ������ѡ������ֵ
    0x01,  // iConfigurationz :�����õ��ַ���������
    0xA0,  // bmAttributes    :Bit4-0������D7:���߹��磬D6:�Թ��磬D5:Զ�̻���
    0x5A,  // bMaxPower       :����������ÿ��λ���� 2 mA

    /* �ӿ������� 1 */
    0x09,  // bLength            :�豸���������ֽ�����С��Ϊ0x09
    0x04,  // bDescriptorType    :���������ͱ�ţ��̶�Ϊ0x04
    0x01,  // bInterfaceNumber   :�ӿڵı��
    0x00,  // bAlternateSetting  :���õĽӿ����������
    0x01,  // bNumEndpoints      :�ýӿ�ʹ�ö˵������������˵�0
    0x03,  // bInterfaceClass    :�ӿ����� (HID�̶�Ϊ3)
    0x00,  // bInterfaceSubClass :�ӿ�������
    0x00,  // bInterfaceProtocol :�ӿ�����ѭ��Э��
    0x00,  // iInterface         :�����ýӿڵ��ַ�������ֵ

    /* HID ������ 1 */
    0x09,              // bLength         :�������ֽ���
    0x21,              // bDescriptorType :0x21 = HID������
    TO2BYTES(0x0111),  // bcdHID          :HID�淶�汾�ţ�BCD 1.11��
    0x00,  // bCountryCode      :Ӳ���豸���ڹ��ҵĹ��Ҵ���
    0x01,  // bNumDescriptors   :�����������Ŀ��������һ��������������
    0x22,  // bDescriptorType   :����������������� (0x22 ����)
    TO2BYTES(sizeof(desc_report)),  // wDescriptorLength:��������������ܳ���

    /* �˵������� 1 */
    0x07,  // bLength          :�豸���������ֽ�����С��Ϊ0x7
    0x05,  // bDescriptorType  :���������ͱ�ţ�Ϊ0x05
    0x82,  // bEndpointAddress :�˵��ַ������������� IN �˵� 2
    0x03,  // bmAttribute      :�˵�Ĵ����������� (0b11�ж�)
    TO2BYTES(EP2_SIZE),  // wMaxPacketSize   :�˵��ա����������Ĵ�С
    0x01,                // bInterval        :������ѯ�˵��ʱ����
};

/* �ַ���������[UNICODE encoding] */
/* �����ַ��� */
char code desc_lang[] = {0x04, 0x03, 0x09, 0x04};

/* �������ַ��� */
char code desc_manuf[] = {0x0A, 0x03, 0xCB, 0x7A, 0x1B,
                          0x52, 0x00, 0x5F, 0x90, 0x6E};
/* ��Ʒ�����ַ��� */
char code desc_produce[] = {
    0x1e, 0x03, 0x70, 0x00, 0x6f, 0x00, 0x6d, 0x00, 0x69, 0x00,
    0x6e, 0x00, 0x20, 0x00, 0x6b, 0x00, 0x65, 0x00, 0x79, 0x00,
    0x62, 0x00, 0x6f, 0x00, 0x61, 0x00, 0x72, 0x00, 0x64, 0x00,
};

UINT8X Ser_Des[30];  // �ɵ�Ƭ��ΨһID���ɵ��豸���к��ַ���

uchar dec2hex(uchar num) { return (num < 10) ? num + '0' : num + '7'; }

// ���ͳ���Ϊ30,ǰ��λΪ51��52������12λΪ�ڲ�ΨһID
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

    IE_USB = 0;       // �ر�USBʹ��
    USB_CTRL = 0x06;  // ���USB���ƼĴ���
    UDEV_CTRL = 0x00;

    USB_CTRL = 0x00;  // ���趨USB�豸ģʽ
    UDEV_CTRL = 0x80;
    UDEV_CTRL |= bUD_PD_DIS;  // ��ֹDP/DM��������

    UDEV_CTRL &= ~bUD_LOW_SPEED;  // ѡ��ȫ��12Mģʽ��Ĭ�Ϸ�ʽ
    USB_CTRL &= ~bUC_LOW_SPEED;

    UEP0_DMA = ep0_buff;  // �˵�0���ݴ����ַ
    UEP2_DMA = ep2_buff;  // �˵�2���ݴ����ַ

    UEP0_T_LEN = 0;  // �˵�0����
    UEP2_T_LEN = 0;  // �˵�2����

    UEP2_3_MOD |= bUEP2_TX_EN;     // �˵�2����ʹ��
    UEP2_3_MOD &= ~bUEP2_BUF_MOD;  // �˵�2��64�ֽڷ��ͻ�����(IN)
    UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK | UEP_R_RES_ACK;
    // �˵�2�Զ���תͬ����־λ��IN���񷵻�NAK��OUT����ACK

    USB_DEV_AD = 0x00;
    USB_CTRL |= bUC_DEV_PU_EN | bUC_INT_BUSY | bUC_DMA_EN;
    // ����USB�豸��DMA�����ж��ڼ��жϱ�־δ���ǰ�Զ�����NAK
    UDEV_CTRL |= bUD_PORT_EN;  // ����USB�˿�
    USB_INT_FG = 0xFF;         // ���жϱ�־
    USB_INT_EN = bUIE_SUSPEND | bUIE_TRANSFER | bUIE_BUS_RST;
    IE_USB = 1;
}

/**
 * @brief �ϴ����ݵ��˵� 2
 *
 * @param buf
 * @param len
 **/
void drv_usb_write_ep2(char *buf, unsigned char len) {
    while ((UEP2_CTRL & MASK_UEP_T_RES) == UEP_T_RES_ACK)
        ;
    len = (len > EP2_SIZE) ? EP2_SIZE : len;
    memcpy(ep2_buff, buf, len);  // �����ϴ�����
    UEP2_T_LEN = len;            // �ϴ����ݳ���
    UEP2_CTRL = UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK;
    // ������ʱ�ϴ����ݲ�Ӧ��ACK
}

void usb_handler(void) interrupt INT_NO_USB using 1 {
    uchar len;

    if (UIF_TRANSFER)  // USB������ɱ�־
    {
        switch (USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP)) {
            case UIS_TOKEN_IN | 2:  // endpoint 2# �˵������ϴ�
                UEP2_T_LEN = 0;     // Ԥʹ�÷��ͳ���һ��Ҫ���
                UEP2_CTRL =
                    UEP2_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;  // Ĭ��Ӧ��NAK
                FLAG = 1;
                break;

            case UIS_TOKEN_OUT | 2:  // endpoint 2# �˵������´�
                if (U_TOG_OK)        // ��ͬ�������ݰ�������
                {
                }
                break;

            case UIS_TOKEN_SETUP | 0:  // SETUP����
                len = USB_RX_LEN;

                if (len == (sizeof(USB_SETUP_REQ))) {
                    if (UsbSetupBuf->wLengthH)  // ������ܻᳬ255
                        SetupLen = 0xff;
                    else
                        SetupLen = UsbSetupBuf->wLengthL;
                    len = 0;  // Ĭ��Ϊ�ɹ������ϴ�0����
                    SetupReq = UsbSetupBuf->bRequest;

                    if ((UsbSetupBuf->bRequestType & USB_REQ_TYP_MASK) !=
                        USB_REQ_TYP_STANDARD) /*HID������*/
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
                                len = 0xFF; /*���֧��*/
                                break;
                        }

                        if (SetupLen > len) {
                            SetupLen = len;  // �����ܳ���
                        }
                    } else  // ��׼����
                    {
                        switch (SetupReq)  // ������
                        {
                            case USB_GET_DESCRIPTOR:
                                switch (UsbSetupBuf->wValueH) {
                                    case 1:  // �豸������
                                        pDescr =
                                            desc_device;  // ���豸�������͵�Ҫ���͵Ļ�����
                                        len = sizeof(desc_device);
                                        break;

                                    case 2:  // ����������
                                        pDescr =
                                            desc_config;  // ���豸�������͵�Ҫ���͵Ļ�����
                                        len = sizeof(desc_config);
                                        break;

                                    case 3:
                                        switch (UsbSetupBuf->wValueL) {
                                            case 0:  // ����������
                                                pDescr = desc_lang;
                                                len = sizeof(desc_lang);
                                                break;
                                            case 1:  // ������������
                                                pDescr = desc_manuf;
                                                len = sizeof(desc_manuf);
                                                break;
                                            case 2:  // ��Ʒ�����ַ���
                                                pDescr = desc_produce;
                                                len = sizeof(desc_produce);
                                                break;
                                            default:  // ��Ʒ���к�
                                                pDescr = Ser_Des;
                                                len = sizeof(Ser_Des);
                                                break;
                                        }
                                        break;

                                    case 0x22:  // ����������
                                        switch (UsbSetupBuf->wIndexL) {
                                            case 1:
                                                pDescr =
                                                    desc_report;  // ����׼���ϴ�
                                                len = sizeof(desc_report);
                                                break;
                                        }

                                    default:
                                        len = 0xff;  // ��֧�ֵ�������߳���
                                        break;
                                }

                                if (SetupLen > len) {
                                    SetupLen = len;  // �����ܳ���
                                }

                                len = SetupLen >= EP0_SIZE
                                          ? EP0_SIZE
                                          : SetupLen;  // ���δ��䳤��
                                memcpy(ep0_buff, pDescr, len);  // �����ϴ�����
                                SetupLen -= len;
                                pDescr += len;
                                break;

                            case USB_SET_ADDRESS:
                                SetupLen =
                                    UsbSetupBuf->wValueL;  // �ݴ�USB�豸��ַ
                                break;

                            case USB_GET_CONFIGURATION:
                                ep0_buff[0] = UsbConfig;

                                if (SetupLen >= 1) {
                                    len = 1;
                                }

                                break;

                            case USB_SET_CONFIGURATION:
                                UsbConfig = UsbSetupBuf->wValueL;
                                if (UsbConfig) Ready = 1;  // ö�����
                                break;

                            case 0x0A:
                                break;

                            case USB_CLEAR_FEATURE:  // Clear Feature
                                if ((UsbSetupBuf->bRequestType &
                                     USB_REQ_RECIP_MASK) ==
                                    USB_REQ_RECIP_ENDP)  // �˵�
                                {
                                    switch (UsbSetupBuf->wIndexL) {
                                        case 0x82:
                                            UEP2_CTRL =
                                                UEP2_CTRL & ~(bUEP_T_TOG |
                                                              MASK_UEP_T_RES) |
                                                UEP_T_RES_NAK;
                                            /* ���ö˵�2 IN  */
                                            break;
                                        case 0x02:
                                            UEP2_CTRL =
                                                UEP2_CTRL & ~(bUEP_T_TOG |
                                                              MASK_UEP_T_RES) |
                                                UEP_T_RES_NAK;
                                            /* ���ö˵�2 OUT */
                                            break;

                                        default:
                                            len = 0xFF;  // ��֧�ֵĶ˵�
                                            break;
                                    }
                                } else {
                                    len = 0xFF;  // ���Ƕ˵㲻֧��
                                }

                                break;

                            case USB_SET_FEATURE: /* Set Feature */
                                if ((UsbSetupBuf->bRequestType & 0x1F) ==
                                    0x00) /* �����豸 */
                                {
                                    if ((((uint)UsbSetupBuf->wValueH << 8) |
                                         UsbSetupBuf->wValueL) == 0x01) {
                                        if (desc_config[7] & 0x20) {
                                            /* ���û���ʹ�ܱ�־ */
                                        } else {
                                            len = 0xFF; /* ����ʧ�� */
                                        }
                                    } else {
                                        len = 0xFF; /* ����ʧ�� */
                                    }
                                } else if ((UsbSetupBuf->bRequestType & 0x1F) ==
                                           0x02) /* ���ö˵� */
                                {
                                    if ((((uint)UsbSetupBuf->wValueH << 8) |
                                         UsbSetupBuf->wValueL) == 0x00) {
                                        switch (
                                            ((uint)UsbSetupBuf->wIndexH << 8) |
                                            UsbSetupBuf->wIndexL) {
                                            case 0x82:
                                                UEP2_CTRL =
                                                    UEP2_CTRL & (~bUEP_T_TOG) |
                                                    UEP_T_RES_STALL; /* ���ö˵�2
                                                           IN
                                                         */
                                                break;
                                            case 0x02:
                                                UEP2_CTRL =
                                                    UEP2_CTRL & (~bUEP_R_TOG) |
                                                    UEP_R_RES_STALL; /* ���ö˵�2
                                                           OUT
                                                            */
                                                break;

                                            default:
                                                len = 0xFF; /* ����ʧ�� */
                                                break;
                                        }
                                    } else {
                                        len = 0xFF; /* ����ʧ�� */
                                    }
                                } else {
                                    len = 0xFF; /* ����ʧ�� */
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
                                len = 0xff;  // ����ʧ��
                                break;
                        }
                    }
                } else {
                    len = 0xff;  // �����ȴ���
                }

                if (len == 0xff) {
                    SetupReq = 0xFF;
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL |
                                UEP_T_RES_STALL;  // STALL
                } else if (len <= EP0_SIZE)  // �ϴ����ݻ���״̬�׶η���0���Ȱ�
                {
                    UEP0_T_LEN = len;
                    UEP0_CTRL =
                        bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK |
                        UEP_T_RES_ACK;  // Ĭ�����ݰ���DATA1������Ӧ��ACK
                } else {
                    UEP0_T_LEN =
                        0;  // ��Ȼ��δ��״̬�׶Σ�������ǰԤ���ϴ�0�������ݰ��Է�������ǰ����״̬�׶�
                    UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK |
                                UEP_T_RES_ACK;  // Ĭ�����ݰ���DATA1,����Ӧ��ACK
                }

                break;

            case UIS_TOKEN_IN | 0:  // endpoint0 IN
                switch (SetupReq) {
                    case USB_GET_DESCRIPTOR:
                    case HID_GET_REPORT:
                        len = SetupLen >= EP0_SIZE ? EP0_SIZE
                                                   : SetupLen;  // ���δ��䳤��
                        memcpy(ep0_buff, pDescr, len);  // �����ϴ�����
                        SetupLen -= len;
                        pDescr += len;
                        UEP0_T_LEN = len;
                        UEP0_CTRL ^= bUEP_T_TOG;  // ͬ����־λ��ת
                        break;

                    case USB_SET_ADDRESS:
                        USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                        break;

                    default:
                        UEP0_T_LEN =
                            0;  // ״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ���
                        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                        break;
                }

                break;

            case UIS_TOKEN_OUT | 0:
                if (USB_RX_LEN == 2 &&
                    ep0_buff[0] == 1)  // ���մ�Сдʱ�̶�����Ϊ2
                {
                    if (ep0_buff[1] & 1)  // �����ּ���LED
                    {
                        num_lock = 1;
                    } else  // �����ּ���LED
                    {
                        num_lock = 0;
                    }

                    if (ep0_buff[1] & 2)  // ����дLED
                    {
                        caps_lock = 1;
                    } else  // �ش�дLED
                    {
                        caps_lock = 0;
                    }

                    if (ep0_buff[1] & 4)  // ��scrol LED
                    {
                    } else  // ��scrol LED
                    {
                    }
                }
                UEP0_CTRL ^= bUEP_R_TOG;  // ͬ����־λ��ת
                break;

            default:
                break;
        }

        UIF_TRANSFER = 0;  // ���жϱ�־
    }

    if (UIF_BUS_RST)  // �豸ģʽUSB���߸�λ�ж�
    {
        UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        UEP2_CTRL = bUEP_AUTO_TOG | UEP_T_RES_ACK;
        USB_DEV_AD = 0x00;
        UIF_SUSPEND = 0;
        UIF_TRANSFER = 0;
        UIF_BUS_RST = 0;  // ���жϱ�־
    }

    if (UIF_SUSPEND)  // USB���߹���/�������
    {
        UIF_SUSPEND = 0;
    } else  // ������ж�,�����ܷ��������
    {
        USB_INT_FG = 0xFF;  // ���жϱ�־
    }
}
