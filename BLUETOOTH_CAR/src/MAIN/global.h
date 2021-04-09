#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#define Run_Qianjin 0x6000
// 0  0      11
// 0000 0000 1100 0000
#define Run_Houtui 0x9000
// 1  1      00
// 1001 0000 0000 0000
#define Run_Zuo 0x2000
// 0  0      10
// 0000 0000 1000 0000
#define Run_You 0x4000
// 0  0      01
// 0000 0000 0100 0000

#define Run_STOP 0x0fff
// 0  0      00
// 0110 1111 0011 1111

#define uint unsigned int
#define uchar unsigned char
#define u8 uchar
#define u16 uint

extern unsigned char mode;
extern unsigned int Run_flag;
extern unsigned int TIM_Val;
extern unsigned char Choose_First;

#endif
