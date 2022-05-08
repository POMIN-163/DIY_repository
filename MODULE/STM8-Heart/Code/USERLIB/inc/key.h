/*
 * @Author: Pomin
 * @Date: 2021-10-03 14:06:18
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-08 13:26:05
 * @Description:
 */
#ifndef __KEY_H
#define __KEY_H
#include "stm8l15x.h"
#include "stm8l15x_adc.h"
#include "stm8l15x_syscfg.h"

void key_init(void);
void key_scan(uint8_t *msg);

typedef enum {
    NONE = 0,
    LEFT,
    RIGHT
} key_msg_t;

#endif

