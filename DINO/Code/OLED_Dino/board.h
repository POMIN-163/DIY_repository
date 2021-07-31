
#ifndef __BOARD_H__
#define __BOARD_H__
#include "CH552.H"

sbit LED = P3 ^ 0;
sbit BEEP = P3 ^ 1;

sbit SW1 = P3 ^ 2;
sbit SW2 = P1 ^ 4;
sbit SW3 = P1 ^ 5;
sbit SW4 = P1 ^ 6;
sbit SW5 = P1 ^ 7;
sbit SW6 = P1 ^ 1;
#define SW (SW1 && SW2 && SW3 && SW4 && SW5 && SW6)
unsigned char Ramod(unsigned char mod);

#endif
