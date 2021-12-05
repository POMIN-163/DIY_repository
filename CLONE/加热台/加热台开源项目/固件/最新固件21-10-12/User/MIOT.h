#ifndef _MIOT_H_
#define _MIOT_H_
#include "User.h"

#define DEBUG


#define BUTTON_1 "btn-power"//开启加热对应点灯APP的按钮名
#define BUTTON_2 "btn-off"//关闭加热对应点灯APP的按钮名
#define BUTTON_3 "btn-hengwen"//设置为恒温模式对应点灯APP的按钮名
#define BUTTON_4 "btn-huiliu"//设置为回流模式对应点灯APP的按钮名
#define BUTTON_5 "btn-fan"//开启、关闭风扇对应点灯APP的按钮名
#define Slider_1 "ran-temp"//调整温度的滑块对应点灯APP的滑块名

/*
    因接入小爱不能自定义设备功能，所以用灯模式+训练计划模拟
    对应自己设置的小爱训练计划修改温度值
*/

#define MIOT_DAY 150     //日光模式
#define hongse  160     //红色
#define huangse 170     //黄色
#define juhuangse 180     //橘黄色
#define lvse  190     //绿色
#define qingse 200     //青色
#define lanse 210     //蓝色
#define tianlanse 220     //天蓝色
#define zise 230     //紫色
#define fense 240     //粉色
#define fenhongse 250     //粉红色

#define zihongse 50     //紫红色  我用来热牛奶
#define qianlvse 100     //浅绿色  


// #define MIOT_COMPUTER
// #define MIOT_READING  //阅读模式温度  
// #define MIOT_TV       //电视模式
// #define MIOT_WARMTH   //温馨模式
// #define MIOT_COLOR    //彩光模式   //恒温
// #define MIOT_NIGHT    //夜光模式   //回流



#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#define BLINKER_WITHOUT_SSL
#define BLINKER_ESP_SMARTCONFIG
// #define BLINKER_APCONFIG



class MIOT{
    public:
    void begin();
    void run_task();
    bool wifi_conect_flg = 0;
    bool miot_able = 0;
    bool open_flg = 0;
};




extern MIOT miot;



#endif