/*
 * @Author: Pomin
 * @Date: 2021-10-03 13:36:23
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-19 15:46:01
 * @Description:
 */
#include "stm8l15x.h"
#include "stm8l15x_flash.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim3.h"
#include "global.h"
#include "led.h"
#include "key.h"
#include "ld_tasks.h"

uint8_t img_heart[][8] = {
    // { 0x00, 0x66, 0x99, 0x81, 0x42, 0x24, 0x18, 0x00 }, // 空心心形
    { 0x00, 0x00, 0x00, 0x3C, 0x18, 0x00, 0x00, 0x00, },
    { 0x00, 0x00, 0x66, 0x7E, 0x3C, 0x18, 0x00, 0x00, },
    { 0x00, 0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, },
    { 0x00, 0x66, 0xFF, 0xFF, 0x7E, 0x3C, 0x18, 0x00, },
    { 0x00, 0x00, 0x66, 0x7E, 0x3C, 0x18, 0x00, 0x00, },
};

uint8_t img_num[][8] = {
    { 0x00, 0x18, 0x24, 0x24, 0x24, 0x24, 0x18, 0x00, }, // 0
    { 0x00, 0x18, 0x38, 0x18, 0x18, 0x18, 0x3C, 0x00, }, // 1
    { 0x00, 0x18, 0x24, 0x04, 0x08, 0x10, 0x3C, 0x00, }, // 2
    { 0x00, 0x18, 0x24, 0x04, 0x18, 0x04, 0x24, 0x18, }, // 3
    { 0x00, 0x08, 0x18, 0x38, 0x28, 0x3C, 0x08, 0x00, }, // 4
    { 0x00, 0x3C, 0x20, 0x38, 0x04, 0x24, 0x18, 0x00, }, // 5
    { 0x00, 0x18, 0x24, 0x38, 0x24, 0x24, 0x18, 0x00, }, // 6
    { 0x00, 0x3C, 0x04, 0x08, 0x10, 0x10, 0x10, 0x00, }, // 7
    { 0x00, 0x18, 0x24, 0x18, 0x24, 0x24, 0x18, 0x00, }, // 8
    { 0x00, 0x18, 0x24, 0x24, 0x1C, 0x24, 0x18, 0x00, }, // 9

};
uint8_t* disp_activity = img_num[1];
uint8_t activity = 0;
uint8_t speed_activity = 1;

void Init(void);

void Init(void) {
    /* interrupt */
    disableInterrupts();
    /* clk */
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
    /* 计时器 */
    /* 16 000 000 / 320 = 50 000  1s */
    TIM1_TimeBaseInit(319, TIM1_CounterMode_Up, 10000, 0);
    TIM1_ClearFlag(TIM1_FLAG_Update);
    TIM1_ITConfig(TIM1_IT_Update, ENABLE);
    enableInterrupts();
    TIM1_Cmd(ENABLE);
    /* 刷屏中断 */
    TIM2_TimeBaseInit(TIM2_Prescaler_64, TIM2_CounterMode_Up, 100);
    TIM2_ClearFlag(TIM2_FLAG_Update);
    TIM2_ITConfig(TIM2_IT_Update, ENABLE);
    enableInterrupts();
    TIM2_Cmd(ENABLE);
    /* 按键扫描 */
    /* 16 000 000 / 64 = 250 000, 2500 / 250 000 = 10ms */
    TIM3_TimeBaseInit(TIM3_Prescaler_64, TIM3_CounterMode_Up, 2500);
    TIM3_ClearFlag(TIM3_FLAG_Update);
    TIM3_ITConfig(TIM3_IT_Update, ENABLE);
    enableInterrupts();
    TIM3_Cmd(ENABLE);

    led_init();
    key_init();
}

u8  key_msg = 0;
void main(void) {
    Init();
    while (1) {
        disp_activity = img_heart[activity];
        if (key_msg != NONE) {
            switch (key_msg) {
            case LEFT:
                if(speed_activity > 0)
                    speed_activity--;
                TIM1_SetAutoreload(5000 + speed_activity * 1000);
                break;
            case RIGHT:
                if(speed_activity < 9)
                    speed_activity++;
                TIM1_SetAutoreload(5000 + speed_activity * 1000);
                break;
            default:
                break;
            }
            key_msg = NONE;
        }
    }
}
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_COM_IRQHandler, 23) {
    if (TIM1_GetITStatus(TIM1_IT_Update) == SET) {
        activity++;
        activity %= 5;
    }
    TIM1_ClearITPendingBit(TIM1_IT_Update);
}
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19) {
    if (TIM2_GetITStatus(TIM2_IT_Update) == SET) {

        led_disp(disp_activity);
    }
    TIM2_ClearITPendingBit(TIM2_IT_Update);
}
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler, 21) {
    if (TIM3_GetITStatus(TIM3_IT_Update) == SET) {
        key_scan(&key_msg);
    }
    TIM3_ClearITPendingBit(TIM3_IT_Update);
}
