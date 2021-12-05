#ifndef _EC11_H_
#define _EC11_H_

#include <Arduino.h>
#include <Ticker.h>


//IO定义  需支持外部中断且能设置为上拉输入的IO
//如编码方向反了调换A\B即可


#define sw_clr 0
#define sw_click 1
#define sw_double 2
#define sw_long 3

#define ec_left -1  
#define ec_right 1   


#define key_time_out 300//长按多少ms识别为长按 
#define key_double_out 400//双击间隔在多少ms内有效 如不需要双击功能 值设为1
#define shake_t 5  //抖动抑制时间  默认5ms  


class Skey {
    
    public:

    void set_io(u8 sw,u8 sa,u8 sb);

    //按键事件回调函数注册 
    void attch_task(void (*func)());
    //按键事件注销
    void detach();

    //启用按键中断  写入EEPROM时必须关闭
    void int_work();
    //关闭按键中断
    void int_close();

    
    //加速算法是否启用 val:true/false 默认关闭false
    void speed_up(bool val);

    //加速算法最大加速值,1-65535  默认100
    void speed_up_max(u16 val);

    //加速算法手感  默认15  0-255
    void speed_up_feel(u8 val);

    //双击识别是否启用 val:true/false 默认开启true
    void double_click(bool val);

    
    //编码值调用识别 未启用加速算法:ec_left左转(值-1) ec_right右转(值1)   启用加速算法后值为±1至±speed_up_max()所设置值
    s16 ec_buf = sw_clr;
    //按键值调用识别 sw_click单击 sw_double双击 sw_long长按
    u8 sw_buf = sw_clr;
    
    private:

    void (*attch_p)() = NULL;
    void task();
    void key_mode_read();
    friend void io_sw_int();
    friend void io_sa_sb_int();
    friend void key_timer();
    bool work_flg = false;
    bool state = 1;
    bool state_buf = 1;
    u8 state_count = 0;
    bool speed_up_flg = false;
    u16 speed_up_max_num = 100;
    u8 speed_up_feel_count = 15;
    u16 double_click_count = key_double_out;
    
    
    
};


extern Skey ec11;  

#endif
