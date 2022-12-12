/**
 * @author: Pomin
 * @date: 2022-12-11 14:17:40
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 20:50:24
 * @description:
 **/
#include "common.h"

// 实际上 WS2812 是绿、红、蓝的次序也就是 GRB, 并非 RGB
void rgb_one(UINT8X *RGB) {
    UINT8X k, byte;
    if (RGB == NULL) {
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        return;
    }  // 空和非空代码一样, 写两遍为了不造成发送数据的时序问题
    else {
        byte = RGB[1];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = RGB[0];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = RGB[2];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
    }
}

void rgb_all(UINT8X *RGB) {
    UINT8X i;
    for (i = 0; i < 3; i++) {
        rgb_one(RGB);  // 亮起按下的键的 RGB
        RGB += 3;
    }
}

void rgb_off(void) {
    UINT8X i;
    for (i = 0; i < 3; i++) rgb_one(NULL);
}
