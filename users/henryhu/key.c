#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "common.h"
#include "macro_strings.h"
#include "cmdmode.h"
#include "alttab.h"
#include "oled.h"
#include "alarm.h"

uint32_t last_key_down_time = 0;

#ifdef ENABLE_CHARCOUNT
uint16_t key_count = 0;

uint16_t getKeyCount(void) {
    return key_count;
}

#else

uint16_t getKeyCount(void) { return 0; }

#endif

uint32_t idleTime(void) {
    return timer_elapsed32(last_key_down_time);
}

void updateKeyDownTime(void) {
    last_key_down_time = timer_read32();
}

bool process_key_down(uint16_t keycode, keyrecord_t *record) {
    updateKeyDownTime();
#ifdef ENABLE_CHARCOUNT
    key_count++;
#endif
    switch (keycode) {
#ifdef ENABLE_ALTTAB
        case ALT_TAB:
            alttab_down();
            break;
#endif
        case RGB_TOG:
            rgblight_toggle_noeeprom();
            break;
#ifdef ENABLE_CMDMODE
        default:
            if (command_mode) {
                return false;
            }
#endif
    }
    return true;
}

#ifdef USE_SEND_STRING_LITE
// Lite version is only needed if we want to avoid send_string().
// VIA uses that anyway.
void send_string_lite(PGM_P buf) {
    bool shift = false;
    for (char ch = pgm_read_byte(buf); ch; ch = pgm_read_byte(++buf)) {
        if (ch == KC_LSFT) {
            if (shift) unregister_code(ch); else register_code(ch);
            shift = !shift;
        } else {
            tap_code(ch);
        }
    }
}

#define send_string send_string_lite
#endif

bool process_key_up(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef ENABLE_MACRO
        case MACRO1:
            send_string(MACRO1_STRING);
            break;
        case MACRO2:
            send_string(MACRO2_STRING);
            break;
#endif
#ifdef ENABLE_ALARM
        case SET_ALARM:
            alarmKey();
            break;
#endif
#ifdef ENABLE_ALTTAB
        case ALT_TAB:
            alttab_up();
            break;
#endif
#ifdef ENABLE_CMDMODE
        case CMDMODE:
            command_mode = !command_mode;
            break;
        default:
            if (command_mode) {
                return command_mode_key(keycode, record);
            }
#endif
    }
    return true;
}
