#include "OLED.h"
#include "DEBUG.H"
#include "board.h"
#include "dino.h"

void Pin_Init() {
    LED  = 0;
    BEEP = 0;
    mDelaymS(300);
    BEEP = 1;
    mDelaymS(100);
    BEEP = 0;
    SW1  = SW2 = SW3 = SW4 = SW5 = SW6 = 1;
}

unsigned char r = 0;

void time0_irs(void) interrupt 1 {
    r++;
    Dino_Tick();
}

void Time_init() {
    TMOD = 0;
    TH0 = (UINT8X)(65532 >> 8);
    TL0 = (UINT8X) 65532;
    TF0 = 0;
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}

unsigned char Ramod(unsigned char mod) { return r % mod; }

/*******************************************************************************
 * 函 数 名       : main
 * 函数功能		 : 主函数
 * 输    入       : 无
 * 输    出    	 : 无
 *******************************************************************************/
void main() {
    CfgFsys();          // CH552时钟选择16M配置
    Pin_Init();

    OLED_Init();
    OLED_DisplayStr(100, 0, "Game");
    Time_init();

    OLED_DisplayStr(35, 3, "IS READY?");
    while(SW)
        ;
    OLED_DisplayStr(35, 3, "         ");
    Dino_Start();

    while (1) {
        mDelaymS(1);
    }
}