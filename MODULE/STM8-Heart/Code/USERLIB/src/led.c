/*
 * @Author: Pomin
 * @Date: 2021-10-19 12:49:32
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-19 13:38:30
 * @Description:
 */
#include "led.h"

#define LED_CLK(x) GPIO_WriteBit(GPIOD, GPIO_Pin_2, (BitAction)x)
#define LED_LE(x)  GPIO_WriteBit(GPIOD, GPIO_Pin_3, (BitAction)x)
#define LED_DI(x)  GPIO_WriteBit(GPIOB, GPIO_Pin_0, (BitAction)x)

void led_init(void) {
    GPIO_Init(GPIOD, GPIO_Pin_2, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOD, GPIO_Pin_3, GPIO_Mode_Out_PP_Low_Slow);
    GPIO_Init(GPIOB, GPIO_Pin_0, GPIO_Mode_Out_PP_Low_Slow);
}
void send_byte(unsigned char dat) {
    unsigned char i;
    /* 拉低移位时钟 */
    LED_CLK(0);
    /* 拉低储存时钟 */
    LED_LE(0);
    /* 循环 8 次写入一个字节数据 */
    for (i = 0; i < 8; i++) {
        if (dat & 0x01) {
            LED_DI(1);
        } else {
            LED_DI(0);
        }
        /* 上升沿发送数据 */
        LED_CLK(1);
        LED_CLK(0);
        dat >>= 1;
    }
}
void led_disp(uint8_t* data_arr) {
    unsigned char i, temp;
    temp = 0x7f;
    for (i = 0; i < 8; i++) {
        /* 列选择 */
        send_byte(temp);
        /* 行数据 */
        send_byte(data_arr[i]);
        /* 锁存数据 */
        LED_LE(1);
        LED_LE(0);
        temp = (temp >> 1) | 0x80;
    }
}

