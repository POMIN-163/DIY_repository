#ifndef __LM393_H
#define __LM393_H
#include "stm32f10x.h"

void lm393Init(void);
void measureVelocity(void);
void tim2IntInit(unsigned int arr,unsigned int psc);
unsigned int getLeftSpeed(void);
unsigned int getRightSpeed(void);

#endif



