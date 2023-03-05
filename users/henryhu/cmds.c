#include QMK_KEYBOARD_H

#include <string.h>

#include "version.h"
#include "cmds.h"

#include "common.h"
#include "oled.h"
#include "alarm.h"
#include "status.h"
#include "cmdmode.h"
#include "clock.h"
#include "i2c.h"

#ifdef ENABLE_CMDS

void cmd_ver(char* cmd, char* buf, int size) {
    strcat_P(buf, PSTR(QMK_VERSION));
}

#ifdef ENABLE_UPTIME
void cmd_uptime(char* cmd, char* buf, int size) {
    appendValue(buf, timer_read32() / ms_per_min);
    strcat(buf, "m");
}
#endif

void append_attr_name(char* buf, PGM_P name) {
    strcat_P(buf, name);
    strcat_P(buf, PSTR(": "));
}

void append_attr_state(char* buf, PGM_P name, const bool value) {
    append_attr_name(buf, name);
    strcat_P(buf, value ? PSTR("YES") : PSTR("NO"));
}

void append_attr_value(char* buf, PGM_P name, const uint8_t value) {
    append_attr_name(buf, name);
    appendValue(buf, value);
}

#ifdef ENABLE_INFO
void cmd_info(char* cmd, char* buf, int size) {
    strcat_P(buf, PSTR(QMK_KEYBOARD));
}

void cmd_prod(char* cmd, char* buf, int size) {
    strcat_P(buf, PSTR(STR(MANUFACTURER) " " STR(PRODUCT)));
}
#endif

#ifdef ENABLE_RGBINFO
void cmd_rgb(char* cmd, char* buf, int size) {
    if (!rgblight_is_enabled()) {
        strcat_P(buf, PSTR("OFF"));
        return;
    }
    append_attr_value(buf, PSTR("M"), rgblight_get_mode());
    append_attr_value(buf, PSTR(" P"), rgblight_get_speed());
    append_attr_value(buf, PSTR("\nH"), rgblight_get_hue());
    append_attr_value(buf, PSTR(" S"), rgblight_get_sat());
    append_attr_value(buf, PSTR(" V"), rgblight_get_val());
}
#endif

#ifdef ENABLE_SPEED
void cmd_speed(char* cmd, char* buf, int size) {
    int arg = atoi(cmd + 6);
    rgblight_set_speed(arg);
    strcat_P(buf, PSTR("Speed:"));
    appendValue(buf, arg);
}
#endif

void cmd_unknown(char* cmd, char* buf, int size) {
    strcat_P(buf, PSTR("?: "));
    strcat(buf, cmd);
}

#ifdef ENABLE_STATUS
void cmd_status(char* cmd, char* buf, int size) {
    uint8_t i = 0, ptr = 7;
    for (i = 0; i < sizeof(statusLine) && cmd[ptr] != 0; ++i, ++ptr) {
        statusLine[i] = cmd[ptr];
    }
    for (; i < sizeof(statusLine); ++i) statusLine[i] = ' ';
}
#endif

typedef void(*handler_t)(char*, char*, int);

typedef struct {
    PGM_P name;
    handler_t handler;
} cmd_t;

void cmd_help(char* cmd, char* buf, int size);

#define DEFINE_COMMAND(name) \
    const char cmdname_##name[] PROGMEM = #name;
#include "cmds.inc"
#undef DEFINE_COMMAND
const cmd_t cmds[] PROGMEM = {
#define DEFINE_COMMAND(name) \
    {cmdname_##name, cmd_##name},
#include "cmds.inc"
#undef DEFINE_COMMAND
    {0, 0},
};

void cmd_help(char* cmd, char* buf, int size) {
    strcat_P(buf, PSTR("cmds:"));
    for (const cmd_t* cmd = &cmds[0]; ; ++cmd) {
        PGM_P name = (PGM_P)pgm_read_ptr(&cmd->name);
        if (name == 0) break;
        strcat_P(buf, PSPACE);
        strcat_P(buf, name);
    }
}

void handle_command(char* input, char* buf, int size) {
    for (const cmd_t* cmd = &cmds[0]; ; ++cmd) {
        PGM_P name = (PGM_P)pgm_read_ptr(&cmd->name);
        if (name == 0) break;
        if (memcmp_P(input, name, strlen_P(name)) == 0) {
            infoLine[0] = '!';
            strlcpy_P(infoLine + 1, name, sizeof(infoLine) - 1);
            handler_t handler = (handler_t)pgm_read_ptr(&cmd->handler);
            return handler(input, buf, size);
        }
    }
    cmd_unknown(input, buf, size);
}
#endif // ENABLE_CMDS
