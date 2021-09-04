#include QMK_KEYBOARD_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "clock.h"
#include "common.h"

#ifdef ENABLE_CLOCK
uint16_t time_base_min = 0;

void get_time(char* buf) {
    const uint16_t now_min = (time_base_min + timer_read32() / ms_per_min) % min_per_day;
    const uint8_t hour = now_min / min_per_hour;
    const uint8_t min = now_min - hour * min_per_hour;
    buf[0] = 0;
    if (hour < 10) strcat(buf, " ");
    utoa(hour, buf + strlen(buf), 10);
    strcat(buf, ":");
    if (min < 10) strcat(buf, "0");
    utoa(min, buf + strlen(buf), 10);
}

void cmd_time(char* cmd, char* buf, int size) {
    const uint16_t hour = atoi(cmd + 5);
    const uint16_t min = atoi(cmd + 8);
    const uint16_t now_min = hour * min_per_hour + min;
    time_base_min = now_min - timer_read32() / ms_per_min;
}
#endif
