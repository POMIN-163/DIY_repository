#include "SYS.h"

u8  fac_us;
u16 fac_ms;

void _SYS_Init(uint32_t SysTick_CLKSource_HCLK_, u8 SYSCLK) {
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_);
    if(SysTick_CLKSource_HCLK_ == SysTick_CLKSource_HCLK_Div8) {
        fac_us = SYSCLK / 8;
        fac_ms = (u16)fac_us * 1000;
    }else{
        fac_us = SYSCLK;
        fac_ms = (u16)fac_us * 1000;
    }
}
void _SYS_Delay_us(u32 us) {
    u32 temp;
    SysTick->LOAD = us * fac_us;                //ʱ�����
    SysTick->VAL = 0x00;                        //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;  //��ʼ����
    do {
        temp = SysTick->CTRL;
    }
    while((temp&0x01) && !(temp&(1<<16)));      //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;  //�رռ�����
    SysTick->VAL = 0X00;                        //��ռ�����
}
void _SYS_Delay_xms(u16 ms) {
    u32 temp;
    SysTick->LOAD = (u32)ms * fac_ms;            //ʱ�����(SysTick->LOADΪ24bit)
    SysTick->VAL = 0x00;                         //��ռ�����
    SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk ;   //��ʼ����
    do{
        temp = SysTick->CTRL;
    }while((temp&0x01) && !(temp&(1<<16)));      //�ȴ�ʱ�䵽��
    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;   //�رռ�����
    SysTick->VAL = 0X00;                         //��ռ�����
}
void _SYS_Delay_ms(u16 ms) {
    u8 repeat = ms / 540;
    u16 remain = ms % 540;
    while(repeat) {
        _SYS_Delay_xms(540);
        repeat--;
    }
    if(remain) _SYS_Delay_xms(remain);
}

