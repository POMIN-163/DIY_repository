#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x.h"

void _GPIO_Config(
  GPIO_TypeDef* GPIOx,
  uint32_t GPIO_Pin,
  GPIOMode_TypeDef GPIO_Mode,
  GPIOSpeed_TypeDef GPIO_Speed
);
void _GPIO_Clock(uint32_t RCC_AHB1Periph, FunctionalState NewState);
void _GPIO_SetBit(GPIO_TypeDef* GPIOx, uint32_t GPIO_Pin, uint8_t NewState);
void _GPIO_PinLock(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
