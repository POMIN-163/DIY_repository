#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 

extern u8 led1 = 0;
extern u8 led2 = 0;
extern u8 G_status = 0;
// 1: º∆ ˝, 2: ‘›Õ£, 3: «Â¡„
extern u8 G_time[2] = {0, 0,};
// √Î°¢10ms

extern unsigned const char LED_0_9[10]={
    // 0: 1    2:    3	  4:	5   6:	 7   8:	  9
    0xbf,0x06,0xdb,0x4f,0xe6,0x6d,0xfd,0x07,0xff,0x6f};

#endif
