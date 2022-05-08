/*
 * @Author: Pomin
 * @Date: 2021-10-03 14:06:15
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-19 14:38:07
 * @Description:
 */
#include "key.h"

typedef struct Button {
    uint8_t last_status;
    uint8_t curr_status;
    uint8_t(*read)(void);
    void (*event)(void);
} Button;

Button key_arr[2];

uint8_t key0_read(void) { return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4); }
uint8_t key1_read(void) { return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5); }

void key_init(void) {
    GPIO_Init(GPIOA, GPIO_Pin_4, GPIO_Mode_In_PU_No_IT);
    GPIO_Init(GPIOA, GPIO_Pin_5, GPIO_Mode_In_PU_No_IT);

    // KEY
    for (int i = 0; i < 2; i++) {
        key_arr[i].curr_status = key_arr[i].last_status = 1;
    }
    key_arr[0].read = key0_read;
    key_arr[1].read = key1_read;
}

void key_scan(uint8_t *msg) {
    if (*msg == 0) {
        for (int i = 0; i < 2; i++) {
            key_arr[i].last_status = key_arr[i].curr_status;
            key_arr[i].curr_status = key_arr[i].read();
            if (key_arr[i].curr_status && !key_arr[i].last_status) {
                *msg = i + 1;
                // printf("key %d press\r\n", i);
            }
        }
    }
}
