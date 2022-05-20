#include "hsv2rgb.h"

#define NUM_LEDS 18
#define _NULL_ ((void *)0)         // 空指针
#define T0RH (UINT8X)(45535 >> 8)  // T0高8位重载值 (10ms)
#define T0RL (UINT8X)45535         // T0低8位重载值

typedef struct {
    void (*task)(void);
    unsigned int cycle_length;
    unsigned int last_run;
} task_s;

#define task_num (2)
volatile unsigned char scheduler_run[task_num] = {
    1,
    1,
};  // 一些伪多线程参数
volatile unsigned int data systime_ms;
unsigned char scheduler_index;

sbit rgb_led = P3 ^ 3;  // WS2812B 驱动引脚
sbit key = P1 ^ 1;
sbit led = P3 ^ 2;  // 测试用LED

UINT8X key_state = 1;   // 按键状态
UINT8X back_state = 1;  // 按键上一次的状态
UINT8X rgb_mode = 0;    // 模式选择
UINT8 buff[18 * 3];     // 显存
UINT8 is_choose = 1;    // 切换了一下模式

#define rgb_buff ((crgb *)buff)
#define LIGHT 10
#define MODE_NUM 9

void main_init(void);
void key_scan(void);
void ConfigT0(void);

void rgb_off(void);       // 全灭
void rgb_one(crgb *rgb);  // 亮一个
void rgb_set(crgb *rgb);  // 亮指定的

/**T0配置函数**/
void ConfigT0(void) {
    TMOD = (TMOD & ~(bT0_GATE | bT0_CT | bT0_M1)) |
           bT0_M0;  //* 模式1，16 位定时/计数器
    TH0 = T0RH;
    TL0 = T0RL;
    TF0 = 0;
    ET0 = 1;
    TR0 = 1;
}

/**按键扫描函数**/
void key_scan(void) {
    back_state = key_state;
    key_state = key;
    if (key_state && !back_state) {
        rgb_mode += 1;
        rgb_mode %= MODE_NUM;
        is_choose = 1;
    }
}

// 实际上 WS2812 是绿、红、蓝的次序也就是 GRB, 并非 rgb
void rgb_one(crgb *rgb) {
    UINT8X k, byte;
    if (rgb == _NULL_) {
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = 0;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        return;
    }  // 空和非空代码一样, 写两遍为了不造成发送数据的时序问题
    else {
        byte = rgb->g;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = rgb->r;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
        byte = rgb->b;
        for (k = 0; k < 8; k++) {
            if (byte & 0x80) {
                SIGNAL1
            } else {
                SIGNAL0
            }
            byte <<= 1;
        }
    }
}

void rgb_set_buff(crgb *rgb) {
    UINT8X i;
    for (i = 0; i < NUM_LEDS; i++) {
        rgb_one(&rgb[i]);
    }
}

void rgb_set_all(crgb *rgb) {
    UINT8X i;
    for (i = 0; i < NUM_LEDS; i++) {
        rgb_one(rgb);
    }
}

void rgb_off(void) {
    UINT8X i;
    for (i = 0; i < NUM_LEDS; i++) {
        rgb_one(_NULL_);
    }
}

void main_init(void) {
    CfgFsys();       // CH552时钟选择24M配置
    // mDelaymS(100);   // 修改主频等待内部晶振稳定,必加
    ConfigT0();      // 定时器 10ms 时钟
    EA = 1;          // 允许单片机中断
    PCON = 1;
    key = 1;
    led = 0;
}

static void TASK_1(void) {
    static int h = 0;
    int i;
    chsv hsv;
    crgb rgb = { 0 };
    /* 模式选择 */
    hsv.s = 80;
    hsv.v = 5;
    switch (rgb_mode) {
        /* 红 */
        case 0:
            if (is_choose) {
                rgb.r = LIGHT;
                rgb.g = rgb.b = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 绿 不好看，，，删了！！！ */
        case 1:
            if (is_choose) {
                rgb.g = 0;
                rgb.r = rgb.b = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 蓝 */
        case 2:
            if (is_choose) {
                rgb.b = LIGHT;
                rgb.g = rgb.r = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 黄 */
        case 3:
            if (is_choose) {
                rgb.g = rgb.r = LIGHT;
                rgb.b = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 紫 */
        case 4:
            if (is_choose) {
                rgb.b = rgb.r = LIGHT;
                rgb.g = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 青 */
        case 5:
            if (is_choose) {
                rgb.b = rgb.g = LIGHT;
                rgb.r = 0;
                rgb_set_all(&rgb);
                is_choose = 0;
            }
            break;
        /* 五彩斑斓 */
        case 6:
            hsv.h = (h + (255 / NUM_LEDS) * i);
            for (i = 0; i < NUM_LEDS; i++) {
                HSVtoRGB(&rgb_buff[i], &hsv);
            }
            rgb_set_buff(rgb_buff);
            /* h 通道循环 */
            h = (h + 3) % 255;
            break;
        /* 流光溢彩 */
        case 7:
            for (i = 0; i < NUM_LEDS; i++) {
                hsv.h = (h + (255 / NUM_LEDS) * i);
                HSVtoRGB(&rgb_buff[i], &hsv);
            }
            rgb_set_buff(rgb_buff);
            /* h 通道循环 */
            h = (h + 5) % 255;
            break;
        case 8:
            rgb_off();
            break;
    }
}

static void TASK_2(void) {}
/* 任务表 */
static task_s systemTasks[] = {
    {TASK_1, 1, 0},
    {TASK_2, 20, 0},
};

void main(void) {
    main_init();
    while (1) {
        for (scheduler_index = 0; scheduler_index < task_num;
             scheduler_index++) {
            if (scheduler_run[scheduler_index]) {
                if (systime_ms - systemTasks[scheduler_index].last_run >
                    systemTasks[scheduler_index].cycle_length) {
                    systemTasks[scheduler_index].last_run = systime_ms;
                    systemTasks[scheduler_index].task();
                }
            } else {
                systemTasks[scheduler_index].last_run =
                    systime_ms;  // 避免立即执行任务
            }
        }
    }
}

/**T0中断函数 (提供心跳)**/
void InterruptTimer0() interrupt INT_NO_TMR0 using 1 {
    systime_ms++;
    if (systime_ms > 4000) {
        systime_ms = 0;

        for (scheduler_index = 0; scheduler_index < task_num;
             scheduler_index++) {
            systemTasks[scheduler_index].last_run = 0;
        }
        scheduler_run[0] = 1;
        scheduler_run[1] = 0;
    }
    key_scan();  // 按键扫描
    TH0 = T0RH;
    TL0 = T0RL;
}
