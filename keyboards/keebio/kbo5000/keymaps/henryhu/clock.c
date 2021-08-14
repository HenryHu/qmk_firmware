#include QMK_KEYBOARD_H

#include "clock.h"

#ifdef ENABLE_CLOCK
uint32_t time_base = 0;

void get_time(char* buf) {
    const uint32_t now = (time_base + timer_read32() / 1000) % 86400;
    const uint32_t hour = now / 3600;
    const uint32_t min = now % 3600 / 60;
    buf[0] = 0;
    if (hour < 10) strcat(buf, " ");
    utoa(hour, buf + strlen(buf), 10);
    strcat(buf, ":");
    if (min < 10) strcat(buf, "0");
    utoa(min, buf + strlen(buf), 10);
}

void cmd_time(char* cmd, char* buf, int size) {
    int hour = atoi(cmd + 5);
    int min = atoi(cmd + 8);
    int now = hour * 60 + min;
    time_base = now * 60L - timer_read32() / 1000;
}
#endif
