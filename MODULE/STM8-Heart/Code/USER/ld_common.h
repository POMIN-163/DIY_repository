/*
 * @Author: Pomin
 * @Date: 2021-09-11 11:14:45
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-08 00:27:13
 */

#ifndef _LD_COMMON_H
#define _LD_COMMON_H

#include "stdio.h"
#include "stm8l15x.h"

typedef void (*func_t)(void* e);

#define LOW_BYTE (x) (uint8_t)((uint16_t)x)
#define HIGH_BYTE(x) (uint8_t)((uint16_t)x >> 8)

#endif
