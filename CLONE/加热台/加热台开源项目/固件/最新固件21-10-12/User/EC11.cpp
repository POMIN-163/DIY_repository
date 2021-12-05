#include "EC11.h"

Skey ec11;  
Ticker key_tic;

s8 io_sw = -1;
s8 io_sa = -1;
s8 io_sb = -1;

u16 key_time = 10000;
u16 double_time = 10000;
u8 key_scan_buf = 0;
u8 last_buf = 0x03;

u8 speed_up_feel_num = 0;
u16 speed_up_count = 0;

void Skey::key_mode_read()
{
    if(key_scan_buf == 0xd2)
    {
        
        if(speed_up_flg)
            speed_up_feel_num = speed_up_feel_count;
        ec11.ec_buf = -1 - speed_up_count;
        task();
    }
    else if(key_scan_buf == 0xe1)
    {   
        if(speed_up_flg)
            speed_up_feel_num = speed_up_feel_count;
        ec11.ec_buf = 1 + speed_up_count;
        task();
    }  
    key_scan_buf = 0;
}

void key_timer()
{

    if(ec11.state_count < 255)
    {
        ec11.state_count ++;
        if(ec11.state_count == shake_t)
            ec11.state = ec11.state_buf;
    }
    if(!ec11.state && key_time < 10000)
    {
        key_time++;
        if(key_time == key_time_out)
        {
            ec11.sw_buf = sw_long;
            ec11.task();
        }
    }

    if(double_time < 10000)
    {
        double_time ++;
        if(double_time == ec11.double_click_count)
        {
            double_time = 10000;
            ec11.sw_buf = sw_click;
            ec11.task();
        }
    }

    if(ec11.state)
    {
        if(key_time > 5 && key_time < key_time_out)
        {
            if(ec11.sw_buf == sw_clr && double_time > key_double_out)
            {
                double_time = 0;
            }
            else if(double_time < key_double_out)
            {
                double_time = 10000;
                ec11.sw_buf = sw_double;
                ec11.task();
            }
        }
        key_time = 0;
    }
    
    if(speed_up_feel_num)
    {
        speed_up_feel_num --;
        if(speed_up_count < (ec11.speed_up_max_num - 1))
            speed_up_count ++;
    }
    else
    {
        speed_up_count = 0;
    }
}


ICACHE_RAM_ATTR void io_sw_int()
{
    ec11.state_buf = digitalRead(io_sw);
    ec11.state_count = 0;
}


ICACHE_RAM_ATTR void io_sa_sb_int()
{
    u8 tmp = digitalRead(io_sb) << 1;
    tmp |= digitalRead(io_sa);

    if(last_buf == tmp)
        return;

    if(last_buf == 3)
    {
        if(tmp == 1||tmp == 2)
            key_scan_buf = tmp;
        else
            key_scan_buf = 0;
    }
    else if(last_buf == 1||last_buf == 2)
    {  
        if(tmp == 3)
        {
            key_scan_buf |= 0xc0;
            ec11.key_mode_read();
        }
        else if(tmp == 1||tmp == 2)
             key_scan_buf = 0;
    }
    else
    {
        if(tmp == 1||tmp == 2)
            key_scan_buf |= tmp << 4;
        else
            key_scan_buf = 0;
    }
    last_buf = tmp;
    
}

void Skey::attch_task(void (*func)())
{
    attch_p = func;
    work_flg = 1;
    int_work();
    key_time = 10000;
    double_time = 10000;
    key_scan_buf = 0;
    last_buf = 0xff;
    key_tic.attach_ms(1,key_timer);
    if(io_sw > 0)
        pinMode(io_sw,INPUT_PULLUP);
    if(io_sa > 0)
        pinMode(io_sa,INPUT_PULLUP);
    if(io_sb > 0)
        pinMode(io_sb,INPUT_PULLUP);

    
}
void Skey::detach()
{
    key_tic.detach();
    work_flg = 0;
    int_close();
    attch_p = NULL;
}

void Skey::task()
{
    if(attch_p != NULL)
        attch_p();
    sw_buf = sw_clr;
    ec_buf = sw_clr;
}




void Skey::int_work()
{
    if(work_flg)
    {
        attachInterrupt(io_sw, io_sw_int,CHANGE);
        attachInterrupt(io_sa, io_sa_sb_int,CHANGE);
        attachInterrupt(io_sb, io_sa_sb_int,CHANGE);  
    }
}

void Skey::int_close()
{
    detachInterrupt(io_sw);
    detachInterrupt(io_sa);
    detachInterrupt(io_sb);
}

void Skey::set_io(u8 sw,u8 sa,u8 sb)
{
    io_sw = sw;
    io_sa = sa;
    io_sb = sb;
}

void Skey::speed_up(bool val){
    speed_up_flg = val;
}

void Skey::speed_up_max(u16 val){
    speed_up_max_num = val;
}

void Skey::speed_up_feel(u8 val){
    speed_up_feel_count = val;
}

void Skey::double_click(bool val){
    if(val)
        double_click_count = key_double_out;    
    else    
        double_click_count = 1;
}