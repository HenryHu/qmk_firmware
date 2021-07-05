#include QMK_KEYBOARD_H
#include "virtser.h"
#include "version.h"
#include "macro_strings.h"

#define ENABLE_CLOCK  // 320B
#define ENABLE_INFO   // 360B
#define ENABLE_TIMER  // 500B
#define ENABLE_NEKO   // 480B + font
#define ENABLE_UPTIME //  50B
#define ENABLE_STATUS //  80B
#define ENABLE_OLED   // 3.7K
#define ENABLE_SERIAL // 1.9K
#define ENABLE_ALTTAB //  90B
#define ENABLE_MACRO  // 250B

enum encoder_names {
  LEFT_HALF_ENC = 0,
  RIGHT_HALF_ENC1 = 2,
  RIGHT_HALF_ENC2,
};

enum custom_keycodes {
    MACRO1 = SAFE_RANGE,
    MACRO2,
    MACRO3,
    MACRO4,
    SET_TIMER,
    ALT_TAB,
    CMD_MODE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_ESC,              KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,            KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS,
    S(KC_INS),  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_INS,  KC_PGUP,
    S(C(KC_T)), KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, KC_END,
    MACRO1,     MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_DEL,  KC_PGDN,
    MACRO2,     KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENTER),                 KC_UP,
    ALT_TAB,    KC_LCTL, MO(2),   KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                    MO(1),   KC_SPC,  MO(2),   KC_RGUI,                   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    RESET,               RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,          _______, _______,          _______, NK_TOGG, KC_MSEL, KC_EJCT, KC_SLEP, KC_PWR,  KC_WAKE,
    RGB_TOG,    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_MPLY, KC_MPRV,
    RGB_MOD,    KC_CAPS, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, _______,                   _______, _______, _______, _______, KC_PSCR, KC_BRID, KC_BRIU, KC_NLCK, KC_MRWD, KC_MFFD,
    MACRO3,     _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_BTN5,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SLCK, KC_WSCH, _______, KC_PENT, KC_MSTP, KC_MNXT,
    MACRO4,     CMD_MODE, _______, KC_WH_U, KC_WH_D, KC_WH_L, KC_WH_R, _______,          KC_CALC, KC_MAIL, KC_WBAK, KC_WFWD, KC_WREF,          _______,          _______,
    SET_TIMER,  _______, _______, KC_RWIN, KC_RALT, _______, _______,                   MO(1),   _______, KC_APP,  _______,                   _______, _______, _______, _______
  ),

  /*
  [2] = LAYOUT_all(
    _______,          _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______,          _______,          _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______,                   _______, _______, _______, _______
  )
  */
};

#ifdef ENABLE_ALTTAB
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
#endif
uint32_t last_key_down_time = 0;
#ifdef ENABLE_TIMER
uint32_t last_alarm_time = 0;
#endif
uint32_t time_base = 0;

#ifdef ENABLE_TIMER
uint32_t timerStart = 0;
uint32_t timerLimit = 0;

void setTimer(int timeout) {
    timerStart = timer_read32();
    timerLimit = timeout * 1000L;
}

bool timerArmed(void) {
    return timerLimit != 0;
}
#endif

bool command_mode = false;
char cmdBuf[64];
int cmdPos = 0;

char get_char_for_key(uint16_t keycode) {
    keycode = keycode & 0xFF;
    if (keycode >= KC_A && keycode <= KC_Z) {
        return keycode - KC_A + 'a';
    }
    if (keycode >= KC_1 && keycode <= KC_9) {
        return keycode - KC_1 + '1';
    }
    if (keycode == KC_0) return '0';
    if (keycode == KC_SPACE) return ' ';
    if (keycode == KC_BSPACE) return '\b';
    if (keycode == KC_ENTER) return '\n';
    return 0;
}

void command_mode_key(uint16_t keycode, keyrecord_t *record);
void handle_command(char* cmd, char* buf, int size);

bool process_key_down(uint16_t keycode, keyrecord_t *record) {
    last_key_down_time = timer_read32();
    switch (keycode) {
#ifdef ENABLE_ALTTAB
        case ALT_TAB:
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
            break;
#endif
        default:
            if (command_mode) {
                return false;
            }
    }
    return true;
}

void send_string_lite(const char* buf, const int len) {
    bool shift = false;
    for (uint8_t i = 0; i < len; ++i) {
        if (buf[i] == KC_LSFT) {
            if (shift) unregister_code(buf[i]); else register_code(buf[i]);
            shift = !shift;
        } else {
            tap_code(buf[i]);
        }
    }
}

bool process_key_up(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
#ifdef ENABLE_MACRO
        case MACRO1:
            send_string_lite(MACRO1_STRING, sizeof(MACRO1_STRING));
            break;
        case MACRO2:
            send_string_lite(MACRO2_STRING, sizeof(MACRO2_STRING));
            break;
        case MACRO3:
            send_string_lite(MACRO3_STRING, sizeof(MACRO3_STRING));
            break;
        case MACRO4:
            send_string_lite(MACRO4_STRING, sizeof(MACRO4_STRING));
            break;
#endif
#ifdef ENABLE_TIMER
        case SET_TIMER:
            if (!timerArmed()) {
                setTimer(10);
            } else if (timerLimit == 10000) {
                setTimer(60);
            }
            break;
#endif
#ifdef ENABLE_ALTTAB
        case ALT_TAB:
            unregister_code(KC_TAB);
            break;
#endif
        case CMD_MODE:
            command_mode = !command_mode;
            break;
        default:
            if (command_mode) {
                command_mode_key(keycode, record);
                return false;
            }
    }
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        return process_key_down(keycode, record);
    } else {
        return process_key_up(keycode, record);
    }
}

#ifdef ENABLE_STATUS
char statusLine[22];
#endif
#ifdef ENABLE_OLED
char infoLine[18];

void setInfoLine(const char* buf) {
    strcpy(infoLine, buf);
}
#else
void setInfoLine(const char* buf) {
}
#endif

void command_process(void) {
    infoLine[0] = 0;
    handle_command(cmdBuf, infoLine, sizeof(infoLine));
}

void command_mode_key(uint16_t keycode, keyrecord_t *record) {
    char ch = get_char_for_key(keycode);
    if (ch == '\n') {
        if (cmdPos > 0) command_process();
        cmdPos = 0;
        memset(cmdBuf, 0, sizeof(cmdBuf));
        return;
    }
    if (ch == '\b') {
        cmdBuf[cmdPos] = 0;
        if (cmdPos > 0) --cmdPos;
    }
    if (ch != 0) {
        cmdBuf[cmdPos++] = ch;
    }
    infoLine[0] = '?';
    strncpy(infoLine + 1, cmdBuf, sizeof(infoLine) - 1);
}

#ifdef ENABLE_SERIAL
char serialBuffer[64];
int serialPtr = 0;

void serial_send(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        virtser_send(str[i]);
    }
}

void cmd_ver(char* cmd, char* buf, int size) {
    strcat(buf, QMK_VERSION " " QMK_BUILDDATE);
}

#ifdef ENABLE_UPTIME
void cmd_uptime(char* cmd, char* buf, int size) {
    utoa(timer_read32() / 1000, buf, 10);
    strcat(buf, "s");
}
#endif

#ifdef ENABLE_INFO
void append_attr_state(char* buf, const char* name, const bool value) {
    strcat(buf, name);
    strcat(buf, ": ");
    strcat(buf, value ? "YES" : "NO");
}

void append_attr_value(char* buf, const char* name, const uint8_t value) {
    strcat(buf, name);
    strcat(buf, ": ");
    utoa(value, buf + strlen(buf), 10);
}

void cmd_info(char* cmd, char* buf, int size) {
    strcat(buf, QMK_KEYBOARD "/" QMK_KEYMAP "\r\n"
           "> " STR(MANUFACTURER) " " STR(PRODUCT) "\r\n"
           "> ");
    append_attr_state(buf, "nkro", keymap_config.nkro);
}
#endif

void cmd_reset(char* cmd, char* buf, int size) {
    serial_send("Bye!\r\n");
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
    append_attr_value(buf, " sat", rgblight_get_sat());
    append_attr_value(buf, " val", rgblight_get_val());
    append_attr_value(buf, " spd", rgblight_get_speed());
}
#endif

#ifdef ENABLE_TIMER
void cmd_timer(char* cmd, char* buf, int size) {
    setTimer(atoi(cmd + 6));
    strcat(buf, "Timer ARMED");
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
    strcat(buf, "?: '");
    strcat(buf, cmd);
    strcat(buf, "'");
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

#ifdef ENABLE_CLOCK
void cmd_time(char* cmd, char* buf, int size) {
    int hour = atoi(cmd + 5);
    int min = atoi(cmd + 8);
    int now = hour * 60 + min;
    time_base = now * 60L - timer_read32() / 1000;
}
#endif

void cmd_exit(char* cmd, char* buf, int size) {
    command_mode = false;
}

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
    DEFINE_COMMAND(exit),
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
    for (int i = 0; i < numCommands; ++i) {
        if (strncmp(cmd, commands[i].cmd, strlen(commands[i].cmd)) == 0) {
#ifdef ENABLE_OLED
            infoLine[0] = '!';
            strcpy(infoLine + 1, commands[i].cmd);
#endif
            return (*commands[i].handler)(cmd, buf, size);
        }
    }
    cmd_unknown(cmd, buf, size);
}

void process_serial_command(void) {
    char buf[256];
    buf[0] = '>';
    buf[1] = ' ';
    buf[2] = 0;
    buf[sizeof(buf) - 1] = 0;
    handle_command(serialBuffer, buf + 2, sizeof(buf) - 2);
    strcat(buf, "\r\n");
    serial_send(buf);
}

void virtser_recv(uint8_t in) {
    switch (in) {
        case 10:
        case 13:
            if (serialPtr == 0) break;
            serial_send("\r\n");
            if (serialPtr >= sizeof(serialBuffer)) serialPtr = sizeof(serialBuffer) - 1;
            serialBuffer[serialPtr] = 0;
            process_serial_command();
            serialPtr = 0;
            break;
        case 8:
        case 127:
            if (serialPtr > 0) --serialPtr;
            virtser_send(in);
            break;
        default:
            if (serialPtr >= sizeof(serialBuffer) - 1) return;
            serialBuffer[serialPtr++] = in;
            virtser_send(in);
    }
}
#endif

void matrix_scan_user(void) {
#ifdef ENABLE_ALTTAB
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 300) {
        is_alt_tab_active = false;
        unregister_code(KC_LALT);
    }
#endif
#ifdef ENABLE_TIMER
    if (timerArmed() && timer_elapsed32(timerStart) > timerLimit) {
#ifdef ENABLE_SERIAL
        serial_send("> TIMER!\a\r\n");
#endif
        last_alarm_time = timer_read32();
        timerLimit = 0;
#ifdef ENABLE_OLED
        infoLine[0] = 0;
#endif
    }
#endif
}

bool encoder_update_user(uint8_t index, bool clockwise) {
    /*
     * if (index == LEFT_HALF_ENC) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else */
    if (index == RIGHT_HALF_ENC1) {
        if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
        } else if (IS_LAYER_ON(2)) {
            if (clockwise) {
                tap_code(KC_WH_D);
            } else {
                tap_code(KC_WH_U);
            }
        } else {
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    } else if (index == RIGHT_HALF_ENC2) {
        if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_step();
            } else {
                rgblight_step_reverse();
            }
        } else if (IS_LAYER_ON(2)) {
            if (clockwise) {
            } else {
            }
        } else {
            if (clockwise) {
                tap_code16(C(KC_EQL));
            } else {
                tap_code16(C(KC_MINUS));
            }
        }
    }
    return true;
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

#ifdef ENABLE_NEKO
extern uint8_t* oled_cursor;
extern uint8_t oled_buffer[];
extern uint8_t oled_rotation_width;
void oled_set_cursor_raw(int x, int y) {
    oled_cursor = &oled_buffer[y * oled_rotation_width / 8 + x];
}

const int8_t frame_offset[] = {
    0, 2, -2, -2, -1, 2, 0, 2
};

void clear_canvas(uint8_t start_x, uint8_t x_len,
                  uint8_t start_row, uint8_t row_count) {
    for (uint8_t row = start_row; row < start_row + row_count; ++row) {
        for (uint8_t x = start_x; x < start_x + x_len; ++x) {
            oled_write_raw_byte(0, row * oled_rotation_width + x);
        }
    }
}

void render_animation(uint8_t col, uint8_t row, int frame) {
    clear_canvas(col * OLED_FONT_WIDTH - 2, 4 * OLED_FONT_WIDTH + 4, row, 3);
    const uint8_t start = 0x80;
    const uint8_t lineLen = 0x20;
    for (uint8_t x = 0; x < 3; ++x) {
        oled_set_cursor_raw(col * OLED_FONT_WIDTH + frame_offset[frame], (row + x) * OLED_FONT_HEIGHT);
        for (uint8_t y = 0; y < 4; ++y) {
            oled_write_char(start + x * lineLen + y + frame * 4, false);
        }
    }
}

int frame = 0;
uint32_t last_frame_time = 0;
int frame_time = 500;

/*
enum NekoState {
    IDLE = 0,
    FLAP = 1,
    SCRATCH = 2,
    SLEEP = 3,
};

enum NekoState neko_state = IDLE;
*/

void neko_sleep(void) {
    frame = frame == 0 ? 6 : frame == 6 ? 0 : frame == 5 ? 0 : 5;
    frame_time = 500;
}

void neko_scratch(void) {
    frame = frame == 2 ? 3 : 2;
    frame_time = 100;
}

void neko_flap(void) {
    frame = frame == 1 ? 7 : 1;
    frame_time = 200;
}

void neko_idle(void) {
    frame = 1;
    frame_time = 1000;
}

void neko_awake(void) {
    frame = 4;
    frame_time = 100;
}

void pick_frame(uint32_t idle_time) {
    if (idle_time > 30000) {
        neko_sleep();
    } else if (idle_time > 15000) {
        neko_scratch();
    } else if (idle_time > 5000) {
        neko_flap();
    } else if (idle_time > 100 && idle_time < 5000) {
        neko_idle();
    } else {
        neko_awake();
    }
}
#endif

#ifdef ENABLE_CLOCK
void get_time(char* buf) {
    const uint32_t now = (time_base + timer_read32() / 1000) % 86400;
    const uint32_t hour = now / 3600;
    const uint32_t min = now % 3600 / 60;
    buf[0] = 0;
    if (hour < 10) strcat(buf, " ");
    utoa(hour, buf + strlen(buf), 10);
    strcat(buf, ":");
    if (min < 10) strcat(buf, "0");
    utoa(min, buf + strlen(buf), 10);
}
#endif

bool get_infoline(void) {
#ifdef ENABLE_TIMER
    if (timerArmed()) {
        strcpy(infoLine, "Timer: ");
        utoa((timerLimit - timer_elapsed32(timerStart)) / 1000,
                infoLine + strlen(infoLine), 10);
        strcat(infoLine, "s");
        return true;
    }
#endif
    return false;
}

#ifdef ENABLE_OLED
void oled_task_user(void) {
    uint32_t idle_time = timer_elapsed32(last_key_down_time);
    const uint32_t alarm_time = timer_elapsed32(last_alarm_time);
    if (alarm_time < idle_time) idle_time = alarm_time;
    if (idle_time > 60000) {
        oled_off();
        return;
    }
    if (IS_LAYER_ON(1)) {
        oled_write_P(PSTR("HYPER "), false);
    } else {
        oled_write_P(PSTR("      "), false);
    }
    led_t led_state = host_keyboard_led_state();
    oled_write_P(led_state.caps_lock ? PSTR("CAP") : PSTR("   "),
            led_state.caps_lock);
    oled_write_P(led_state.scroll_lock ? PSTR("SCR") : PSTR("   "),
            led_state.scroll_lock);
    oled_write_P(led_state.num_lock ? PSTR("NUM\n") : PSTR("   \n"),
            led_state.num_lock);
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
        uint32_t now = timer_read32();
        if (now > last_frame_time + frame_time || idle_time < 100) {
            last_frame_time = now;
            pick_frame(idle_time);
        }
    }
    render_animation(17, 0, frame);
#endif

    get_infoline();
    oled_set_cursor(0, 2);
    oled_write("                 ", false);
    oled_set_cursor(0, 2);
    oled_write(infoLine, false);

#ifdef ENABLE_STATUS
    oled_set_cursor(0, 3);
    statusLine[sizeof(statusLine) - 1] = 0;
    oled_write(statusLine, false);
#endif
}
#endif
