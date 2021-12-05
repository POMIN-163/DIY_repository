#ifndef _ui_H_
#define _ui_H_

#include "User.h"



#define show_disable 0
#define show_set_temp 1
#define show_now_temp 2
#define show_set_light 3
#define show_temp_mode1_time 4


#define back 0
#define sure 1

void ui_key_callb();
extern short curve_temp_buf[4];


class UI{

    public:

    void run_task();

    bool page_switch(u8 mode);

    void page1_key();
    void page2_key();
    void page3_key();

    bool oled_display_set();
    
    u8 show_temp_mode = show_now_temp;
    u8 temp_move_flg = 0;//温度显示切换标志
    u8 temp_mode_flg = 0;//加热模式切换标志
    u8 heat_flg = 0;     //加热开关切换标志
    bool page_switch_flg = 1;//界面切换标志
    s8 switch_buf = sure;
    bool temp_time_switch_flg = 0;
    bool oled_sleep_flg = 0;


    s16 oled_light = 0;
    bool wake_sleep_change_flg = 0;

    
    
    
    private:

    void show_temp(s8 x,s8 y,s8 xx,s8 yy);
    void show_page(short x,short y,u8 page);
    void temp_move();
    void temp_mode_move();
    void heat_move();
    void page2_move();
    void page3_switch();
    void blinker_config();
    void write_oled_light();
    void show_curve(s8 y,s8 data_y);
    void temp_time_switch();
    void wake_sleep_page();
    

    u8 set_temp_x = 92;
    

    s8 miot_option_buf = 0;

    bool blinker_config_flg = 0;
    bool write_oled_flg = 0;


    
  

    
};


extern UI ui;
extern u8 show_temp_mode;


#endif