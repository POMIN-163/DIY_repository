#ifndef __TIMER_H_
#define __TIMER_H_
#include "stm32f10x.h"
#include "system.h"
#include "global.h"
void TIM3__Init(u16 arr,u16 psc);
void Motor_PWM_Init(u16 arr,u16 psc);
void Motor_Ctrl(void);
void Ctrl_Flip(u8 _mode);
//void TIM4__Init(u16 arr,u16 psc);
#endif
