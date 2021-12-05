#include "PWM.h"

Ticker pwm_tic;
PWM pwm;
u8 count = 0;

s16 ek[3] = {0,0,0};

PWM::PWM()
{
    pinMode(PWM_IO,OUTPUT);
    pinMode(FAN_IO,OUTPUT);
    digitalWrite(PWM_IO,LOW);
    digitalWrite(FAN_IO,LOW);
}

void pwm_irq()
{
    count ++;
    if(!count && pwm.high_time)
        digitalWrite(PWM_IO,HIGH);
    if(count > pwm.high_time)
        digitalWrite(PWM_IO,LOW);  
}



void PWM::begin()
{
    if(temp_mode == Co_Temp)
        temp_time_buf = temp_mode1_time;
    else
    {
        if(adc.now_temp > 60)
            return;
        backflow_working_state = 1;
        backflow_temp_tmp = adc.now_temp;
        percent = 0;
    }
    pinMode(PWM_IO,OUTPUT);
    digitalWrite(PWM_IO,LOW);
    pwm_tic.attach_ms(4,pwm_irq);
    power = 1;
    
    ui.heat_flg = 1;  
    oled_sleep_t = 0; 

}
void PWM::end()
{
    pwm_tic.detach();
    high_time = 0;
    digitalWrite(PWM_IO,LOW);
    power = 0;

    backflow_working_state = 0;
    if(temp_mode == Co_Temp && temp_reached_flg)
        ui.temp_time_switch_flg = true;
    ui.heat_flg = 1;
    oled_sleep_t = 0;
}

void PWM::temp_set()
{
    if(!pwm.power)
    {
        high_time = 0;
        return;
    }

    if(temp_mode == Co_Temp)
    {
        need_set_temp = temp_buf;
        if(adc.now_temp >= temp_buf && temp_reached_flg == false)
        {
            if(ui.temp_time_switch_flg == false)
                ui.temp_time_switch_flg = true;
        }
        if(temp_reached_flg == true && temp_time_buf == 0)      
        {
            end();
            
            return;
        }
    }
    else
        need_set_temp = backflow_temp_tmp;


////////////////////////////////////////////


    ek[2] = ek[1];
    ek[1] = ek[0];
    ek[0] = need_set_temp - adc.now_temp;

    if(adc.now_temp == 38)
    {
        pwm_buf = need_set_temp / 2;
    }
    else
    {
        pwm_buf_f += ek[0];

        if(adc.now_temp < 180)
        {
            if(pwm_buf_f > 20)
                    pwm_buf_f = 20;
            else if(pwm_buf_f < -20)
                pwm_buf_f = -20;
        }
        else
        {
            if(pwm_buf_f > 50)
                pwm_buf_f = 50;
            else if(pwm_buf_f < -50)
                pwm_buf_f = -50;
        }
        if(adc.now_temp < 200)
            pwm_buf = kp * (ek[0] + ki * pwm_buf_f + kd * (ek[0] - ek[1]));
        else
            pwm_buf = kp * (ek[0] + ki_high * pwm_buf_f + kd * (ek[0] - ek[1]));


        if(ek[0] < 10 && ek[0] > 3)
        {
            if(adc.now_temp < 200)
                pwm_buf /=  3; 
        }
        else if(ek[0] < 4)
        {
            if(adc.now_temp < 200)
            {
                pwm_buf /=  8;
            }
            else
                pwm_buf /=  3;

            if(ek[0] > -1)
                pwm_buf += need_set_temp / 10;
            
        }
        if(ek[0] < 0 && pwm_buf > 0)
        {
            pwm_buf = 0;
        }
    }

    if(pwm_buf < 0)
    {
        pwm_buf = 0;  
    }
    if(pwm_buf > 255)
    {
        pwm_buf = 255;  
    }
    high_time = pwm_buf;
    // high_time = 255;
    Serial.println(high_time);
}

void PWM::fan()
{
    fan_state = !fan_state;
    digitalWrite(FAN_IO,fan_state);
}
