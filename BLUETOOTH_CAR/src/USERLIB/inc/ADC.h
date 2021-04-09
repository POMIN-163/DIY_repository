#ifndef __ADC_H_
#define __ADC_H_
#include "stm32f10x.h"
#include "SYS.h"

void DianCi_Init(void);
u16 Get_Adc(ADC_TypeDef* ADCx, u8 ch);
u16 Get_Adc_Average(ADC_TypeDef* ADCx, u8 ch,u8 times);

#endif
