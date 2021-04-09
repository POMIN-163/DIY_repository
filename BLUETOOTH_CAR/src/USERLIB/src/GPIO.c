#include "GPIO.h"

void _GPIO_Config(
    GPIO_TypeDef* GPIOx,
    uint32_t GPIO_Pin,
    GPIOMode_TypeDef GPIO_Mode,
    GPIOSpeed_TypeDef GPIO_Speed
) {
    GPIO_InitTypeDef Temp;
		Temp.GPIO_Mode = GPIO_Mode;
		Temp.GPIO_Pin = GPIO_Pin;
        Temp.GPIO_Speed = GPIO_Speed;
    GPIO_Init(GPIOx, &Temp);
}

