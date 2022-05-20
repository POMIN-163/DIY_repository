#ifndef __WS2812_H
#define __WS2812_H
#include <intrins.h>

#define SIGNAL1 rgb_led = 1;\
_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
rgb_led = 0;\
_nop_();_nop_();_nop_();_nop_();_nop_();

#define SIGNAL0 rgb_led = 1;\
_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
rgb_led = 0;\
_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();\
_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();

// 当前环境一个 nop 约 0.0625us
/*
   实测:
   1H : 833ns
   1L : 417ns
   0H : 417ns
   0L : 833ns
*/

#endif


