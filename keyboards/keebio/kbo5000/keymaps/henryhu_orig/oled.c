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

void setInfoLine(const char* buf) {
    strlcpy(infoLine, buf, sizeof(infoLine));
}

void appendInfoLine(const uint16_t value) {
    appendValue(infoLine, value);
}

void get_infoline(void) {
#ifdef ENABLE_ALARM
    if (alarmArmed()) {
        setInfoLine("Alarm: ");
        appendInfoLine(alarmRemaining());
        strcat(infoLine, "s");
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

void oled_write_led_state(bool led_state, const char* name) {
    oled_write(led_state ? name : "   ", led_state);
}

void oled_task_user(void) {
    uint32_t idle_time = idleTime();
#ifdef ENABLE_ALARM
    const uint32_t alarm_time = alarmTime();
    if (alarm_time < idle_time) idle_time = alarm_time;
#endif
    if (idle_time > idle_timeout) {
        oled_off();
        return;
    }
    const uint16_t idle_time_16 = idle_time;
    if (idle_time_16 > info_timeout) infoLine[0] = 0;

    // 3 chars
    if (IS_LAYER_ON(3)) {
        oled_write("XOP", false);
    } else if (IS_LAYER_ON(2)) {
        oled_write("SUP", false);
    } else if (IS_LAYER_ON(1)) {
        oled_write("HYP", false);
    } else {
        oled_write("   ", false);
    }

    // 9/12 chars
    const led_t led_state = host_keyboard_led_state();
    oled_write_led_state(led_state.caps_lock, "CAP");
    oled_write_led_state(led_state.scroll_lock, "SCR");
    oled_write_led_state(led_state.num_lock, "NUM");
#ifdef ENABLE_CMDMODE
    oled_write_led_state(command_mode, "CMD");
#endif
#ifdef NKRO_ENABLE
    oled_write(keymap_config.nkro ? "N" : " ", false);
#endif

    // 11 chars
    oled_set_cursor(0, 1);
    const uint8_t mods = get_mods();
    oled_write(mods & MOD_MASK_SHIFT ? "SFT" : "   ", false);
    oled_write(mods & MOD_MASK_CTRL ? " CTL" : "    ", false);
    oled_write(mods & MOD_MASK_GUI ? "W" : " ", false);
    oled_write(mods & MOD_MASK_ALT ? "ALT" : "   ", false);

#ifdef ENABLE_CLOCK
    // 5 chars
    char buf[7];
    get_time(buf);
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
    oled_set_cursor(0, 2);
    statusLine[sizeof(statusLine) - 1] = 0;
    oled_write(statusLine, false);
#endif

    get_infoline();
    oled_set_cursor(0, 3);
    oled_write(infoLine, false);
}

void shutdown_oled(void) {
    oled_write("RESET", false);
    oled_render();
}
#else
void setInfoLine(const char* buf) {}
void appendInfoLine(const uint16_t value) {}
#endif
