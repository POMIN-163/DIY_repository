#include "global.h"

void delay_ms(unsigned int ms) {
    unsigned int a;
    while (ms) {
        a = 800;
        while (a--);
        ms--;
    }
    return;
}
