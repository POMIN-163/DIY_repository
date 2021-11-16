/*
 * @Author: Pomin
 * @Date: 2021-10-03 14:06:15
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-04 12:24:29
 * @Description:
 */
#include "key.h"

#define KEY1 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define KEY2 GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)

typedef struct Button {
    uint8_t last_status;
    uint8_t curr_status;
    uint8_t(*read)(void);
    void (*event)(void);
} Button;

Button key_arr[3];

uint8_t key0_read(void) { return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4); }
uint8_t key1_read(void) { return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5); }
uint8_t key2_read(void) { return GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6); }

void key_init(void) {
    /* Enable ADC1 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_ADC1, ENABLE);

    ADC_Init(ADC1, ADC_ConversionMode_Continuous, ADC_Resolution_12Bit, ADC_Prescaler_2);
    ADC_SamplingTimeConfig(ADC1, ADC_Group_SlowChannels, ADC_SamplingTime_384Cycles);
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);

    /* Enable ADC1 Channels 11 */
    ADC_ChannelCmd(ADC1, ADC_Channel_11, ENABLE); /* connected to potentiometer */

    /* Start ADC1 Conversion using Software trigger*/
    ADC_SoftwareStartConv(ADC1);

    GPIO_Init(GPIOB, GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6, GPIO_Mode_In_PU_No_IT);
    // KEY

    for (int i = 0; i < 3; i++) {
        key_arr[i].curr_status = key_arr[i].last_status = 1;
    }
    key_arr[0].read = key0_read;
    key_arr[1].read = key1_read;
    key_arr[2].read = key2_read;
}

void key_scan(uint8_t *msg) {
    for (int i = 0; i < 3; i++) {
        key_arr[i].last_status = key_arr[i].curr_status;
        key_arr[i].curr_status = key_arr[i].read();
        if (key_arr[i].curr_status && !key_arr[i].last_status) {
            *msg = i + 1;
            // printf("key %d press\r\n", i);
        }
    }
}
