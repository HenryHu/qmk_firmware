#include "common.h"

const uint16_t ms_per_sec = 1000;
const uint8_t sec_per_min = 60;
const uint8_t min_per_hour = 60;
const uint8_t hour_per_day = 24;
const uint16_t sec_per_hour = 3600;
const uint16_t min_per_day = 1440;
const uint16_t ms_per_min = 60000;

void appendValue(char* buf, const uint16_t value) {
    utoa(value, buf + strlen(buf), 10);
}
