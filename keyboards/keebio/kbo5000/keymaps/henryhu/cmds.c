#include QMK_KEYBOARD_H

#include "version.h"
#include "cmds.h"

#include "oled.h"
#include "timer.h"
#include "status.h"
#include "cmdmode.h"
#include "clock.h"

#ifdef ENABLE_CMDS
void cmd_ver(char* cmd, char* buf, int size) {
    strcat(buf, QMK_VERSION "\n" QMK_BUILDDATE);
}

#ifdef ENABLE_UPTIME
void cmd_uptime(char* cmd, char* buf, int size) {
    ultoa(timer_read32() / 1000, buf, 10);
    strcat(buf, "s");
}
#endif

#ifdef ENABLE_INFO
void append_attr_name(char* buf, const char* name) {
    strcat(buf, name);
    strcat(buf, ": ");
}

void append_attr_state(char* buf, const char* name, const bool value) {
    append_attr_name(buf, name);
    strcat(buf, value ? "YES" : "NO");
}

void append_attr_value(char* buf, const char* name, const uint8_t value) {
    append_attr_name(buf, name);
    utoa(value, buf + strlen(buf), 10);
}

void cmd_info(char* cmd, char* buf, int size) {
    strcat(buf, QMK_KEYBOARD "\n" STR(MANUFACTURER) " " STR(PRODUCT) "\n");
#ifdef NKRO_ENABLE
    append_attr_state(buf, "nkro", keymap_config.nkro);
#endif
}
#endif

void cmd_reset(char* cmd, char* buf, int size) {
    bootloader_jump();
}

#ifdef ENABLE_INFO
void cmd_rgb(char* cmd, char* buf, int size) {
    if (!rgblight_is_enabled()) {
        strcat(buf, "OFF");
        return;
    }
    append_attr_value(buf, "mode", rgblight_get_mode());
    append_attr_value(buf, " hue", rgblight_get_hue());
    append_attr_value(buf, "\nsat", rgblight_get_sat());
    append_attr_value(buf, " val", rgblight_get_val());
    append_attr_value(buf, "\nspd", rgblight_get_speed());
}
#endif

#ifdef ENABLE_TIMER
void cmd_timer(char* cmd, char* buf, int size) {
    setTimer(atoi(cmd + 6));
    strcat(buf, "ARMED");
}
#endif

#ifdef ENABLE_SPEED
void cmd_speed(char* cmd, char* buf, int size) {
    int arg = atoi(cmd + 6);
    rgblight_set_speed(arg);
    strcat(buf, "Speed:");
    utoa(arg, buf + strlen(buf), 10);
}
#endif

void cmd_unknown(char* cmd, char* buf, int size) {
    strcat(buf, "?: ");
    strcat(buf, cmd);
}

#ifdef ENABLE_STATUS
void cmd_status(char* cmd, char* buf, int size) {
    int i;
    for (i = 0; i < sizeof(statusLine) && cmd[i + 7]!= 0; ++i) {
        statusLine[i] = cmd[i + 7];
    }
    for (; i < sizeof(statusLine); ++i) statusLine[i] = ' ';
}
#endif

void cmd_help(char* cmd, char* buf, int size);

typedef struct command {
    const char* cmd;
    void (*handler) (char* cmd, char* buf, int size);
} command_t;

#define DEFINE_COMMAND(name) {#name, cmd_##name}

command_t commands[] = {
    DEFINE_COMMAND(ver),
#ifdef ENABLE_UPTIME
    DEFINE_COMMAND(uptime),
#endif
#ifdef ENABLE_INFO
    DEFINE_COMMAND(info),
    DEFINE_COMMAND(rgb),
#endif
    DEFINE_COMMAND(help),
#ifdef ENABLE_TIMER
    DEFINE_COMMAND(timer),
#endif
    DEFINE_COMMAND(reset),
#ifdef ENABLE_SPEED
    DEFINE_COMMAND(speed),
#endif
#ifdef ENABLE_STATUS
    DEFINE_COMMAND(status),
#endif
#ifdef ENABLE_CLOCK
    DEFINE_COMMAND(time),
#endif
#ifdef ENABLE_CMDMODE
    DEFINE_COMMAND(exit),
#endif
    {"", NULL},
};

const int numCommands = sizeof(commands) / sizeof(command_t);

void cmd_help(char* cmd, char* buf, int size) {
    strcat(buf, "cmds: ");
    for (int i = 0; i < numCommands; ++i) {
        strcat(buf, commands[i].cmd);
        strcat(buf, " ");
    }
}

void handle_command(char* cmd, char* buf, int size) {
    for (command_t* command = &commands[0]; command->handler != NULL; ++command) {
        if (memcmp(cmd, command->cmd, strlen(command->cmd)) == 0) {
#ifdef ENABLE_OLED
            infoLine[0] = '!';
            strlcpy(infoLine + 1, command->cmd, sizeof(infoLine) - 1);
#endif
            return (*command->handler)(cmd, buf, size);
        }
    }
    cmd_unknown(cmd, buf, size);
}
#endif // ENABLE_CMDS


