#include QMK_KEYBOARD_H

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "options.h"
#include "alarm.h"
#include "key.h"
#include "status.h"
#include "cmdmode.h"
#include "clock.h"
#include "anim.h"
#include "oled.h"
#include "common.h"

#ifdef ENABLE_OLED
char infoLine[22];
const uint8_t infoLineLen = 22;

const uint16_t idle_timeout = 60000;
const uint16_t info_timeout = 5000;
bool light_up = true;

void setInfoLine(const char* buf) {
    strlcpy(infoLine, buf, sizeof(infoLine));
}

void setInfoLine_P(PGM_P buf) {
    strlcpy_P(infoLine, buf, sizeof(infoLine));
}

void appendInfoLine(const uint16_t value) {
    appendValue(infoLine, value);
}

void get_infoline(void) {
#ifdef ENABLE_ALARM
    if (alarmArmed()) {
        setInfoLine_P(PSTR("Alarm: "));
        appendInfoLine(alarmRemaining());
        strcat_P(infoLine, PSTR("s"));
    }
#endif
    uint8_t len = strlen(infoLine);
    for (uint8_t j = len; j < sizeof(infoLine); ++j) infoLine[j] = ' ';
    infoLine[sizeof(infoLine) - 1] = 0;
}

void render_logo(uint8_t col, uint8_t row) {
    static const char PROGMEM qmk_logo[][5] = {
        { 0x80, 0x81, 0x82, 0x83, 0x00},
        { 0xA0, 0xA1, 0xA2, 0xA3, 0x00},
        { 0xC0, 0xC1, 0xC2, 0xC3, 0x00},
    };
    oled_set_cursor(col, row);
    oled_write_P(qmk_logo[0], false);
    oled_set_cursor(col, row + 1);
    oled_write_P(qmk_logo[1], false);
    oled_set_cursor(col, row + 2);
    oled_write_P(qmk_logo[2], false);
}

void oled_write_led_state(bool led_state, PGM_P name) {
    oled_write_P(led_state ? name : PSPACE_3, led_state);
}

bool oled_task_user(void) {
    uint32_t idle_time = idleTime();
#ifdef ENABLE_ALARM
    const uint32_t alarm_time = alarmTime();
    if (alarm_time < idle_time) idle_time = alarm_time;
#endif
    if (idle_time > idle_timeout) {
        if (light_up) {
            light_up = false;
            oled_off();
            rgblight_disable();
        }
        return false;
    } else if (!light_up) {
        rgblight_enable();
        light_up = true;
    }
    const uint16_t idle_time_16 = idle_time;
    if (idle_time_16 > info_timeout) infoLine[0] = 0;

    // 3 chars
    if (IS_LAYER_ON(3)) {
        oled_write_P(PSTR("XOP"), false);
    } else if (IS_LAYER_ON(2)) {
        oled_write_P(PSTR("SUP"), false);
    } else if (IS_LAYER_ON(1)) {
        oled_write_P(PSTR("HYP"), false);
    } else {
        oled_write_P(PSPACE_3, false);
    }

    // 9/12 chars
    const led_t led_state = host_keyboard_led_state();
    oled_write_led_state(led_state.caps_lock, PSTR("CAP"));
    oled_write_led_state(led_state.scroll_lock, PSTR("SCR"));
    oled_write_led_state(led_state.num_lock, PSTR("NUM"));
#ifdef ENABLE_CMDMODE
    oled_write_led_state(command_mode, PSTR("CMD"));
#endif
#ifdef NKRO_ENABLE
    oled_write_P(keymap_config.nkro ? PSTR("N") : PSPACE, false);
#endif

    // 11 chars
    oled_set_cursor(0, 1);
    const uint8_t mods = get_mods();
    oled_write_P(mods & MOD_MASK_SHIFT ? PSTR("SFT") : PSPACE_3, false);
    oled_write_P(mods & MOD_MASK_CTRL ? PSTR(" CTL") : PSPACE_4, false);
    oled_write_P(mods & MOD_MASK_GUI ? PSTR(" WIN") : PSPACE_4, false);
    oled_write_P(mods & MOD_MASK_ALT ? PSTR(" ALT") : PSPACE_4, false);

#ifdef ENABLE_CLOCK
    // 5 chars
    char buf[7];
    get_time(buf);
    oled_set_cursor(16, 4);
    oled_write(buf, false);
#endif

#ifdef ENABLE_NEKO
    const bool anime_pause = mods & MOD_MASK_CTRL;

    // render_logo(17, 0);

    if (!anime_pause) {
        pick_frame(idle_time_16);
    }
    render_animation(17, 0);
#endif

#ifdef ENABLE_STATUS
    oled_set_cursor(0, 5);
    statusLine[sizeof(statusLine) - 1] = 0;
    oled_write(statusLine, false);
#endif

#ifdef ENABLE_CMDMODE
    oled_set_cursor(0, 6);
    if (command_mode) {
        oled_write_P(PSTR("> "), false);
        oled_write(cmdBuf, false);
        oled_write_ln_P(PSTR("_"), false);
    } else {
        oled_write_ln_P(PSTR(""), false);
    }
#endif

    get_infoline();
    oled_set_cursor(0, 7);
    oled_write(infoLine, false);
    return false;
}

void shutdown_oled(void) {
    oled_set_cursor(0, 0);
    oled_write_P(PSTR("RESET"), false);
    oled_render();
}
#else
void setInfoLine(const char* buf) {}
void appendInfoLine(const uint16_t value) {}
#endif
