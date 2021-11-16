/*
 * @Author: Pomin
 * @Date: 2021-10-03 13:36:24
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-18 23:46:09
 * @Description:
 */
#ifndef __OLED_H
#define __OLED_H
#include "stm8l15x.h"//STM8L051/151等系列共用库函数

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据
#define OLED_MODE 0

/****************时钟*********************/
#define OLED_SCLK_PORT  (GPIOC)
#define OLED_SCLK_PINS  (GPIO_Pin_1)

/****************数据*********************/
#define OLED_SDIN_PORT  (GPIOC)
#define OLED_SDIN_PINS  (GPIO_Pin_0)

/****************复位*********************/
// #define OLED_RST_PORT  (GPIOB)
// #define OLED_RST_PINS  (GPIO_Pin_7)

/****************数据/命令*********************/
// #define OLED_DC_PORT  (GPIOB)
// #define OLED_DC_PINS  (GPIO_Pin_6)

/****************片选*********************/
// #define OLED_CS_PORT  (GPIOB)
// #define OLED_CS_PINS  (GPIO_Pin_5)

// #define LED_PORT  (GPIOD)
// #define LED_PINS  (GPIO_Pin_0)

#define OLED_SCLK_Clr() GPIO_WriteBit(OLED_SCLK_PORT, OLED_SCLK_PINS, (BitAction)0)
#define OLED_SCLK_Set() GPIO_WriteBit(OLED_SCLK_PORT, OLED_SCLK_PINS, (BitAction)1)

#define OLED_SDIN_Clr() GPIO_WriteBit(OLED_SDIN_PORT, OLED_SDIN_PINS, (BitAction)0)
#define OLED_SDIN_Set() GPIO_WriteBit(OLED_SDIN_PORT, OLED_SDIN_PINS, (BitAction)1)

#define Max_Column	128
#define Max_Row		32

//-----------------OLED端口定义----------------

void delay_ms(unsigned int ms);

void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Init(void);
void OLED_Clear(void);
void OLED_DrawPoint(u8 x, u8 y, u8 t);
void OLED_Fill(u8 x1, u8 y1, u8 x2, u8 y2, u8 dot);
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode);
void OLED_ShowString(u8 x, u8 y, u8* p, u8 size, u8 mode);
void OLED_ShowCHinese(u8 x, u8 y, u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);
void LED_ON(void);
void LED_OFF(void);


#endif




