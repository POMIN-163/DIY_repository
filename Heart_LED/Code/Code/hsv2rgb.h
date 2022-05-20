/**
 * @author: Pomin
 * @date: 2022-05-19 22:03:17
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-05-20 10:24:48
 * @description:
**/
#ifndef _hsv2rgb_H
#define _hsv2rgb_H

#include "./Public/CH554.H"
#include "./Public/DEBUG.H"
#include "WS2812.h"


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

void HSVtoRGB(crgb* rgb, chsv* hsv);

#endif // _hsv2rgb_H
