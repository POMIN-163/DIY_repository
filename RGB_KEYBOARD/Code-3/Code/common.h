/**
 * @author: Pomin
 * @date: 2022-12-09 21:49:59
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 21:17:26
 * @description:
 **/
#ifndef _COMMON_H
#define _COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "Public/CH552.H"
#include "Public/DEBUG.H"
#include "keycode.h"
#include "ws2812.h"

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long

sbit IO_RGB = P3 ^ 2;
sbit IO_KEY0 = P1 ^ 5;
sbit IO_KEY1 = P1 ^ 6;
sbit IO_KEY2 = P1 ^ 7;
sbit IO_EC11_D = P1 ^ 4;
sbit IO_EC11_A = P3 ^ 4;
sbit IO_EC11_B = P3 ^ 3;

/* hsv 色彩空间 */
typedef struct chsv {
    UINT8 h;
    UINT8 s;
    UINT8 v;
    UINT8 raw[3];
} chsv;

/* rgb 色彩 */
typedef struct crgb {
    UINT8 r;
    UINT8 g;
    UINT8 b;
} crgb;

/* 普通键盘数据 */
extern UINT8 HIDKey[8];
/* 多媒体按键数据 */
extern UINT8 HIDKeyMUL[1];
extern UINT8 SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;

extern bit num_lock;
extern bit caps_lock;

void rgb_off(void);
void rgb_one(UINT8X *RGB);
void rgb_all(UINT8X *RGB);

void HSVtoRGB(crgb *rgb, chsv *hsv);

/* usb 初始化 */
void drv_usb_init(void);
/* usb 上传数据到端点 2 */
void drv_usb_write_ep2(char *buf, uchar len);

/* 按键、dial 驱动 */
void drv_usb_dial(int wheel);
void drv_usb_keyboard(uchar *p);
void drv_usb_mul(uchar dat);
void drv_usb_mul_click(uchar dat);

/* 按键状态 */
uchar get_numlock(void);
uchar get_fn_lock(void);
uchar get_capslock(void);

#ifdef __cplusplus
}
#endif
#endif  // _COMMON_H
