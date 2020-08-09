#include <device.h>

extern uint8 buf[96][128];
extern void update();

void test() {
    buf[0][0] ^= 63;
    update();
}