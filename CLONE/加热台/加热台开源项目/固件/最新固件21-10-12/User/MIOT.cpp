#include "MIOT.h"
#include <Blinker.h>

MIOT miot;

BlinkerButton Button1(BUTTON_1);
BlinkerButton Button2(BUTTON_2);
BlinkerButton Button3(BUTTON_3);
BlinkerButton Button4(BUTTON_4);
BlinkerButton Button5(BUTTON_5);
BlinkerSlider Slider1(Slider_1);





void button1_callback(const String & state)//按键事件
{
    if(!pwm.power)
    {
        pwm.begin();
        ui.oled_display_set();
    }

}

void button2_callback(const String & state)//按键事件
{
    if(pwm.power)
    {
        pwm.end();
        ui.oled_display_set();
    }
    
}
void button3_callback(const String & state)//按键事件
{
    if(!pwm.temp_mode)
    {
        pwm.temp_mode = 1;
        ui.temp_mode_flg = 1;
        if(pwm.power)
        {
            pwm.end();
            eeprom.write_flg = 1;
            eeprom.write_t = 0;
        }
        ui.oled_display_set();
    }
    
}

void button4_callback(const String & state)//按键事件
{
    if(pwm.temp_mode)
    {
        pwm.temp_mode = 0;
        ui.temp_mode_flg = 1;
        if(pwm.power)
        {
            pwm.end();
            eeprom.write_flg = 1;
            eeprom.write_t = 0;
        }
        ui.oled_display_set();
    }
    
}

void button5_callback(const String & state)//按键事件
{
    pwm.fan();

    
}

void slider1_callback(int32_t value)//滑块处理
{
    if(pwm.temp_mode)
    {
        if(value < 40)
            value = 40;
        else if(value > 250)
            value = 250;
        pwm.temp_buf = value;

        ui.temp_move_flg = 1;
        eeprom.write_t = 0;
        eeprom.write_flg = 1;
        ui.show_temp_mode = show_set_temp;
        ui.oled_display_set();
    }

}



void miotPowerState(const String & state)//电源事件
{

    if (state == BLINKER_CMD_ON) {
        if(!pwm.power)
        {
            pwm.begin();
            ui.oled_display_set();
        }
    }
    else if (state == BLINKER_CMD_OFF) {
        if(pwm.power)
        {
            pwm.end();
            ui.oled_display_set();
        }

    }
}


void miotColor(int32_t color)
{
    if(pwm.temp_mode)
    {
        color &= 0xffffff;

        switch (color)
        {
            case 9498256:pwm.temp_buf = qianlvse;break;
            case 16711935:pwm.temp_buf = zihongse;break;
            case 16761035:pwm.temp_buf = fenhongse;break;
            case 16738740:pwm.temp_buf = fense;break;
            case 10494192:pwm.temp_buf = zise;break;
            case 8900331:pwm.temp_buf = tianlanse;break;
            case 255:pwm.temp_buf = lanse;break;
            case 65535:pwm.temp_buf = qingse;break;
            case 65280:pwm.temp_buf = lvse;break;
            case 16753920:pwm.temp_buf = juhuangse;break;
            case 16776960:pwm.temp_buf = huangse;break;
            case 16711680:pwm.temp_buf = hongse;break;
        }
        ui.temp_move_flg = 1;
        eeprom.write_t = 0;
        eeprom.write_flg = 1;
        ui.show_temp_mode = show_set_temp;
        ui.oled_display_set();
    }

}



void miotMode(uint8_t mode)
{

    if (mode == BLINKER_CMD_MIOT_DAY) {//日光模式     
        if(pwm.temp_mode)
        {
            pwm.temp_buf = MIOT_DAY;
            ui.temp_move_flg = 1;
            ui.show_temp_mode = show_set_temp;
            eeprom.write_t = 0;
            eeprom.write_flg = 1;
            ui.oled_display_set();
        }
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT) {//夜光模式
        if(pwm.temp_mode)
        {
            pwm.temp_mode = 0;
            ui.temp_mode_flg = 1;
            ui.oled_display_set();
        }

    }
    else if (mode == BLINKER_CMD_MIOT_COLOR) {//彩光模式
        if(!pwm.temp_mode)
        {
            pwm.temp_mode = 1;
            ui.temp_mode_flg = 1;
            ui.oled_display_set();
        }
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH) {//温馨模式

    }
    else if (mode == BLINKER_CMD_MIOT_TV) {//电视模式

    }
    else if (mode == BLINKER_CMD_MIOT_READING) {//阅读模式

    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER) {//电脑模式

    }

}

void MIOT::begin()
{

    if(miot_able){
        open_flg = 1;
        if(WiFi.status() != WL_CONNECTED)
        {
            wifi_conect_flg = setwifi.power_on_conect();
            if(!wifi_conect_flg)
            {
                WiFi.beginSmartConfig();
                return;
            }
        }
    }
    else
        return;

    

    if(strlen(&wifima.blinker_id[0]) == 12)
        Blinker.begin(&wifima.blinker_id[0]);
    else
        return;

    

    #ifdef DEBUG
    {
        BLINKER_DEBUG.stream(Serial);
    }
    #else
    {
        const char *p = &wifima.blinker_id[0];
        Blinker.begin(p);
    }
    #endif

    
    
    Button1.attach(button1_callback);
    Button2.attach(button2_callback);
    Button3.attach(button3_callback);
    Button4.attach(button4_callback);
    Button5.attach(button5_callback);
    BlinkerMIOT.attachPowerState(miotPowerState);
    BlinkerMIOT.attachColor(miotColor);
    Slider1.attach(slider1_callback);
    BlinkerMIOT.attachMode(miotMode);
}

void MIOT::run_task()
{
    if(miot_able && WiFi.status() == WL_CONNECTED)
    {
        if(wifi_conect_flg == false)
        {
            begin();
            wifi_conect_flg = true;
        }
        Blinker.run();
    }
        
}



























