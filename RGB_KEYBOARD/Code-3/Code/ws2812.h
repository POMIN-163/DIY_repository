/**
 * @author: Pomin
 * @date: 2022-12-09 21:48:46
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-12-09 21:58:03
 * @description:
 **/
#ifndef _WS2812_H
#define _WS2812_H

#ifdef __cplusplus
extern "C" {
#endif

#include "common.h"
#include <intrins.h>

#define SIGNAL1 \
    IO_RGB = 1; \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    IO_RGB = 0; \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();

#define SIGNAL0 \
    IO_RGB = 1; \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    IO_RGB = 0; \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();    \
    _nop_();

// 当前环境 (24MHz)一个 nop 约 0.0625us
/*
   实测:
   1H : 833ns
   1L : 417ns
   0H : 417ns
   0L : 833ns
*/

#ifdef __cplusplus
}
#endif
#endif  // _WS2812_H
