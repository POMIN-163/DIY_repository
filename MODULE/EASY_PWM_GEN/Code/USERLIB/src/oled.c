
#include "oled.h"
#include "oledfont.h"
#include "stm8l15x.h"//STM8L051/151等系列共用库函数

void delay_ms(unsigned int ms) {
    unsigned int a;
    while (ms) {
        a = 800;
        while (a--);
        ms--;
    }
    return;
}
void IIC_Start() {
    OLED_SCLK_Set();
    OLED_SDIN_Set();
    OLED_SDIN_Clr();
    OLED_SCLK_Clr();
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop() {
    OLED_SCLK_Set();
    //    OLED_SCLK_Clr();
    OLED_SDIN_Clr();
    OLED_SDIN_Set();

}
void IIC_Wait_Ack() {
    OLED_SCLK_Set();
    OLED_SCLK_Clr();
}
/**********************************************
// IIC Write byte
**********************************************/

void Write_IIC_Byte(unsigned char IIC_Byte) {
    unsigned char i;
    unsigned char m, da;
    da = IIC_Byte;
    OLED_SCLK_Clr();
    for (i = 0;i < 8;i++) {
        m = da;
        m = m & 0x80;
        if (m == 0x80) {
            OLED_SDIN_Set();
        } else OLED_SDIN_Clr();
        da = da << 1;
        OLED_SCLK_Set();
        OLED_SCLK_Clr();
    }
}
/**********************************************
// IIC Write Command
**********************************************/
void Write_IIC_Command(unsigned char IIC_Command) {
    IIC_Start();
    Write_IIC_Byte(0x78);            //Slave address,SA0=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x00);            //write command
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Command);
    IIC_Wait_Ack();
    IIC_Stop();
}
/**********************************************
// IIC Write Data
**********************************************/
void Write_IIC_Data(unsigned char IIC_Data) {
    IIC_Start();
    Write_IIC_Byte(0x78);            //D/C#=0; R/W#=0
    IIC_Wait_Ack();
    Write_IIC_Byte(0x40);            //write data
    IIC_Wait_Ack();
    Write_IIC_Byte(IIC_Data);
    IIC_Wait_Ack();
    IIC_Stop();
}
void OLED_WR_Byte(u8 dat, u8 cmd) {
    if (cmd) {
        Write_IIC_Data(dat);
    } else {
        Write_IIC_Command(dat);

    }
}
void OLED_Set_Pos(unsigned char x, unsigned char y) {
    OLED_WR_Byte(0xb0 + y, OLED_CMD);
    OLED_WR_Byte(((x & 0xf0) >> 4) | 0x10, OLED_CMD);
    OLED_WR_Byte((x & 0x0f) | 0x01, OLED_CMD);
}
//开启OLED显示
void OLED_Display_On(void) {
    OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X14, OLED_CMD);  //DCDC ON
    OLED_WR_Byte(0XAF, OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示
void OLED_Display_Off(void) {
    OLED_WR_Byte(0X8D, OLED_CMD);  //SET DCDC命令
    OLED_WR_Byte(0X10, OLED_CMD);  //DCDC OFF
    OLED_WR_Byte(0XAE, OLED_CMD);  //DISPLAY OFF
}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OLED_Clear(void) {
    u8 i, n;
    for (i = 0;i < 8;i++) {
        OLED_WR_Byte(0xb0 + i, OLED_CMD);  //设置页地址（0~7）
        OLED_WR_Byte(0x00, OLED_CMD);      //设置显示位置—列低地址
        OLED_WR_Byte(0x10, OLED_CMD);      //设置显示位置—列高地址
        for (n = 0;n < 128;n++)OLED_WR_Byte(0, OLED_DATA);
    } //更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/8
void OLED_0(void) {
    OLED_WR_Byte(0, OLED_DATA);
    OLED_WR_Byte(0, OLED_DATA);
}
void OLED_1(void) {
    OLED_WR_Byte(0xff, OLED_DATA);
    OLED_WR_Byte(0xff, OLED_DATA);
}
void OLED_ShowChar(u8 x, u8 y, u8 chr, u8 size, u8 mode) {
    unsigned char c = 0, i = 0;
    if (x > Max_Column - 1) { x = 0;y = y + 2; }

    switch (size)
    {
    case 8:
        c = chr - '.';
        OLED_Set_Pos(x, y);
        for (i = 0;i < 6;i++)
            if(mode)
                OLED_WR_Byte(F6x8[c][i], OLED_DATA);
            else
                OLED_WR_Byte(~F6x8[c][i], OLED_DATA);
        break;
    case 16:
        c = chr - '/';
        OLED_Set_Pos(x, y);
        for (i = 0;i < 8;i++) {
            if(mode) {
                OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
            }
            else {
                OLED_WR_Byte(~F8X16[c * 16 + i], OLED_DATA);
            }
        }
        if(mode) {
            OLED_0();
        }
        else {
            OLED_1();
        }
        OLED_Set_Pos(x, y + 1);
        for (i = 0;i < 8;i++) {
            if(mode) {
                OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
            }
            else {
                OLED_WR_Byte(~F8X16[c * 16 + i + 8], OLED_DATA);
            }
        }
        if(mode) {
            OLED_0();
        }
        else {
            OLED_1();
        }
        break;
    }
    // if (1) {
    //     OLED_Set_Pos(x, y);
    //     for (i = 0;i < 8;i++)
    //         OLED_WR_Byte(F8X16[c * 16 + i], OLED_DATA);
    //     OLED_Set_Pos(x, y + 1);
    //     for (i = 0;i < 8;i++)
    //         OLED_WR_Byte(F8X16[c * 16 + i + 8], OLED_DATA);
    // } else {
    // }
}
//显示一个字符串
void OLED_ShowString(u8 x, u8 y, u8* chr, u8 size, u8 mode) {
    unsigned char j = 0;
    while (chr[j] != '\0') {
        OLED_ShowChar(x, y, chr[j], size, mode);
        x += size / 2 + 2;
        if (x > 120) { x = 0;y += 2; }
        j++;
    }
}

//初始化SSD1306
void OLED_Init(void) {
    GPIO_Init(OLED_SCLK_PORT, (GPIO_Pin_TypeDef)OLED_SCLK_PINS, GPIO_Mode_Out_PP_High_Fast);
    GPIO_Init(OLED_SDIN_PORT, (GPIO_Pin_TypeDef)OLED_SDIN_PINS, GPIO_Mode_Out_PP_High_Fast);
    // GPIO_Init(OLED_RST_PORT, (GPIO_Pin_TypeDef)OLED_RST_PINS, GPIO_Mode_Out_PP_High_Fast);

    // GPIO_Init(LED_PORT, (GPIO_Pin_TypeDef)LED_PINS, GPIO_Mode_Out_PP_High_Fast);


    OLED_WR_Byte(0x40, OLED_CMD);//---set low column address
    OLED_WR_Byte(0xB0, OLED_CMD);//---set high column address

    OLED_WR_Byte(0xC8, OLED_CMD);//-not offset

    OLED_WR_Byte(0x81, OLED_CMD);//设置对比度
    OLED_WR_Byte(0xff, OLED_CMD);

    OLED_WR_Byte(0xa1, OLED_CMD);//段重定向设置

    OLED_WR_Byte(0xa6, OLED_CMD);//

    OLED_WR_Byte(0xa8, OLED_CMD);//设置驱动路数
    OLED_WR_Byte(0x1f, OLED_CMD);

    OLED_WR_Byte(0xd3, OLED_CMD);
    OLED_WR_Byte(0x00, OLED_CMD);

    OLED_WR_Byte(0xd5, OLED_CMD);
    OLED_WR_Byte(0xf0, OLED_CMD);

    OLED_WR_Byte(0xd9, OLED_CMD);
    OLED_WR_Byte(0x22, OLED_CMD);

    OLED_WR_Byte(0xda, OLED_CMD);
    OLED_WR_Byte(0x02, OLED_CMD);

    OLED_WR_Byte(0xdb, OLED_CMD);
    OLED_WR_Byte(0x49, OLED_CMD);

    OLED_WR_Byte(0x8d, OLED_CMD);
    OLED_WR_Byte(0x14, OLED_CMD);

    OLED_WR_Byte(0xaf, OLED_CMD);
    OLED_Clear();
    OLED_Set_Pos(0, 0);
}



