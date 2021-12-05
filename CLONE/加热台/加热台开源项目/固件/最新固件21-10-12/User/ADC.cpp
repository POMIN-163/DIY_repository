#include "ADC.h"

ADC adc;

ADC::ADC()
{
    pinMode(switch_io,OUTPUT);
    digitalWrite(switch_io,channel_low_temp);
}
 

void ADC::get()
{
    u8 i;
    if(adc_mode_state == channel_low_temp)
    {
        for(i=0;i<7;i++)
            adc_buf[i] = adc_buf[i+1];
        adc_buf[i] = analogRead(A0);
    }
    else
    {
        for(i=0;i<7;i++)
            adc_buf_high[i] = adc_buf_high[i+1];
        adc_buf_high[i] = analogRead(A0);
    }

    set_channel(!adc_mode_state);


}

void ADC::get_voltage()
{
    u8 i;
    vol_low = 0;
    for(i=0;i<8;i++)
        vol_low += adc_buf[i];
    vol_low >>= 3;
    vol_low = vol_low * 1000 / 1024;

    vol_high = 0;
    for(i=0;i<8;i++)
        vol_high += adc_buf_high[i];
    vol_high >>= 3;
    vol_high = vol_high * 1000 / 1024;

}
//51  150`
void ADC::get_temp_task()
{
    s16 rt;
    get_voltage();
    s16 tt;
    s16 temp_buf = 0;
    double buf;

    rt = vol_low * 1000 / ((3300 - vol_low) / 13);
    now_temp = (100 / (log(rt / 10000.0) / 3950 + 1/298.15) - 27315) / 100;
    if(now_temp < 151)
        return;

    if(now_temp < 160 && !adc_error && pwm.power)
    {
        temp_buf = now_temp;
    }

    vol_high = vol_high * 1000 / 21;

    buf = vol_high * 1000 / ((3300000 - vol_high) / 13.0);
    tt =       (100 / (log(buf / 10000) / 3950 + 1/298.15) - 27315) / 100;
    tt = tt - 52.0 / 175.0 * (tt - 132);
    now_temp = tt;
    if(temp_buf)
    {
        adc_error = temp_buf - now_temp;
    }
    now_temp += adc_error;
}

void ADC::set_channel(bool channel)
{
    digitalWrite(switch_io,channel);
    adc_mode_state = channel;
}


