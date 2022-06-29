#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "common.h"
#include "macro_strings.h"
#include "cmdmode.h"
#include "alttab.h"
#include "oled.h"
#include "alarm.h"

uint32_t last_key_down_time = 0;

uint32_t idleTime(void) {
    return timer_elapsed32(last_key_down_time);
}

void updateKeyDownTime(void) {
    last_key_down_time = timer_read32();
}

bool process_key_down(uint16_t keycode, keyrecord_t *record) {
    updateKeyDownTime();
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

void send_string_lite(const char* buf) {
    bool shift = false;
    for (; *buf; ++buf) {
        if (*buf == KC_LSFT) {
            if (shift) unregister_code(*buf); else register_code(*buf);
            shift = !shift;
        } else {
            tap_code(*buf);
        }
    }
}

bool process_key_up(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef ENABLE_MACRO
        case MACRO1:
            send_string_lite(MACRO1_STRING);
            break;
        case MACRO2:
            send_string_lite(MACRO2_STRING);
            break;
        case MACRO3:
            send_string_lite(MACRO3_STRING);
            break;
        case MACRO4:
            send_string_lite(MACRO4_STRING);
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
        case CMD_MODE:
            command_mode = !command_mode;
            setInfoLine("?_");
            break;
        default:
            if (command_mode) {
                return command_mode_key(keycode, record);
            }
#endif
    }
    return true;
}
