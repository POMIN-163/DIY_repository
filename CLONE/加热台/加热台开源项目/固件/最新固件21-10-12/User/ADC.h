#ifndef _ADC_H_
#define _ADC_H_


#define switch_io 16
#define channel_low_temp 0
#define channel_high_temp 1

#include <Arduino.h>
#include "pwm.h"



class ADC{

public:
    ADC();
    
    void get_temp_task();
    void get();
    void set_channel(bool channel);
    u16 now_temp = 0;
    u16 now_temp_high = 0;
    bool adc_get_temp_flg = 0;

private:
    void get_voltage();
    u16 adc_buf[8];
    u16 adc_buf_high[8];
    u16 vol_low = 0;
    u16 vol_high = 0;
    s8 adc_error = 0;

    bool adc_mode_state = channel_low_temp;
};

extern ADC adc;

#endif