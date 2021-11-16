/*
 * @Author: Pomin
 * @Date: 2021-10-03 13:49:01
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-10 12:17:50
 * @Description:
 */
#ifndef __PWM_H
#define __PWM_H
#include "stm8l15x.h"
#include "stm8l15x_tim1.h"
#include "stm8l15x_tim2.h"
#include "stm8l15x_tim3.h"

#define PERIOD_MAX      100

void pwm_init(void);


#endif
