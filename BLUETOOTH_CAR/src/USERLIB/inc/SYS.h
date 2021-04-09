#ifndef __SYS_H_
#define __SYS_H_
#include "stm32f10x.h"

//***本部分代码参考与正点原子,不做商业用途***//

void _SYS_Init(uint32_t SysTick_CLKSource_HCLK_, u8 SYSCLK);
void _SYS_Delay_us(u32 us);
void _SYS_Delay_xms(u16 ms);
void _SYS_Delay_ms(u16 ms);


#endif
