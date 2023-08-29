#include "common.h"

UINT8X key_state[3 + 1] = {
    1,
    1,
    1,
    1,
};  // 按键状态 (三按键 + EC11 的一侧)
UINT8X back_state[3 + 1] = {
    1,
    1,
    1,
    1,
};                   // 按键上一次的状态
UINT8X key_msg = 0;  // 按键消息

/* hsv 颜色值 "随机列表" */
UINT16X code clr_list[] = {
    0,    // R     红
    60,   // R + G 黄
    120,  // G     绿
    180,  // G + B 青蓝
    240,  // B     蓝
    300,  // R + B 紫色
};

chsv hsv_clr[3] = {0, 0, 0};    // hsv 值
UINT8X rgb_clr[9] = {0, 0, 0};  // 三色值
UINT8 HIDKey[8] = {0};          // 普通按键键值字节流
UINT8 HIDKeyMUL[1] = {0};       // 多媒体按键字节流

#define HID_KEY_CTRL HIDKey[0]  // Ctrl Shift Alt 三个键
#define HID_KEY_MAIN HIDKey[2]  // 主键盘

void init(void);
void key_act(void);
void key_scan(void);
void rand_clr(UINT8X index);

/**
 * @brief "随机颜色"
 *
 * @param index
 **/
void rand_clr(UINT8X index) {
    static UINT8X rand = 0;
    if (rand >= sizeof(clr_list) / sizeof(UINT16X)) {
        rand = 0;
    }
    hsv_clr[index].h = clr_list[rand++];
    hsv_clr[index].v = 100;
}

/* 按键扫描 */
void key_scan(void) {
    // 0 - 2 是 3 个按键, 3 是 EC11 B 脚
    UINT8X i;
#define EC11_B_BACK_STA back_state[3]
#define EC11_B_NOW_STA  key_state[3]
    EC11_B_BACK_STA = EC11_B_NOW_STA;
    EC11_B_NOW_STA = IO_EC11_B;

    /* EC11 响应 上升沿/下降沿全触发 抵消不灵敏的感觉 */
    if (!EC11_B_NOW_STA && EC11_B_BACK_STA) {
        if (IO_EC11_A) {
            key_msg = 22;  // 顺时针
        } else {
            key_msg = 21;  // 逆时针
        }
    }
    /* 普通按键扫描 */
    for (i = 0; i < 3; i++) {
        back_state[i] = key_state[i];
    }
    key_state[0] = IO_KEY0;
    key_state[1] = IO_KEY1;
    key_state[2] = IO_KEY2;

    for (i = 0; i < 3; i++) {
        if (key_state[i] && !back_state[i]) {
            key_msg = i + 1;
        }
    }
    /* 渐灭灯 */
    if (key_msg > 0 && key_msg < 4) rand_clr(key_msg - 1);
}

/* 按键动作函数 */
void key_act(void) {
    switch (key_msg) {
        case 1:
            drv_usb_mul_click(HID_CONSUMER_PLAY_PAUSE);
            break;
        case 2:
            drv_usb_mul_click(HID_CONSUMER_SCAN_NEXT_TRK);
            break;
        case 3:
            drv_usb_mul_click(KEY_SYS_REST);
            break;
        case 4:
            break;
        case 21:
            drv_usb_dial(200);
            break;
        case 22:
            drv_usb_dial(-200);
            break;
        default:
            break;
    }
}

void init(void) {
    UINT8X i;
    CfgFsys();       // CH552时钟选择24M配置
    mDelaymS(100);   // 修改主频等待内部晶振稳定,必加
    drv_usb_init();  // USB设备模式初始化
    EA = 1;          // 允许单片机中断
    UEP2_T_LEN = 0;  // 清空端点2发送长度
    FLAG = 0;        // 清空USB中断传输完成标志
    IO_KEY0 = 1;     // 硬件已经上/下拉, 此处初始置1/0即可
    IO_KEY1 = 1;
    IO_KEY2 = 1;
    IO_RGB = 0;

    IO_EC11_A = 1;
    IO_EC11_B = 1;
    IO_EC11_D = 1;

    /* 提示灯 */
    for (i = 0; i < 3; i++) {
        rand_clr(i);
    }

    rgb_off();

    // 无用代码 消除 L16 warning
    get_numlock();
    get_capslock();
    get_fn_lock();
}

static void TASK_CYCLE_1(void) {
    static uint idata sw_num = 0;
    if (IO_EC11_D == 0) {
        sw_num++;
        if (sw_num == 5) {
            drv_usb_dial(1);
        }
    } else {
        if (sw_num >= 5) drv_usb_dial(0);
        sw_num = 0;
    }
}

static void TASK_CYCLE_2(void) {
    key_scan();
    if (key_msg) {
        key_act();
        key_msg = 0;
    }
}

static void TASK_CYCLE_5(void) {
    UINT8X i;
    static UINT16X h = 0;
    static char step[3] = {
        1,
        1,
        1,
    };
    /* h 通道循环 */
    for (i = 0; i < 3; i++) {
        /* 按下亮起 */
        if (hsv_clr[i].v != 0) {
            hsv_clr[i].v -= 1;
            hsv_clr[i].s = 100;
        } else {
            hsv_clr[i].s = 0;
            hsv_clr[i].v = 0;
            hsv_clr[i].h = 0;
        }

        /* 彩色呼吸 */
        // hsv_clr[i].s = 100;
        // hsv_clr[i].v = 100;

        // hsv_clr[i].h += step[i];
        // if (hsv_clr[i].h == 358) {
        //     step[i] = -1;
        // } else if (hsv_clr[i].h == 0) {
        //     step[i] = 1;
        // }

        HSVtoRGB((crgb *)(&rgb_clr[3 * i]), &hsv_clr[i]);
    }

    rgb_all(rgb_clr);
}

/* 一些伪多线程参数 (本质为软件定时器) */
typedef struct {
    void (*task)(void);
    unsigned int cycle_length;
    unsigned int last_run;
} task_s;

static task_s systemTasks[] = {
    {TASK_CYCLE_1, 1, 0},
    {TASK_CYCLE_2, 2, 0},
    {TASK_CYCLE_5, 5, 0},
};
#define task_num sizeof(systemTasks) / sizeof(task_s)
volatile unsigned int data systime_ms;
UINT8X scheduler_index;

void main(void) {
    init();

    while (1) {
        mDelaymS(1);
        systime_ms++;
        for (scheduler_index = 0; scheduler_index < task_num;
             scheduler_index++) {
            if (systime_ms - systemTasks[scheduler_index].last_run >
                systemTasks[scheduler_index].cycle_length) {
                systemTasks[scheduler_index].last_run = systime_ms;
                systemTasks[scheduler_index].task();
            }
        }
    }
}
