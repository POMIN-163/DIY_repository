/********************************** (C) COPYRIGHT *******************************
* File Name          : DEVICE.H
* Author             : RZ
* Version            : V1.00
* Date               : 2017-5-17
* Description        : USB��������
*******************************************************************************/

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "DEBUG.H"
#include "CONFIG.H"

extern UINT16 CTP_VID;                                                      /* �����������޸�VID PID, Ĭ��Ϊ�ߺ㹫˾����VID_PID */
extern UINT16 CTP_PID;
extern UINT8  MAX_POINT;                                                    /* ���֧�ֵ���,����Ҫ֧��XPϵͳ,��ֵ�������7, ��WIN7������ϵͳ���֧��10�� */	

extern UINT8 	USB_Enum_OK;                                               //usb��ʼ����ɻ���1��
extern void 	USBDeviceInit		( void );                              //usb��ʼ������
extern UINT8 	Absolute_Up_Pack	( POINTER * pTP, UINT16 Press_Flag );  //usb�ϴ�����
	


#endif

/* END OF FILE */
