/**************************************************
 * 原项目地址：https://www.oshwhub.com/TheLight/zi-ding-yi-jian-pan
 * CH552程序下载：http://www.wch.cn/download/WCHISPTool_Setup_exe.html
***************************************************/

/******************* 后期修改 **********************
 * @Date   2021-1-26
 * @QQ     1637766030
 * @Note   在原作者的基础上, 加入了 WS2812B 彩灯(24M的主频驱动真的费劲, 如果实测
 *         显示异常就去 WS2812.h 去加减 nop 的数量), 关于 USB 和 HID 有关的底层
 *         知识可以看 key.c 里面原作者的注解, 不再赘述, 主要是我看不懂 *__*, EC
 *         16 没用过, 就没写, 应该和 EC11 差不多.....吧。
 ***************************************************/
#include "./Public/CH552.H"
#include "./Public/DEBUG.H"
#include "WS2812B.h"
#include "keys.h"

#define HID_KEY_CTRL HIDKey[0]    // Ctrl Shift Alt 三个键
#define HID_KEY_MAIN HIDKey[2]    // 主键盘
#define HID_KEY_SYS HIDKeyMUL[3]  // 系统功能按键

#define KEY_Left_Control 0x01  // 多控制键可以用 | 连接比如 Ctrl+Alt 就是 KEY_Left_Control | KEY_Left_Alt
#define KEY_Left_Shift 0x02    // 左Shift, 下面就不解释了
#define KEY_Left_Alt 0x04
#define KEY_Left_GUI 0x08
#define KEY_Right_Control 0x10
#define KEY_Right_Shift 0x20
#define KEY_Right_Alt 0x40
#define KEY_Right_GUI 0x80

/* 自定义按键 ( 需和 key.c 内一致 ) */
// ------- HIDKeyMUL[0] -------- //
#define KEY_MUL_VOL_DOWN 0x01  // 音量减
#define KEY_MUL_VOL_UP   0x02  // 音量加
#define KEY_MUL_VOL_ZERO 0x04  // 静音
#define KEY_MUL_EMAIL    0x08  // 打开邮件
#define KEY_MUL_MEDIA    0x10  // 打开媒体播放器
#define KEY_MUL_BROWSER  0x20  // 打开浏览器
#define KEY_MUL_PAUSE    0x40  // 暂停/播放
#define KEY_MUL_STOP     0x80  // 停止
// ------- HIDKeyMUL[1] -------- //
#define KEY_MUL_PRE      0x01  /* 0x00 */ // 上一曲
#define KEY_MUL_NEXT     0x02  /* 0x00 */ // 下一曲
#define KEY_MUL_LGT_ADD  0x04  /* 0x00 */ // 亮度增加
#define KEY_MUL_LGH_SUB  0x08  /* 0x00 */ // 亮度减少

/* 非自定义按键 */
#define KEY_SYS_POWEROFF 0x81  // 关机
#define KEY_SYS_REST     0x82  // 休眠
#define KEY_SYS_WAKE     0x83  // 唤醒

#define _NULL_ ((void *)0)       // 空指针
#define T0RH (UINT8X)(45535 >> 8)// T0高8位重载值 (10ms)
#define T0RL (UINT8X) 45535      // T0低8位重载值

typedef struct {
    void(*task)(void);
    unsigned int cycle_length;
	unsigned int last_run;
} task_s;
#define task_num (4)
volatile unsigned char scheduler_run[4] = {1,0,0,1}; // 一些伪多线程参数

sbit rgb = P3 ^ 4;    // WS2812B 驱动引脚
sbit key1 = P3 ^ 2;   // 一堆按键(没有用复用, 如果想扩展键盘数量可以考虑用查理
sbit key2 = P1 ^ 4;   // 复用的办法, 7 个管脚可以整 42 个键, -.- 掉头发警告)
sbit key3 = P1 ^ 5;
sbit key4 = P1 ^ 6;
sbit key5 = P1 ^ 7;
sbit key6 = P1 ^ 1;
sbit key7 = P3 ^ 3;   // EC11_D
sbit EC11_A = P3 ^ 1;
sbit EC11_B = P3 ^ 0; // EC11的A、B脚
sbit ledTest = P3 ^ 3;// 测试用LED

UINT8X KeyState[8]  = { 1, 1, 1, 1, 1, 1, 1, 1}; // 按键状态
UINT8X BackState[8] = { 1, 1, 1, 1, 1, 1, 1, 1}; // 按键上一次的状态
UINT8X Key_msg = 0;                  // 按键消息
UINT8X rgbColor[3]  = { 0, 0, 200 }; // 三色值
UINT8X rgbStep[3]   = { 2, 2, 2 };   // 三色步进值
UINT8X rgbResult[3] = { 0, 0, 0 };   // 三色目标值
UINT8X rgbNum = 0;                   // 亮灯按键序号
UINT8X rgbCnt = 0;                   // 亮灯颜色选择

void Init(void);
void KeyScan(void);
void ConfigT0(void);
void KeyAction(unsigned char keyCode);

void rgbOff(void);        // 全灭
void rgbOne(UINT8X *RGB); // 亮一个
void rgbSet(UINT8X *RGB); // 亮指定的

/**T0配置函数**/
// void ConfigT0(void) {
//     TMOD = ( TMOD & ~( bT0_GATE | bT0_CT | bT0_M1 ) ) | bT0_M0;
//     // 模式1，16 位定时/计数器
//     TH0 = T0RH;
//     TL0 = T0RL;
//     TF0 = 0;
//     ET0 = 1;
//     TR0 = 1;
// }

/**按键扫描函数**/
void KeyScan(void) {
    // 0 - 5 是六个按键, 6是 EC11 按键(D脚), 7是 EC11_B 脚
    UINT8X i;
    BackState[7] = KeyState[7];
    KeyState[7] = EC11_B;
    // 按键放开响应
    if( !KeyState[7] && BackState[7]) {
        if(EC11_A) {
            Key_msg = 21; // 顺时针
        } else {
            Key_msg = 22; // 逆时针
        }
    }
    for(i = 0; i < 7; i++){
        BackState[i] = KeyState[i];
    }
    KeyState[0] = key1; KeyState[1] = key2; KeyState[2] = key3; KeyState[3] = key4;
    KeyState[4] = key5; KeyState[5] = key6; KeyState[6] = key7;

    for(i = 0; i < 7; i++) {
        if( KeyState[i] && !BackState[i]) {
            Key_msg = i+1;
        }
    }
    if( Key_msg > 0 && Key_msg < 7)
        rgbNum = Key_msg;  // 准备亮灯
}

/*
    普通按键
    例如ctrl + c :
    HIDKey[0] = 0x01;
    HIDKey[2] = 0x06;
    if(Ready) //枚举成功
    {
        HIDValueHandle1();
    }
*/

/* 按键动作函数 */
void KeyAction(unsigned char keyCode) {
    switch(Key_msg) {
    case 1:
        HIDKeyMUL[1] = KEY_MUL_PRE;       //上一曲
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 2:
        HIDKeyMUL[0] = KEY_MUL_PAUSE;     //播放/暂停
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 3:
        HIDKeyMUL[1] = KEY_MUL_NEXT;      //下一曲
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 4:
        HIDKeyMUL[0] = KEY_MUL_MEDIA;     // 打开媒体播放器
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 5:
        HIDKeyMUL[0] = KEY_MUL_BROWSER;   // 打开浏览器
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 6:
        HIDKeyMUL[3] = KEY_SYS_REST;      // 电脑休眠 (WIN10实测直接黑屏, 和 WIN + L的反应不一样, 别吓到了 *.*)
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 7:
        HIDKeyMUL[0] = KEY_MUL_VOL_ZERO;  // 静音(EC11按下)
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 21:
        HIDKeyMUL[0] = KEY_MUL_VOL_UP;    // 音量+
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    case 22:
        HIDKeyMUL[0] = KEY_MUL_VOL_DOWN;  // 音量-(EC11旋钮)
        if(Ready) HIDValueHandleMul();
        else USBDeviceInit();
        break;
    default :
        break;
    }
}

// 实际上 WS2812 是绿、红、蓝的次序也就是 GRB, 并非 RGB
void rgbOne(UINT8X *RGB) {
    UINT8X k,byte;
    if(RGB == _NULL_) {
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
        return;
    } // 空和非空代码一样, 写两遍为了不造成发送数据的时序问题
    else {
        byte = RGB[1];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
        byte = RGB[0];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
        byte = RGB[2];
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) { SIGNAL1 }
            else { SIGNAL0 }
            byte <<= 1;
        }
    }
}

void rgbSet(UINT8X *RGB) {
    UINT8X i;
    for ( i = 0; i < 6; i++) {
        if(rgbNum == 1 + i) {
            rgbOne(RGB); // 亮起按下的键的 RGB
        }
        else {
            rgbOne(_NULL_);
        }
    }
}
void rgbOff(void) {
    UINT8X i;
    for ( i = 0; i < 6; i++)
        rgbOne(_NULL_);
}
void Init(void)
{
    UINT8X i;
    CfgFsys();          // CH552时钟选择24M配置
    mDelaymS(100);      // 修改主频等待内部晶振稳定,必加
    // ConfigT0();         // 配置1ms T0中断
    USBDeviceInit();    // USB设备模式初始化
    EA = 1;             // 允许单片机中断
    UEP1_T_LEN = 0;     // 预使用发送长度一定要清空
    UEP2_T_LEN = 0;	    // 清空端点2发送长度
    FLAG = 0;           // 清空USB中断传输完成标志
    Ready = 0;
    LED_VALID = 1;      // 给一个默认值
    PCON = 1;
    key1 = 1;
    key2 = 1;
    key3 = 1;
    key4 = 1;
    key5 = 1;
    key6 = 1;
    key7 = 1;
    rgb = 0;

    EC11_A = 1;
    EC11_B = 1;

    for (i = 0; i < 6; i++) {
        rgbNum = i + 1;
        rgbSet(rgbColor);
        mDelaymS(80);
    }

    rgbColor[0] = rgbColor[1] = rgbColor[2] = 0;
    rgbOff();
    CH554WDTModeSelect(1);
}

/*********************************************** 使用伪线程 ************************************************/
volatile unsigned int data systime_ms;
unsigned char scheduler_index;
static void TASK_CYCLE_1(void) {
    if(Key_msg) {
        KeyAction(Key_msg);
        Key_msg = 0;
        switch (rgbCnt)
        {
        case 0:
            rgbResult[0] = 200; rgbResult[1] = 0; rgbResult[2] = 0; rgbCnt++; break;// 三色目标值
        case 1:
            rgbResult[0] = 0; rgbResult[1] = 200; rgbResult[2] = 200; rgbCnt++; break;
        case 2:
            rgbResult[0] = 0; rgbResult[1] = 200; rgbResult[2] = 0; rgbCnt++;  break;

        case 3:
            rgbResult[0] = 200; rgbResult[1] = 0; rgbResult[2] = 200; rgbCnt++; break;
        case 4:
            rgbResult[0] = 0; rgbResult[1] = 0; rgbResult[2] = 200; rgbCnt++;  break;
        default:// (5)
            rgbResult[0] = 200; rgbResult[1] = 200; rgbResult[2] = 0; rgbCnt = 0; break;
        }
    #define TIMES 5  // 步进次数 (和线程周期长度共同决定快慢程度)
        rgbStep[0] = rgbResult[0] /TIMES;
        rgbStep[1] = rgbResult[1] /TIMES;
        rgbStep[2] = rgbResult[2] /TIMES;// 三色步进值
        rgbColor[0] = rgbColor[1] = rgbColor[2] = 0;
        scheduler_run[1] = 1;            // 启动渐增
    }
}

static void TASK_CYCLE_2(void) {
    rgbColor[0] += rgbStep[0];
    rgbColor[1] += rgbStep[1];
    rgbColor[2] += rgbStep[2];
    rgbSet(rgbColor);
    if(rgbColor[0] >= rgbResult[0] && rgbColor[1] >= rgbResult[1] && rgbColor[2] >= rgbResult[2]) {
        scheduler_run[1] = 0;
        scheduler_run[2] = 1;       // 启动渐灭
    }
}

static void TASK_CYCLE_3(void) {
    if(rgbColor[0]) rgbColor[0] -= rgbStep[0];
    if(rgbColor[1]) rgbColor[1] -= rgbStep[1];
    if(rgbColor[2]) rgbColor[2] -= rgbStep[2];
    rgbSet(rgbColor);
    if(rgbColor[0] || rgbColor[1] || rgbColor[2]) {
    } else {
        scheduler_run[1] = 0;
        scheduler_run[2] = 0;       // 结束
        rgbNum = 0x00;
    }
}

static void TASK_CYCLE_5(void) {
//    static UINT16X timeout = 0;
//    timeout++;
      CH554WDTFeed(0);
//    if(timeout > 1000) {
//        SAFE_MOD = 0x55;
//        SAFE_MOD = 0xAA;
//        GLOBAL_CFG	|=bSW_RESET;
//    }
}

static task_s systemTasks[] = {
    {TASK_CYCLE_1 , 1,  0},
    {TASK_CYCLE_2 , 2,  0},
    {TASK_CYCLE_3 , 3,  0},
    {TASK_CYCLE_5 , 5,  0},
};

void main(void) {
    UINT8X i;
    Init();
    while (1) {
        mDelaymS(10);
        systime_ms++;
        if(systime_ms > 4000) {
            systime_ms = 0;
            if(!Ready) USBDeviceInit();
            for(scheduler_index = 0; scheduler_index < task_num; scheduler_index++) {
                systemTasks[scheduler_index].last_run = 0;
            }
            scheduler_run[0] = 1;
            scheduler_run[1] = 0;
            scheduler_run[2] = 0;
            scheduler_run[3] = 1;
        }
        KeyScan(); //按键扫描
        for(scheduler_index = 0; scheduler_index < task_num; scheduler_index++) {
            if(scheduler_run[scheduler_index]) {
                if(systime_ms - systemTasks[scheduler_index].last_run > systemTasks[scheduler_index].cycle_length) {
                    systemTasks[scheduler_index].last_run = systime_ms;
                    systemTasks[scheduler_index].task();
                }
            }
            else {
                systemTasks[scheduler_index].last_run = systime_ms;// 避免立即执行任务
            }
        }
    }
}
// /**T0中断函数 (提供心跳)**/
// void InterruptTimer0() interrupt INT_NO_TMR0 using 1
// {
//     TH0 = T0RH;
//     TL0 = T0RL;
// }
