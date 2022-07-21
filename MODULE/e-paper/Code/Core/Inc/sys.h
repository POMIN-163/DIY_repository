#ifndef __SYS_H
#define __SYS_H
#include "stm32f1xx_hal.h"

#define BIT_REG(Reg, Bit) (*((uint32_t volatile*)                     \
    (0x42000000u + (((uint32_t) & (Reg) - (uint32_t)0x40000000u) << 5) +  \
    (((uint32_t)(Bit)) << 2))))

#define PAout(n) BIT_REG (GPIOA->ODR, n)
#define PBout(n) BIT_REG (GPIOB->ODR, n)
#define PCout(n) BIT_REG (GPIOC->ODR, n)
#define PDout(n) BIT_REG (GPIOD->ODR, n)
#define PEout(n) BIT_REG (GPIOE->ODR, n)

#define PAin(n) BIT_REG (GPIOA->IDR, n)
#define PBin(n) BIT_REG (GPIOB->IDR, n)
#define PCin(n) BIT_REG (GPIOC->IDR, n)
#define PDin(n) BIT_REG (GPIOD->IDR, n)
#define PEin(n) BIT_REG (GPIOE->IDR, n)

#endif
