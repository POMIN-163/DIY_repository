/**
 * @author: Pomin
 * @date: 2022-03-12 18:49:39
 * @github: https://github.com/POMIN-163
 * @lastedit: 2022-07-21 14:44:47
 * @description: 主函数
 **/
#include "common.h"
#include "sys.h"
//EPD
#include "Display_EPD_W21.h"
#include "pic.h"
#include "heart_r.h"
#include "pomin_bw.h"

void user_main(void) {
    printf("hello\r\n");
    while (1) {
        EPD_init();
        HAL_Delay(2000);
        PIC_display_Clear();
        PIC_display(NULL, gImage_heart_r);
        HAL_Delay(2000);
        PIC_display(gImage_pomin_bw, NULL);
        EPD_sleep();
        while(1)
            ;
    }
}

/**************************   以下函数无需关心    ***************************/

int fputc(int ch, FILE *f) {
    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
