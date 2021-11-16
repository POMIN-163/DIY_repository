/*
 * @Author: Pomin
 * @Date: 2021-10-03 13:36:23
 * @Github: https://github.com/POMIN-163
 * @LastEditTime: 2021-10-10 12:36:27
 * @Description:
 */
#include "stm8l15x.h"
#include "stm8l15x_it.h"
#include "stm8l15x_flash.h"
// #include "stdio.h"
#include "oled.h"
#include "pwm.h"
#include "key.h"

// char buff[10];
// #define printf_lcd(line, format, mode, ...) do {    \
//     sprintf(buff, format, ##__VA_ARGS__);     \
//     OLED_ShowString(36, line * 2, (u8 *)buff, 16, mode); \
// } while(0)

#define LED1_H GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define LED1_L GPIO_ResetBits(GPIOC, GPIO_Pin_5)
#define LED2_H GPIO_SetBits(GPIOC, GPIO_Pin_6)
#define LED2_L GPIO_ResetBits(GPIOC, GPIO_Pin_6)

u8 led1 = 0;
u8 led2 = 1;

uint8_t ch_act = 0;
uint8_t set_act = 0;
uint8_t ch_or_set = 1;

#pragma pack(1)

typedef struct {
    uint8_t duty;
    uint8_t freq;
} set_var_t;

#pragma pack()

uint8_t var_arr[6] = { 1, 2, 3, 4, 5, 6, };

void Init(void);

void set_pwm(void) {
    /* 1 000 000 / period = freq, period * duty / 100 = value ==> vlaue = 1 000 000 / freq * duty / 100 */
    /* (float)((set_var_t*)&var_arr[0])->freq 不是实际频率，实际频率是其 100 倍 */
    TIM1_SetCompare1((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[0])->freq * (float)((set_var_t*)&var_arr[0])->duty));
    TIM1_SetCompare2((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[0])->freq * (float)((set_var_t*)&var_arr[0])->duty));
    TIM2_SetCompare1((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[2])->freq * (float)((set_var_t*)&var_arr[2])->duty));
    TIM2_SetCompare2((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[2])->freq * (float)((set_var_t*)&var_arr[2])->duty));
    TIM3_SetCompare1((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[4])->freq * (float)((set_var_t*)&var_arr[4])->duty));
    TIM3_SetCompare2((uint16_t)(100.0f / (float)((set_var_t*)&var_arr[4])->freq * (float)((set_var_t*)&var_arr[4])->duty));

    TIM1_SetAutoreload((uint16_t)(10000.0f / (float)((set_var_t*)&var_arr[0])->freq));
    TIM2_SetAutoreload((uint16_t)(10000.0f / (float)((set_var_t*)&var_arr[2])->freq));
    TIM3_SetAutoreload((uint16_t)(10000.0f / (float)((set_var_t*)&var_arr[4])->freq));
}

void Init(void) {
    u8 i = 0;
    /* interrupt */
    disableInterrupts();
    /* clk */
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSE);
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_1);
    /* led */
    GPIO_Init(GPIOC, GPIO_Pin_6, GPIO_Mode_Out_OD_Low_Slow);
    GPIO_Init(GPIOC, GPIO_Pin_5, GPIO_Mode_Out_OD_Low_Slow);
    /* flash */
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    while (FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET) { }
    FLASH_Unlock(FLASH_MemType_Data);
    while (FLASH_GetFlagStatus(FLASH_FLAG_DUL) == RESET) { }
    for (i = 0; i < 8; i++) {
        var_arr[i] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i);
    }
    /* key pwm oled */
    key_init();

    pwm_init();
    set_pwm();

    OLED_Init();
    OLED_Clear();
}
/**
 * @brief 奇数通道可调频率和占空比 12、34、56通道的频率占空比相同 100 - 10 000 可调
 *
**/
void main(void) {
    u8  i = 0;
    u8  key_msg = 0;
    u16 adc_val_now = 0;
    u16 adc_val_last = 0;
    char disp_freq[10] = "F:00000HZ";
    char disp_duty[10] = "DUTY:000/";

    Init();

    while (1) {
        /* key and led */
        GPIO_ToggleBits(GPIOC, GPIO_Pin_6);
        key_scan(&key_msg);
        /* adc transform to 1 ~ 100 */
        adc_val_now = (4095 - ADC_GetConversionValue(ADC1));
        adc_val_now = (adc_val_now > 4000) ? 4000 : adc_val_now;
        adc_val_now = (u16)((float)adc_val_now / 40.0f);
        /* adc changed */
        if (adc_val_last != adc_val_now && ch_or_set == 0) {
            adc_val_last = adc_val_now;
            if (set_act == 0) {
                ((set_var_t*)&var_arr[ch_act * 2])->duty = adc_val_now;
                set_pwm();
            } else if (set_act == 1) {
                ((set_var_t*)&var_arr[ch_act * 2])->freq = adc_val_now ? adc_val_now : 1;
                set_pwm();
            }
            /* write flash */
            for ( i = 0; i < 8; i++) {
                FLASH_ProgramByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + i, var_arr[i]);
                while (FLASH_GetFlagStatus(FLASH_FLAG_HVOFF) == RESET) { }
            }
        }
        /* to reduce memory usage, the "oledfont.h" has changed */
        OLED_ShowString(0, 0, "./012.", 8, ch_act != 0);
        OLED_ShowString(0, 1, "./034.", 8, ch_act != 1);
        OLED_ShowString(0, 2, "./056.", 8, ch_act != 2);

        if (key_msg != NONE) {
            switch (key_msg) {
            case LEFT:
                if (ch_or_set) {
                    --ch_act;
                    ch_act %= 3;
                } else {
                    --set_act;
                    set_act %= 2;
                }
                break;
            case ENTER:
                ch_or_set ^= 1;
                break;
            case RIGHT:
                if (ch_or_set) {
                    ++ch_act;
                    ch_act %= 3;
                } else {
                    ++set_act;
                    set_act %= 2;
                }
                break;
            default:
                break;
            }
            key_msg = NONE;
        }

        disp_freq[2] = ((set_var_t*)&var_arr[ch_act * 2])->freq / 100 + '0';
        disp_freq[3] = (((set_var_t*)&var_arr[ch_act * 2])->freq / 10) % 10 + '0';
        disp_freq[4] = ((set_var_t*)&var_arr[ch_act * 2])->freq % 10 + '0';

        disp_duty[5] = ((set_var_t*)&var_arr[ch_act * 2])->duty / 100 + '0';
        disp_duty[6] = (((set_var_t*)&var_arr[ch_act * 2])->duty / 10) % 10 + '0';
        disp_duty[7] = ((set_var_t*)&var_arr[ch_act * 2])->duty % 10 + '0';

        OLED_ShowString(37, 0, (u8 *)disp_freq, 16, !(ch_or_set == 0 && set_act == 1));
        OLED_ShowString(37, 2, (u8 *)disp_duty, 16, !(ch_or_set == 0 && set_act == 0));
        // printf_lcd(0, "F:%03d00HZ", !(ch_or_set == 0 && set_act == 1), ((set_var_t*)&var_arr[ch_act * 2])->freq);
        // printf_lcd(1, "DUTY:%03d/", !(ch_or_set == 0 && set_act == 0), ((set_var_t*)&var_arr[ch_act * 2])->duty);
        delay_ms(20);
    }
}
/*
INTERRUPT_HANDLER(TIM1_UPD_OVF_TRG_BRK_IRQHandler, 23) {
    if (TIM1_GetITStatus(TIM1_IT_Update) == SET) {
        OLED_ShowString(0, 0, "tim1");
    }
    TIM1_ClearFlag(TIM1_FLAG_Update);
}
INTERRUPT_HANDLER(TIM2_UPD_OVF_TRG_BRK_IRQHandler, 19) {
    if (TIM2_GetITStatus(TIM2_IT_Update) == SET) {
        OLED_ShowString(0, 0, "tim2");
    }
    TIM2_ClearFlag(TIM2_FLAG_Update);
}
INTERRUPT_HANDLER(TIM3_UPD_OVF_TRG_BRK_IRQHandler, 21) {
    if (TIM3_GetITStatus(TIM3_IT_Update) == SET) {
        OLED_ShowString(0, 0, "tim3");
    }
    TIM3_ClearFlag(TIM3_FLAG_Update);
}
*/


