/**
 * @author: Pomin
 * @date: 2022-12-11 14:10:27
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 21:58:21
 * @description:
 **/
#include "common.h"

bit num_lock = 0;
bit caps_lock = 0;


/**
 * @brief 普通按键
 * HID_KEY_CTRL = HID_L_ALT;
 * HID_KEY_MAIN = HID_KEYBOARD_A;
 * drv_usb_keyboard(HIDKey);
 * @param p
**/
void drv_usb_keyboard(uchar *p) {
    char idata temp[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    uchar i;
    for (i = 0; i < 8; i++) {
        temp[i + 1] = p[i];
    }
    drv_usb_write_ep2(temp, 9);
}

/**
 * @brief 旋钮
 *
 * @param wheel
**/
void drv_usb_dial(int wheel) {
    char buf_tx[4] = {
        3,
        0,
        0,
        0,
    };

    buf_tx[1] = *((char *)&wheel + 1);
    buf_tx[2] = *((char *)&wheel + 0);

    drv_usb_write_ep2(buf_tx, 3);
}

/**
 * @brief 单击多媒体按键
 *
 **/
extern UINT8 FLAG;  // 上传完成标志
void drv_usb_mul_click(uchar dat) {
    FLAG = 0;
    drv_usb_mul(dat);
    while (FLAG == 0)
        ;
    drv_usb_mul(0);
    while (FLAG == 0)
        ;
}

/**
 * @brief 多媒体按键
 *
 * @param dat
**/
void drv_usb_mul(uchar dat) {
    char idata temp[5] = {2, 0, 0, 0, 0};

    temp[1] = dat;

    if (dat == HID_CONSUMER_CALCULATOR || dat == HID_CONSUMER_MULTI)
        temp[2] = 0x01;
    else if (dat == HID_CONSUMER_CHROME)
        temp[2] = 0x02;
    else
        temp[2] = 0x00;
    drv_usb_write_ep2(temp, 5);
}

/**
 * @brief 获取数字锁、大写锁定、fn
 *
 * @return uchar
**/
uchar get_numlock(void) { return !!(num_lock); }
uchar get_capslock(void) { return !!(caps_lock); }
uchar get_fn_lock(void) { return !(num_lock == caps_lock); }
