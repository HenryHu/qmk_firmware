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

#ifdef ENABLE_OLED
char infoLine[22];

void setInfoLine(const char* buf) {
    strlcpy(infoLine, buf, sizeof(infoLine));
}

void get_infoline(void) {
#ifdef ENABLE_ALARM
    if (alarmArmed()) {
        strcpy(infoLine, "Alarm: ");
        utoa(alarmRemaining(), infoLine + 7, 10);
        strcat(infoLine, "s");
    }
#endif
    int len = strlen(infoLine);
    for (int j = len; j < sizeof(infoLine); ++j) infoLine[j] = ' ';
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

void oled_task_user(void) {
    uint32_t idle_time = idleTime();
#ifdef ENABLE_ALARM
    const uint32_t alarm_time = alarmTime();
    if (alarm_time < idle_time) idle_time = alarm_time;
#endif
    if (idle_time > 60000) {
        oled_off();
        return;
    }
    if (idle_time > 5000) infoLine[0] = 0;
    if (IS_LAYER_ON(3)) {
        oled_write_P(PSTR("XOP "), false);
    } else if (IS_LAYER_ON(2)) {
        oled_write_P(PSTR("SUP "), false);
    } else if (IS_LAYER_ON(1)) {
        oled_write_P(PSTR("HYP "), false);
    } else {
        oled_write_P(PSTR("    "), false);
    }
    const led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAP") : PSTR("   "),
            led_state.caps_lock);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR") : PSTR("   "),
            led_state.scroll_lock);
    oled_write_P(led_state.num_lock ? PSTR("NUM") : PSTR("   "),
            led_state.num_lock);
#ifdef ENABLE_CMDMODE
    oled_write_P(command_mode ? PSTR("CMD\n") : PSTR("   \n"), command_mode);
#else
    oled_write_P(PSTR("\n"), false);
#endif
    oled_write_P(get_mods() & MOD_MASK_SHIFT ? PSTR("SFT") : PSTR("   "), false);
    oled_write_P(get_mods() & MOD_MASK_CTRL ? PSTR(" CTL") : PSTR("    "), false);
    oled_write_P(get_mods() & MOD_MASK_ALT ? PSTR(" ALT") : PSTR("    "), false);

#ifdef ENABLE_CLOCK
    char buf[7];
    get_time(buf);
    oled_write(buf, false);
#endif
    oled_write_P(PSTR("\n"), false);

#ifdef ENABLE_NEKO
    const bool anime_pause = get_mods() & MOD_MASK_CTRL;

    // render_logo(17, 0);

    if (!anime_pause) {
        pick_frame(idle_time);
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
    oled_clear();
    oled_write_P(PSTR("RESET"), false);
    oled_render();
}
#else
void setInfoLine(const char* buf) {
}
#endif
