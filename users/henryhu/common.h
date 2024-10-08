#pragma once

#include QMK_KEYBOARD_H

#include <stdint.h>
#include <platforms/progmem.h>

enum custom_keycodes {
    MACRO1 = SAFE_RANGE,
    MACRO2,
    MACRO3,
    MACRO4,
    SET_ALARM,
    ALT_TAB,
    CMDMODE,
};

extern const uint16_t ms_per_sec;
extern const uint8_t sec_per_min;
extern const uint8_t min_per_hour;
extern const uint8_t hour_per_day;
extern const uint16_t sec_per_hour;
extern const uint16_t min_per_day;
extern const uint16_t ms_per_min;

extern const char PSPACE_4[];
extern const char PSPACE_3[];
extern const char PSPACE_2[];
extern const char PSPACE[];

void appendValue(char* buf, const uint16_t value);

#ifndef __AVR__
#define strcat_P strcat
#define memcmp_P memcmp
#define strlcpy_P strlcpy
#endif
