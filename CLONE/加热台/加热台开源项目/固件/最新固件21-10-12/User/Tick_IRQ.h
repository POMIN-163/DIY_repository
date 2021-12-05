#ifndef _Tick_IRQ_H_
#define _Tick_IRQ_H_

#include "User.h"

#define oled_display_sleep_time 60 //屏幕休眠时间 如不需要休眠  可设置为256



void Ticker_init();


extern u8 oled_flg ;
extern s16 temp_time_buf;
extern s16 min_count;
extern u8 oled_sleep_t;



#endif