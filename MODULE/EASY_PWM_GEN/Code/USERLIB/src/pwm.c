/*
 * @Author: Pomin
 * @Date: 2021-10-03 13:49:05
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-18 23:48:23
 * @Description:
 */
#include "pwm.h"


#define TIM1_PERIOD     65535
#define TIM1_PRESCALER  1

__IO uint16_t CCR1_Val = 32768;
__IO uint16_t CCR2_Val = 16384;

static void CLK_Config(void);
static void GPIO_Config(void);
static void PWM_Config(void);

void pwm_init(void) {
    CLK_Config();
    GPIO_Config();
    PWM_Config();
}

static void CLK_Config(void) {

    CLK_PeripheralClockConfig(CLK_Peripheral_TIM1, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM3, ENABLE);
}

static void GPIO_Config(void) {
    /* TIM1 */
    GPIO_Init(GPIOD, GPIO_Pin_2 | GPIO_Pin_4, GPIO_Mode_Out_PP_Low_Fast);
    /* TIM2 */
    GPIO_Init(GPIOB, GPIO_Pin_0 | GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Fast);
    /* TIM3 */
    GPIO_Init(GPIOD, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_Out_PP_Low_Fast);
}

static void PWM_Config(void) {

    TIM1_TimeBaseInit(15, TIM1_CounterMode_Up, PERIOD_MAX, 0);
    // 1 000 000 Hz / 100  =  10 000Hz (16M / x)
    TIM2_TimeBaseInit(TIM2_Prescaler_16, TIM2_CounterMode_Up, PERIOD_MAX);

    TIM3_TimeBaseInit(TIM3_Prescaler_16, TIM3_CounterMode_Up, PERIOD_MAX);

    TIM1_OC1Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
                 PERIOD_MAX / 2, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Reset,
                 TIM1_OCNIdleState_Set);
    TIM1_OC1PreloadConfig(DISABLE);
    TIM1_OC2Init(TIM1_OCMode_PWM1, TIM1_OutputState_Enable, TIM1_OutputNState_Disable,
                 PERIOD_MAX / 2, TIM1_OCPolarity_High, TIM1_OCNPolarity_Low, TIM1_OCIdleState_Reset,
                 TIM1_OCNIdleState_Set);
    TIM1_OC2PreloadConfig(DISABLE);

    TIM2_OC1Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, PERIOD_MAX / 2, TIM2_OCPolarity_High, TIM2_OCIdleState_Reset);
    TIM2_OC1PreloadConfig(DISABLE);
    TIM2_OC2Init(TIM2_OCMode_PWM1, TIM2_OutputState_Enable, PERIOD_MAX / 2, TIM2_OCPolarity_High, TIM2_OCIdleState_Reset);
    TIM2_OC2PreloadConfig(DISABLE);

    TIM3_OC1Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, PERIOD_MAX / 2, TIM3_OCPolarity_High, TIM3_OCIdleState_Reset);
    TIM3_OC1PreloadConfig(DISABLE);
    TIM3_OC2Init(TIM3_OCMode_PWM1, TIM3_OutputState_Enable, PERIOD_MAX / 2, TIM3_OCPolarity_High, TIM3_OCIdleState_Reset);
    TIM3_OC2PreloadConfig(DISABLE);

    TIM1_ARRPreloadConfig(ENABLE);
    TIM2_ARRPreloadConfig(ENABLE);
    TIM3_ARRPreloadConfig(ENABLE);

    TIM1_CtrlPWMOutputs(ENABLE);
    TIM2_CtrlPWMOutputs(ENABLE);
    TIM3_CtrlPWMOutputs(ENABLE);

    TIM1_Cmd(ENABLE);
    TIM2_Cmd(ENABLE);
    TIM3_Cmd(ENABLE);
}


