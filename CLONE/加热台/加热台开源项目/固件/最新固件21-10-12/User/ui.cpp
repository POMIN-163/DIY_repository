#include "User.h"
#include "bmp.h"
UI ui;


#define page2_menu_num_max 7

#define temp_mode_menu_num 0
#define backflow_menu_num 1
#define constant_temperature_menu_num 2
#define iot_menu_num 3
#define oled_light_menu_num 4
#define use_menu_num 5
#define about_menu_num 6

s8 page_num = 0;



/*菜单页定义*/
s8 page2_menu_num = 0;
s8 page2_move_tmp = 0;
s8 page2_move_flg = 0;


const char page2_menu0[] = {"温控模式"};
const char page2_menu1[] = {"回流参数"};
const char page2_menu2[] = {"恒温参数"};
const char page2_menu3[] = {"物联网"};
const char page2_menu4[] = {"屏幕亮度"};
const char page2_menu5[] = {"使用说明"};
const char page2_menu6[] = {"关于"};

const char *page2_str_ptr[] = {page2_menu0,page2_menu1,page2_menu2,page2_menu3,page2_menu4,page2_menu5,page2_menu6};
const char *page2_bmp_ptr[] = {page2_bmp0,page2_bmp1,page2_bmp2,page2_bmp3,page2_bmp4,page2_bmp5,page2_bmp6};
/************************/

/*设置页定义*/
s8 page3_switch_flg = 0;


const char menu0_option0[] = {"回流焊模式"};
const char menu0_option1[] = {"恒温模式"};

const char menu1_option0[] = {"活性区"};
const char menu1_option1[] = {"回流区"};
const char menu1_option_s[] = {"秒"};
const char menu1_option_t[] = {"摄"};
s8 temp_mode0_option = 0;//回流曲线设置项 0：活性区温度 1：活性区时间 2：回流区温度 3：回流区时间
short curve_temp_buf[] = {//0：保温区温度140-200 1：保温区时长60-120 2：回流区温度220-250  3：回流区时长 30-90
    160,90,230,60
};

const char menu2_option0[] = {"分钟"};

const char menu3_option0_0[] = {"关闭"};
const char menu3_option0_1[] = {"开启"};
const char menu3_option1_0[] = {"设备密匙"};
const char menu3_option1_1[] = {"请连接至热点"};
const char menu3_option1_2[] = {"QF_HP"};

//const char menu5_option0[] = {"这么简单自己摸索"};

const char menu5_option0_0[] = {"配网使用"};
const char menu5_option0_1[] = {"ESPTouch"};

const char menu6_option0_0[] = {"启凡科创智能加热台"};
const char menu6_option0_1[] = {"Version:V1.3"};

/************************/

void UI::run_task()
{

    wake_sleep_page();

    if(oled_sleep_flg)
        return;
    

    switch (page_num)
    {
        case 1:
        temp_move();
        temp_mode_move();
        heat_move();
        temp_time_switch();
        break;
        case 2:
        page2_move();
        break;
        case 3:
        page3_switch();
        blinker_config();
        break;
    }

    if(page2_move_flg)
        return;

    page_switch(switch_buf);
    
    

    if(!oled_flg)
        return;
    oled_flg = 0;
    oled.clr();

    
    show_page(0,0,page_num);
    write_oled_light();

    oled.refresh();
}




void UI::page1_key()//主界面按键对应功能
{
    if(ec11.sw_buf == sw_click)//单击
    {
        
        if(pwm.power) 
        {
            pwm.end();
        }
        else
        {
            pwm.begin();
        }   
    }
    else if(ec11.sw_buf == sw_long)//长按
    {
        switch_buf = sure;
        page_switch_flg = 1;
        ec11.speed_up(false);
        
    }
    else if(ec11.sw_buf == sw_double)//双击
    {
        pwm.fan();
    }
    if(ec11.ec_buf != sw_clr)
    {
        if(pwm.temp_mode)
        {
            pwm.temp_buf += ec11.ec_buf;

            if(pwm.temp_buf > 250)
                pwm.temp_buf = 250;
            else if(pwm.temp_buf < 40)
                pwm.temp_buf = 40;
            eeprom.write_flg = 1;
            if(show_temp_mode != show_set_temp)
            {
                show_temp_mode = show_set_temp;
                temp_move_flg = 1;
            }
        }
    }

}

void UI::page2_key()//界面2按键对应功能
{
    if(ec11.sw_buf == sw_click)//单击
    {
        // switch_buf = back;
        // page_switch_flg = 1;
        // ec11.speed_up(true);
    }
    else if(ec11.sw_buf == sw_long)//长按
    {
        switch_buf = sure;
        page_switch_flg = 1;
        if(page2_menu_num == constant_temperature_menu_num || page2_menu_num == oled_light_menu_num || page2_menu_num == backflow_menu_num)
            ec11.speed_up(true);
        ec11.double_click(false);
        
    }
    else if(ec11.sw_buf == sw_double)//双击
    {
        switch_buf = back;
        page_switch_flg = 1;
        ec11.speed_up(true);
        
    }
    if(ec11.ec_buf)
    {
        if(page2_move_tmp < 0 && ec11.ec_buf == 1)
            page2_move_tmp = -1;
        else if(page2_move_tmp > 0 && ec11.ec_buf == -1)
            page2_move_tmp = 1;
        page2_move_tmp += ec11.ec_buf;
        page2_move_flg = 1;
    }

}

void UI::page3_key()//界面3按键对应功能
{
    if(ec11.sw_buf)
    {
        if(ec11.sw_buf == sw_click)//单击
        {
            if(page2_menu_num == backflow_menu_num)
            {
                if(!page3_switch_flg)
                {
                    page3_switch_flg = 1;
                    temp_mode0_option ++;
                    if(temp_mode0_option == 4)
                        temp_mode0_option = 0;
                }

            }

            if(page2_menu_num == iot_menu_num)
            {
                if(!page3_switch_flg)
                {
                    if(miot_option_buf)
                    {
                        blinker_config_flg = 1;
                    }
                    else
                    {
                        page3_switch_flg = 1;
                        miot.miot_able = !miot.miot_able;
                    }
                }
            }
        }
        else
        {
            if(wifima.wifima_flg)
            {
                wifima.back_flg = 1;
            }
            else
            {
                switch_buf = back;
                page_switch_flg = 1;
                ec11.speed_up(false);
                if(page2_menu_num < 6)
                    eeprom.write_flg = 1;
                ec11.double_click(true);

                if(pwm.power && page2_menu_num == temp_mode_menu_num) 
                {
                    pwm.end();
                }
                if(page2_menu_num == iot_menu_num)
                {
                    if(miot.miot_able && !miot.open_flg)
                    {
                        eeprom.write(miot_able_add,1);
                        ESP.reset();
                    }
                }
            }
        }
    }


    if(ec11.ec_buf)  
    {
        
        switch (page2_menu_num)
        {
            case temp_mode_menu_num://温控模式
            if(!page3_switch_flg)
                pwm.temp_mode = !pwm.temp_mode;
            break;

            case backflow_menu_num://回流参数
            if(!page3_switch_flg)
            {
                switch (temp_mode0_option)
                {
                    case 0:
                    curve_temp_buf[0] += ec11.ec_buf;
                    if(curve_temp_buf[0] < 130)
                        curve_temp_buf[0] = 130;
                    else if(curve_temp_buf[0] > 200)
                        curve_temp_buf[0] = 200;
                    break;

                    case 1:
                    curve_temp_buf[1] += ec11.ec_buf;
                    if(curve_temp_buf[1] < 60)
                        curve_temp_buf[1] = 60;
                    else if(curve_temp_buf[1] > 120)
                        curve_temp_buf[1] = 120;
                    break;

                    case 2:
                    curve_temp_buf[2] += ec11.ec_buf;
                    if(curve_temp_buf[2] < 220)
                        curve_temp_buf[2] = 220;
                    else if(curve_temp_buf[2] > 260)
                        curve_temp_buf[2] = 260;
                    break;

                    case 3:
                    curve_temp_buf[3] += ec11.ec_buf;
                    if(curve_temp_buf[3] < 30)
                        curve_temp_buf[3] = 30;
                    else if(curve_temp_buf[3] > 90)
                        curve_temp_buf[3] = 90;
                    break;

                }
            }
            break;

            case constant_temperature_menu_num://恒温参数
            pwm.temp_mode1_time += ec11.ec_buf;
            if(pwm.temp_mode1_time < 0)
                pwm.temp_mode1_time = 0;
            else if(pwm.temp_mode1_time > 520)
                pwm.temp_mode1_time = 520;
            break;


            case iot_menu_num://iot
            if(!page3_switch_flg)
               miot_option_buf = !miot_option_buf;
            break;

            case oled_light_menu_num:// 屏幕亮度
            oled_light += ec11.ec_buf;
            if(oled_light < 0)
                oled_light = 0;
            else if(oled_light > 255)
                oled_light = 255;
            write_oled_flg = 1;
            break;


        }
        if(page2_menu_num == temp_mode_menu_num || page2_menu_num == iot_menu_num)
            page3_switch_flg = ec11.ec_buf;
    }
}

void UI::blinker_config()
{
    s8 y;
    if(!blinker_config_flg)
        return;
    blinker_config_flg = 0;

    y = -1;
    for(;;)
    {
        if(y == -33)
            break;
        oled.clr();
        show_page(0,y,3);
        oled.chinese(16,y+32,menu3_option1_1,16,1,0);
        oled.str(44,y+48,menu3_option1_2,16,1,0);
        oled.refresh();
        y --;
        yield();
    }

    if(wifima.startConfigPortal("QF_HP"))
    {
        Serial.println(wifima.blinker_id);
        ec11.int_close();
        if(strlen(&wifima.blinker_id[0]) == 12)
        {
            eeprom.write_str(blinker_id_add,wifima.blinker_id);
            if(miot.miot_able)
                ESP.reset();
        }
        ec11.int_work();
    }

    y = 1;
    for(;;)
    {
        if(y == 32)
            break;
        oled.clr();
        show_page(0,y-32,3);
        oled.chinese(16,y,menu3_option1_1,16,1,0);
        oled.str(44,y+16,menu3_option1_2,16,1,0);
        oled.refresh();
        y ++;
        yield();
    }

}

bool UI::oled_display_set()
{
    if(ui.wake_sleep_change_flg)
        return 1;
    if(!ui.oled_sleep_flg)
    {
        oled_sleep_t = 0;
        return 0;
    }
    else
    {
        ui.wake_sleep_change_flg = 1;
        oled_sleep_t = 0;
        return 1;
    }

}

void ui_key_callb()//按键事件中断处理
{
    if(ui.oled_display_set())
        return;

    switch (page_num)
    {
        case 1:ui.page1_key();break;
        case 2:ui.page2_key();break;
        case 3:ui.page3_key();break;
    }
    eeprom.write_t = 0;
}


void UI::write_oled_light()
{
    if(write_oled_flg)
    {
        write_oled_flg = 0;
        oled.light(oled_light);
    }
}


bool UI::page_switch(u8 mode)
{
    if(!page_switch_flg)
        return 0;
    page_switch_flg = false;


    u8 next_page;
    s8 show_y = 0;
    s8 next_y;
    if(mode == back){
        next_page = page_num - 1;
        next_y = -32;
    }
    else if(mode == sure){
        next_page = page_num + 1;
        next_y = 32;
    }
    else 
        return 0;

    for(;;)
    {
        oled.clr();
        show_page(0,show_y,page_num);
        show_page(0,next_y,next_page);
        oled.refresh();

        if(mode == back)
        {
            show_y ++;
            next_y ++;
        }
        else
        {
            show_y --;
            next_y --;
        }
        if(show_y == 33||show_y == -33)
            break;
        yield();
        
    }

    page_num = next_page;
    
    return 1;
}


void UI::show_page(short x,short y,u8 page)
{
    u8 mode_tmp = ui.show_temp_mode;
    switch (page)
    {
        case 1:
            if(show_temp_mode == show_now_temp)
                show_temp(x,y,93,y+18);
            else
                show_temp(x,y,0,0);
            oled.xy_set(0,0,128,4);
            if(pwm.temp_mode == Re_So)
                oled.chinese(69,y,"回流",16,1,0);
            else
                oled.chinese(69,y,"恒温",16,1,0);
            if(pwm.power)
                oled.BMP(95,y+2,32,28,heating,1);
        break; 
        case 2:
            
            oled.xy_set(0,0,128,4);
            oled.chinese(64,y+8,page2_str_ptr[page2_menu_num],16,1,0);
            oled.BMP(y,page2_bmp_ptr[page2_menu_num]);
        break; 
        case 3:
            switch (page2_menu_num)
            {
                case temp_mode_menu_num://模式设置
                if(pwm.temp_mode)
                    oled.chinese(32,y+8,menu0_option1,16,1,0);
                else
                    oled.chinese(24,y+8,menu0_option0,16,1,0);
                break;

                case backflow_menu_num://回流曲线
                if(page3_switch_flg)
                {
                    show_curve(0,y);
                }
                else
                {
                    show_curve(y,y);
                }
                break;

                case constant_temperature_menu_num://恒温时长
                ui.show_temp_mode = show_temp_mode1_time;
                show_temp(12,y,0,0);
                oled.chinese(84,y+16,menu2_option0,16,1,0);
                ui.show_temp_mode = mode_tmp;
                break;

                case iot_menu_num://物联网
                if(miot_option_buf)
                {
                    oled.chinese(32,y+8,menu3_option1_0,16,1,0);
                }
                else
                {
                    if(miot.miot_able)
                        oled.chinese(48,y+8,menu3_option0_1,16,1,0);
                    else
                        oled.chinese(48,y+8,menu3_option0_0,16,1,0);
                }
                break;

                case oled_light_menu_num://屏幕亮度
                ui.show_temp_mode = show_set_light;
                show_temp(28,y,0,0);
                ui.show_temp_mode = mode_tmp;
                break;

                case use_menu_num://使用说明
                oled.chinese(0,y+8,menu5_option0_0,16,1,0);
                oled.str(64,y+8,menu5_option0_1,16,1,0);
                break;

                case about_menu_num://关于
                oled.chinese(0,y,menu6_option0_0,16,1,1);
                oled.str(32,y + 16,menu6_option0_1,16,1,0);
                break;
            }
        break; 
    }

}

void UI::wake_sleep_page()
{
    if(wake_sleep_change_flg)
    {
        
        oled.display_on();
        if(oled_sleep_flg)
        {
            for(s8 i = 32;i > 0;i --)
            {
                oled.clr();
                show_page(0,i,page_num);
                oled.refresh();
                yield();    
            }  
        }
        else
        {
            oled.roll(0,0,128,4,1,UP,32);
            oled.display_off();
        }
        oled_sleep_flg = !oled_sleep_flg;
        wake_sleep_change_flg = 0;
    }
}

void UI::page3_switch()
{
    
    if(!page3_switch_flg)
        return;
    s8 y;

    if(page3_switch_flg < 0)
    {
        oled.roll(0,0,128,4,2,DOWN,16);
        y = -32;
    }
    else
    {
        if(page2_menu_num == backflow_menu_num)
            oled.roll(72,0,48,4,1,UP,32);
        else
            oled.roll(0,0,128,4,2,UP,16);
        y = 32;
    }

    for(;;)
    {
        if(y == 0)
            break;
        oled.clr();
        show_page(0,y,3);
        oled.refresh();
        if(y < 0)
            y ++;
        else
            y --;
        yield();
    }

    page3_switch_flg = 0;
}

void UI::page2_move()
{

    if(!page2_move_flg)
        return;
    s8 num_tmp ;
    s8 now_y = 0;
    s8 next_y;
    s8 bmp_x;

    if(page2_move_tmp < 0)
    {
        num_tmp = page2_menu_num - 1;
        next_y = -32;
    }
    else
    {
        num_tmp = page2_menu_num + 1;
        next_y = 32;
    }
    if(num_tmp < 0)
        num_tmp = page2_menu_num_max - 1;
    else if(num_tmp == page2_menu_num_max)
        num_tmp = 0;

    
    for(;;)
    {
        if(page2_move_tmp == 0)
        {
            if(next_y < 0)
                page2_move_tmp = -1;
            else
                page2_move_tmp = 1;
        }
        now_y -= page2_move_tmp;
        next_y -= page2_move_tmp;
        if(now_y < -32 || now_y > 32)
            break;
        oled.clr();
        oled.chinese(64,now_y + 8,page2_str_ptr[page2_menu_num],16,1,0);
        oled.BMP(now_y,page2_bmp_ptr[page2_menu_num]);

        oled.chinese(64,next_y + 8,page2_str_ptr[num_tmp],16,1,0);
        oled.BMP(next_y,page2_bmp_ptr[num_tmp]);
        
        oled.refresh();
        yield();
    }
    if(page2_move_tmp < 0)
    {
        page2_move_tmp ++;
    }
    else
    {
        page2_move_tmp --;
    }
    if(page2_move_tmp == 0)
        page2_move_flg = 0;
    page2_menu_num = num_tmp;


}


void UI::heat_move()
{
    if(!heat_flg)
        return;
    heat_flg = 0;
    s8 y;
    if(pwm.power)
        y = 32;
    else
        y = 2;
    oled.xy_set(0,0,128,4);
    for(;;)
    {
        if(pwm.power)
            y --;
        else
            y ++;
        oled.choose_clr(101,0,26,2);
        oled.choose_clr(95,2,32,2);
        oled.BMP(95,y,32,28,heating,1);
        oled.choose_refresh(95,0,32,4);
        if(y == 2 || y == 32)
            return;
        yield();
    }
    
}

void UI::temp_mode_move()
{
    if(!temp_mode_flg)
        return;
    temp_mode_flg = 0;
    s8 y = 0;
    oled.xy_set(0,0,128,2);
    for(y = 0;y >= -16;y --)
    {
        oled.choose_clr(69,0,32,2);
        if(pwm.temp_mode == Re_So)
        {
            oled.chinese(69,y + 16,"回流",16,1,0);
            oled.chinese(69,y,"恒温",16,1,0);
        }
        else
        {
            oled.chinese(69,y,"回流",16,1,0);
            oled.chinese(69,y + 16,"恒温",16,1,0);
        }
        oled.choose_refresh(69,0,32,2);
        delay(8);
    }
}



void UI::temp_move()
{
    if(!temp_move_flg)
        return;
    temp_move_flg = 0;
    s8 temp_x;
    u8 temp_y;
    u8 small_x;
    if(show_temp_mode == show_now_temp)
    {
        temp_x = 0;
    }
    else
    {
        oled.roll(0,0,68,4,2,UP,16);
        temp_x = 68;
    }
    for(;;)
    {
        if(show_temp_mode == show_now_temp)
        {
            temp_x +=4;   
        }
        else if(show_temp_mode == show_set_temp)
        {
            temp_x -=4;
        }
        
        temp_y = temp_x * 1000 / 2125;
        small_x = temp_x * 100 / 283 + 69;


        oled.choose_clr(0,0,68,4);
        oled.choose_clr(68,2,24,2);
        show_temp(temp_x,temp_y,small_x,18);

        oled.choose_refresh(0,0,68,4);
        oled.choose_refresh(68,2,24,2);
        if(temp_x >= 68 || temp_x <= 0)
            break;
        yield();
    }
    if(show_temp_mode == show_now_temp)
    {
        for(temp_y = 32;temp_y > 0; temp_y -=2)
        {
            oled.choose_clr(0,0,68,4);
            show_temp(0,temp_y,0,18);
            oled.choose_refresh(0,0,68,4);
            yield();
        }
    }

}



void UI::show_temp(s8 x,s8 y,s8 xx,s8 yy){
    u8 dat_buf[3];
    u16 tmp;
    if(show_temp_mode == show_now_temp)
    {
        tmp = adc.now_temp;
        if(adc.now_temp <= 38)
            oled.BMP(x + 2,y,less);
    }
    else if(show_temp_mode == show_set_temp)
    {
        tmp = pwm.temp_buf;
    }
    else if(show_temp_mode == show_set_light)
    {
        tmp = oled_light;
    }
    else if(show_temp_mode == show_temp_mode1_time)
    {
        tmp = pwm.temp_mode1_time;
    }
    else 
        return;
    oled.xy_set(68,0,128,4);
    if(pwm.temp_mode)
    {
        if(pwm.temp_reached_flg)
            oled.num(xx,yy,temp_time_buf,3,16,RIGHT,1);
        else
            oled.num(xx,yy,pwm.temp_buf,3,16,RIGHT,1);
    }
    else
        oled.num(xx,yy,pwm.percent,3,16,RIGHT,1);


    dat_buf[0] = tmp/100%10;
    dat_buf[1] = tmp/10%10;
    dat_buf[2] = tmp%10;
    if(show_temp_mode < 3)
        oled.xy_set(0,0,68,4);
    else
        oled.xy_set(0,0,128,4); 
    if(dat_buf[0])
        oled.BMP(x,y,20,32,number[dat_buf[0]],1);
    if(dat_buf[0] || dat_buf[1])
        oled.BMP(x+24,y,20,32,number[dat_buf[1]],1);
    oled.BMP(x+48,y,20,32,number[dat_buf[2]],1);  
    
}



void UI::show_curve(s8 y,s8 data_y)
{
    s8 y_tmp;
    s8 i;
    s8 y_buf;
    s8 x_tmp;
    s8 x_buf;
    s8 tmp;
    s8 last_y;

    y_tmp = -((curve_temp_buf[0] - 40) * 100000 / 671875) + 32;
    y_buf = y_tmp;

    oled.point(11,y_buf + 1 + y,1);

    for(i = 10;i > 0; i--)//升温曲线1
    {
        oled.point(i,++y_buf + y,1);
    }
    x_tmp = curve_temp_buf[1] / 5;
    x_buf = curve_temp_buf[3] / 5;

    for(i = 0;i < x_tmp; i++)//保温曲线
    {
        oled.point(12 + i,y_tmp + y,1);
    }
    x_tmp += 12;

    y_tmp--;

    oled.point(x_tmp ++,y_tmp + y,1);

    y_buf = -((curve_temp_buf[2] - 40) * 100000 / 671875) + 32;
    
    for(i = 10;i > 0; i--)//升温曲线2
    {
        oled.point(x_tmp ++,y_tmp + y,1);
        y_tmp --;
        if(y_tmp == y_buf)
            break;
        if(y_tmp < 6)
        {
            x_buf -= 2;
        }
    }

    oled.point(x_tmp ++,y_tmp +1 + y,1);
    x_buf-=2;

    for(i = 0;i < x_buf;i ++)
    {
        oled.point(x_tmp ++,y_tmp + y,1);
    }
    y_tmp ++;
    oled.point(x_tmp ++,y_tmp + y,1);


    for(i = 0;i < 10; i++)
    {
        oled.point(x_tmp++,y_tmp + y,1);
        y_tmp ++;
        if(x_tmp == 72)
            break;
    }


    if(temp_mode0_option < 2)
        oled.chinese(72,data_y,menu1_option0,16,1,0);
    else
        oled.chinese(72,data_y,menu1_option1,16,1,0);
    
    oled.num(100,data_y + 16,curve_temp_buf[temp_mode0_option],3,16,RIGHT,1);
    
    if(temp_mode0_option == 0 || temp_mode0_option == 2)
        oled.chinese(100,data_y + 16,menu1_option_t,16,1,0);
    else
        oled.chinese(100,data_y + 16,menu1_option_s,16,1,0);

    

}

void UI::temp_time_switch()
{
    if(temp_time_switch_flg == false)
        return;
    if(pwm.temp_reached_flg == false)
        min_count = 0;
    Serial.println(pwm.temp_reached_flg);
    pwm.temp_reached_flg = !pwm.temp_reached_flg;
    oled.roll(69,2,24,2,1,UP,16);

    for(s8 i = 32;i > 18;i--)
    {
        oled.choose_clr(69,2,24,2);
        if(pwm.temp_reached_flg)
            oled.num(93,i,temp_time_buf,3,16,RIGHT,1);
        else
            oled.num(93,i,pwm.temp_buf,3,16,RIGHT,1);
        oled.choose_refresh(69,2,24,2);
        delay(10);
    }
    temp_time_switch_flg = false;
}
