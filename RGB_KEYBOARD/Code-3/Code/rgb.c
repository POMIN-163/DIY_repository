/**
 * @author: Pomin
 * @date: 2022-12-11 14:17:40
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-11 20:50:24
 * @description:
 **/
#include "common.h"

// ʵ���� WS2812 ���̡��졢���Ĵ���Ҳ���� GRB, ���� RGB
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
    }  // �պͷǿմ���һ��, д����Ϊ�˲���ɷ������ݵ�ʱ������
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
        rgb_one(RGB);  // �����µļ��� RGB
        RGB += 3;
    }
}

void rgb_off(void) {
    UINT8X i;
    for (i = 0; i < 3; i++) rgb_one(NULL);
}
