#include "Tick_IRQ.h"

Ticker ticker25ms;
Ticker ticker100_ms;
Ticker ticker50_ms;
Ticker ticker1s;

u8 oled_flg = 0;

s16 temp_time_buf = 0;
s16 min_count = 0;
u8 oled_sleep_t = 0;

void s1_tic()
{

  adc.get_temp_task();
  pwm.temp_set();
  s8 tmp;

  if(adc.now_temp == 38 && !pwm.power)
  {
    oled_sleep_t ++;
    if(oled_sleep_t == oled_display_sleep_time && !ui.oled_sleep_flg)
    {
      ui.wake_sleep_change_flg = 1;
    }
  }
  else
  {
    oled_sleep_t = 0;
  }
  

  min_count ++;
  if(pwm.temp_reached_flg == true)
  {
    if(min_count == 60)
    {
      min_count = 0;
      temp_time_buf --;
    }
  }
  if(pwm.backflow_working_state == 1)
  {
    tmp = (curve_temp_buf[0] - 38) * 10 / 25 ;
    if(tmp == 0)
      tmp = 1;
    pwm.percent = (adc.now_temp - 38) * 10 / tmp;
    if(pwm.backflow_temp_tmp < curve_temp_buf[0])
    {
      pwm.backflow_temp_tmp += 3;
      if(pwm.backflow_temp_tmp > curve_temp_buf[0])
        pwm.backflow_temp_tmp = curve_temp_buf[0];
    }
    else
    {
      
      if(adc.now_temp >= pwm.backflow_temp_tmp)
      {
        pwm.backflow_working_state = 2;
        min_count = 0;
      }
    }
    
  }
  else if(pwm.backflow_working_state == 2)
  {
    tmp = curve_temp_buf[1] *10 / 25 ;
    if(tmp == 0)
      tmp = 1;
    pwm.percent = 25 + min_count * 10 / tmp;
    if(min_count == curve_temp_buf[1])
    {
      pwm.backflow_working_state = 3;
      pwm.backflow_temp_tmp = curve_temp_buf[2];
    }
    
  }
  else if(pwm.backflow_working_state == 3)
  {
    tmp = (217 - curve_temp_buf[0]) * 10 / 25;
    if(tmp == 0)
      tmp = 1;
    pwm.percent = 50 + (adc.now_temp - curve_temp_buf[0]) * 10 / tmp;
    if(adc.now_temp > 215)
    {
      pwm.backflow_working_state = 4;
      min_count = 0;
    }
  }
  else if(pwm.backflow_working_state == 4)
  {
    tmp = curve_temp_buf[3] * 10 / 25 ;
    if(tmp == 0)
      tmp = 1;
    pwm.percent = 75 + min_count * 10 / tmp;
    if(min_count == (curve_temp_buf[3] - 10))
    {
      pwm.backflow_temp_tmp = 0;
    }
    else if(min_count == curve_temp_buf[3])
    {
      pwm.backflow_working_state = 0;
      pwm.percent = 100;
      pwm.end();
    }
  }
  
}

void ms25_tic()
{
  oled_flg = 1;
  
}

void ms50_tic()
{
  adc.get();
  
}

void ms100_tic()
{
  
  if(eeprom.write_t < 21)
  {
    eeprom.write_t ++;

    if(eeprom.write_t == 20)
    {
      if(eeprom.write_flg)
        eeprom.write_flg = 2;
      if(ui.show_temp_mode == show_set_temp)
      {
        ui.show_temp_mode = show_now_temp;
        ui.temp_move_flg = 1;
      }
    }
  }
}


void Ticker_init()
{
  ticker25ms.attach_ms(25,ms25_tic);
  ticker100_ms.attach_ms(100,ms100_tic);
  ticker50_ms.attach_ms(50,ms50_tic);

  ticker1s.attach(1,s1_tic);
}

