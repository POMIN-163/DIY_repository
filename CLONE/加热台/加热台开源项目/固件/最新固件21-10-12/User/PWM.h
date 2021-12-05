#ifndef _PWM_H_
#define _PWM_H_

#include <Arduino.h>
#include <Ticker.h>
#include "ADC.h"
#include "Tick_IRQ.h"

#define PWM_IO 14
#define FAN_IO 15



#define kp 4.5
#define ki 0.5
#define ki_high 3
#define kd 30.0









#define OFF 0
#define ON 1

#define Re_So 0
#define Co_Temp 1





class PWM{
    public:
    PWM();
    void begin();
    void end();
    void temp_set();
    void fan();
    bool power = OFF;
    bool sleep_flg = OFF;
    
    s16 temp_buf = 0;//设置温度

    u8 temp_mode = 0; //0 回流 1：恒温

    s16 temp_mode1_time = 0;//恒温时间

    u8 percent = 0;

    bool temp_reached_flg = false;

    u8 backflow_working_state = 0;
    // s16 backflow_temp_buf = 0;
    s16 backflow_temp_tmp = 0;

    
    private:

    bool fan_state = OFF;

    s16 need_set_temp = 50;
    
    u8 high_time = 0;

    s16 pwm_buf = 0;
    float pwm_buf_f = 0;
    friend void pwm_irq();
    


    
};



extern PWM pwm;



#endif