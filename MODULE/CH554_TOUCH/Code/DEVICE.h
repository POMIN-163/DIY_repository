/********************************** (C) COPYRIGHT *******************************
* File Name          : DEVICE.H
* Author             : RZ
* Version            : V1.00
* Date               : 2017-5-17
* Description        : USB驱动函数
*******************************************************************************/

#ifndef __DEVICE_H__
#define __DEVICE_H__

#include "DEBUG.H"
#include "CONFIG.H"

extern UINT16 CTP_VID;                                                      /* 可以主函数修改VID PID, 默认为沁恒公司保留VID_PID */
extern UINT16 CTP_PID;
extern UINT8  MAX_POINT;                                                    /* 最大支持点数,如需要支持XP系统,该值最大设置7, 如WIN7或其他系统最大支持10点 */	

extern UINT8 	USB_Enum_OK;                                               //usb初始化完成会置1。
extern void 	USBDeviceInit		( void );                              //usb初始化函数
extern UINT8 	Absolute_Up_Pack	( POINTER * pTP, UINT16 Press_Flag );  //usb上传函数
	


#endif

/* END OF FILE */
