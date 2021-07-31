/**
 * @file main..c
 * @author Pomin (Q: 1637766030)
 * @brief badusb, 仅供学习交流使用, 请勿用于任何对他人造成伤害的用途, 否则后果自负
 * @date 2021-03-02
 *
 * @copyright Copyright (c) 2021
 *
**/
#include "./Public/CH554.H"
#include "./Public/DEBUG.H"
#include "key.h"
#include <stdio.h>

#define HID_KEY_CTRL HIDKey[0]   // Ctrl Shift Alt 三个键
#define HID_KEY_MAIN HIDKey[2]   // 主键盘
#define HID_KEY_SYS HIDKeyMUL[3] // 系统功能按键
#define KEY_Left_Control 0x01    // 多控制键可以用 | 连接比如 Ctrl+Alt 就是 KEY_Left_Control | KEY_Left_Alt
#define KEY_Left_Shift 0x02      // 左Shift, 下面就不解释了
#define KEY_Left_Alt 0x04
#define KEY_Left_GUI 0x08
#define KEY_Right_Control 0x10
#define KEY_Right_Shift 0x20
#define KEY_Right_Alt 0x40
#define KEY_Right_GUI 0x80

#define KEY_SYS_POWEROFF 0x81    // 关机
#define KEY_SYS_REST 0x82        // 休眠
#define KEY_SYS_WAKE 0x83        // 唤醒

#define _NULL_ ((void *)0)       // 空指针
#define T0RH (UINT8X)(64535 >> 8)// T0高8位重载值 (1ms)
#define T0RL (UINT8X) 64535      // T0低8位重载值

sbit led = P3 ^ 4;// 测试用LED

void Init(void);
void KeyScan(void);
void ConfigT0(void);
void KeyAction(unsigned char keyCode);
/**
 * @brief T0配置函数 (1ms)
 *
**/
void ConfigT0(void) {
    TMOD = ( TMOD & ~( bT0_GATE | bT0_CT | bT0_M1 ) ) | bT0_M0;//* 模式1，16 位定时/计数器
    TH0 = T0RH;
    TL0 = T0RL;
    TF0 = 0;
    ET0 = 1;
    TR0 = 1;
}
/**
 * @brief 按键动作函数
 *
 * @param mode
**/
void KeyAction(unsigned char mode)
{
    switch(mode) {
    case 0:
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 1:
        if(Ready) HIDValueHandle();
        else USBDeviceInit();
        break;
    default :
        break;
    }
    mDelaymS(20);
}

void Init(void) {
    CfgFsys();          // CH552时钟选择16M配置
    ConfigT0();         // 配置1ms T0中断
    USBDeviceInit();    // USB设备模式初始化
    EA = 1;             // 允许单片机中断
    UEP1_T_LEN = 0;     // 预使用发送长度一定要清空
    UEP2_T_LEN = 0;	    // 清空端点2发送长度
    FLAG = 0;           // 清空USB中断传输完成标志
    Ready = 0;
    LED_VALID = 1;      // 给一个默认值
    PCON = 1;
    led = 0;
}
// 大写锁定
void Capslk(void) {
    // 先关闭大写锁定
    HID_KEY_CTRL = KEY_Left_Shift;
    HID_KEY_MAIN = KEY_NULL;
    KeyAction(1);
    mDelaymS(500);
    HID_KEY_CTRL = KEY_Left_Shift;
    HID_KEY_MAIN = KEY_NULL;
    KeyAction(1);
    mDelaymS(500);
    // 打开大写锁定
    HID_KEY_CTRL = KEY_NULL;
    HID_KEY_MAIN = KEY_CAPS_LOCK;
    KeyAction(1);
    mDelaymS(500);
}
// 发送字符串到命令行
char command[100];
void str2cmd(const char *str) {
    while(*str != '\0') {
        // 1 - 9
        if((*str < ('9' + 1)) && (*str > ('0'))) {
            HID_KEY_CTRL = KEY_Left_Shift;
            HID_KEY_MAIN = *str - 19;      // 对应HID值 (数字)
            KeyAction(1);
        } else if(*str == '0') {
            HID_KEY_CTRL = KEY_Left_Shift;
            HID_KEY_MAIN = KEY_0;
            KeyAction(1);
        } else if((*str < ('Z' + 1)) && (*str > ('A' - 1))) {
            HID_KEY_CTRL = KEY_NULL;
            HID_KEY_MAIN = *str - 61;      // 对应HID值 (字母)
            KeyAction(1);
        } else if((*str < ('z' + 1)) && (*str > ('a' - 1))) {
            HID_KEY_CTRL = KEY_Left_Shift;
            HID_KEY_MAIN = *str - 61 - 32; // 对应HID值 (字母)
            KeyAction(1);
        } else if((*str == ' ')) {
            HID_KEY_CTRL = KEY_NULL;
            HID_KEY_MAIN = KEY_SPACE;      // 对应HID值 (空格)
            KeyAction(1);
        } else if((*str == '-')) {
            HID_KEY_CTRL = KEY_NULL;
            HID_KEY_MAIN = KEY_SUB;        // 对应HID值 (-)
            KeyAction(1);
        }
        str++;
    }
    HID_KEY_CTRL = KEY_NULL;
    HID_KEY_MAIN = KEY_ENTER;
    KeyAction(1);
}
#define sendCmd(str)       \
    sprintf(command, str); \
    str2cmd(command)

UINT8 Status = 0;
main(void) {
    UINT8X i;
    Init();
    // 等待连接
    while (1) {
        if (Ready) break;
        mDelaymS(500);
        led = ~led;
    }
    led = 0;
    // 连接完毕
    mDelaymS(500);
    // 打开大写锁定
    Capslk();
    mDelaymS(1000);
    // 打开运行
    HID_KEY_CTRL = KEY_Left_GUI;
    HID_KEY_MAIN = KEY_R;
    KeyAction(1);
    // 打开cmdC
    sendCmd("CMD");
    mDelaymS(500);
    // 发送命令
    sendCmd("echo HaHaHa");
    mDelaymS(200);
    sendCmd("echo HaHaHa");
    mDelaymS(200);
    sendCmd("echo HaHaHa");
    mDelaymS(200);
    sendCmd("echo HaHaHa");
    mDelaymS(200);
    sendCmd("echo HaHaHa");
    /*
    // 遍历关闭窗口
    i = 100;
    while(i--) {
        HID_KEY_CTRL = KEY_Left_Alt;
        HID_KEY_MAIN = KEY_TAB;
        KeyAction(1);
        mDelaymS(200);
        HID_KEY_CTRL = KEY_Left_Alt;
        HID_KEY_MAIN = KEY_F4;
        KeyAction(1);
        mDelaymS(200);
    }*/
    while(1);
}
/**T0中断函数 (提供心跳)**/
void InterruptTimer0() interrupt INT_NO_TMR0 using 1
{
    TH0 = T0RH;
    TL0 = T0RL;
}