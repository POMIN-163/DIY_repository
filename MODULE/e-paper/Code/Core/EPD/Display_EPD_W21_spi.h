#ifndef _DISPLAY_EPD_W21_SPI_
#define _DISPLAY_EPD_W21_SPI_
#include "stm32f10x.h"

//CS-->PA4   SCK-->PA6  SDO--->PC4
// D/C--->PA2	   RES-->PA0
// BUSY--->PC12

#define EPD_W21_MOSI_0	GPIO_ResetBits(GPIOC, GPIO_Pin_4)
#define EPD_W21_MOSI_1	GPIO_SetBits(GPIOC, GPIO_Pin_4)

#define EPD_W21_CLK_0	GPIO_ResetBits(GPIOA, GPIO_Pin_6)
#define EPD_W21_CLK_1	GPIO_SetBits(GPIOA, GPIO_Pin_6)

#define EPD_W21_CS_0	GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define EPD_W21_CS_1	GPIO_SetBits(GPIOA, GPIO_Pin_4)

#define EPD_W21_DC_0	GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define EPD_W21_DC_1	GPIO_SetBits(GPIOA, GPIO_Pin_2)

#define EPD_W21_RST_0	GPIO_ResetBits(GPIOA, GPIO_Pin_1)
#define EPD_W21_RST_1	GPIO_SetBits(GPIOA, GPIO_Pin_1)

#define isEPD_W21_BUSY GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_12) // for solomen solutions


void SPI_Write(unsigned char value);
void EPD_W21_WriteDATA(unsigned char data);
void EPD_W21_WriteCMD(unsigned char command);
void EPD_GPIO_Init(void);

#endif  //#ifndef _MCU_SPI_H_

/***********************************************************
						end file
***********************************************************/
