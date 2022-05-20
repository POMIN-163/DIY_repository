/**
 * @author: Pomin
 * @date: 2022-05-19 22:03:08
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-05-20 11:35:17
 * @description:
**/
#include "hsv2rgb.h"

void HSVtoRGB(crgb* rgb, chsv* hsv) {
    // R,G,B from 0-255, H from 0-360, S,V from 0-100
    int i;
    int difs;
    UINT32 RGB_min, RGB_max, RGB_Adj;

    RGB_max = hsv->v * 2.55f;
    RGB_min = RGB_max * (100 - hsv->s) / 100.0f;

    i = hsv->h / 60;
    difs = hsv->h % 60;  // factorial part of h

    // rgb adjustment amount by hue
    RGB_Adj = (RGB_max - RGB_min) * difs / 60.0f;

    switch (i) {
        case 0:
            rgb->r = RGB_max;
            rgb->g = RGB_min + RGB_Adj;
            rgb->b = RGB_min;
            break;
        case 1:
            rgb->r = RGB_max - RGB_Adj;
            rgb->g = RGB_max;
            rgb->b = RGB_min;
            break;
        case 2:
            rgb->r = RGB_min;
            rgb->g = RGB_max;
            rgb->b = RGB_min + RGB_Adj;
            break;
        case 3:
            rgb->r = RGB_min;
            rgb->g = RGB_max - RGB_Adj;
            rgb->b = RGB_max;
            break;
        case 4:
            rgb->r = RGB_min + RGB_Adj;
            rgb->g = RGB_min;
            rgb->b = RGB_max;
            break;
        default:  // case 5:
            rgb->r = RGB_max;
            rgb->g = RGB_min;
            rgb->b = RGB_max - RGB_Adj;
            break;
    }
}

