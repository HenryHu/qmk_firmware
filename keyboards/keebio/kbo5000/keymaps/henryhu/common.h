#pragma once

#include QMK_KEYBOARD_H

#include <stdint.h>

enum custom_keycodes {
    MACRO1 = SAFE_RANGE,
    MACRO2,
    MACRO3,
    MACRO4,
    SET_ALARM,
    ALT_TAB,
    CMD_MODE,
};

extern const uint16_t ms_per_sec;
extern const uint8_t sec_per_min;
extern const uint8_t min_per_hour;
extern const uint8_t hour_per_day;
extern const uint16_t sec_per_hour;
extern const uint16_t min_per_day;
extern const uint16_t ms_per_min;
