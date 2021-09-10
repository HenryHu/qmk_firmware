#include QMK_KEYBOARD_H

#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "clock.h"
#include "common.h"

#ifdef ENABLE_CLOCK
uint16_t time_base_min = 0;

uint16_t timer_min(void) {
    return timer_read32() / ms_per_min;
}

void get_time(char* buf) {
    const uint16_t now_min = (time_base_min + timer_min()) % min_per_day;
    div_t hour_min = div(now_min, min_per_hour);
    const uint8_t hour = hour_min.quot;
    const uint8_t min = hour_min.rem;
    buf[0] = 0;
    if (hour < 10) strcat(buf, " ");
    appendValue(buf, hour_min.quot);
    strcat(buf, ":");
    if (min < 10) strcat(buf, "0");
    appendValue(buf, min);
}

void cmd_time(char* cmd, char* buf, int size) {
    const uint16_t hour = atoi(cmd + 5);
    const uint16_t min = atoi(cmd + 8);
    const uint16_t now_min = hour * min_per_hour + min;
    time_base_min = now_min - timer_min();
}
#endif
