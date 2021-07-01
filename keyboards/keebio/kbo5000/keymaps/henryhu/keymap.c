#include QMK_KEYBOARD_H
#include "virtser.h"
#include "version.h"
#include "macro_strings.h"

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
    MACRO5,
    ALT_TAB,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_ESC,              KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,            KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS,
    S(KC_INS),  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_INS,  KC_PGUP,
    S(C(KC_T)), KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, KC_END,
    MACRO1,     MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_DEL,  KC_PGDN,
    MACRO2,     KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENTER),                 KC_UP,
    ALT_TAB,    KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                    MO(1),   KC_SPC,  MO(2),   KC_RGUI,                   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    RESET,               RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,          _______, _______,          _______, NK_TOGG, KC_MSEL, KC_EJCT, KC_SLEP, KC_PWR,  KC_WAKE,
    RGB_TOG,    _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_MPLY, KC_MPRV,
    RGB_MOD,    _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, _______,                   _______, _______, _______, _______, KC_PSCR, KC_BRID, KC_BRIU, KC_NLCK, KC_MRWD, KC_MFFD,
    MACRO3,     _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_BTN5,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SLCK, KC_WSCH, _______, KC_PENT, KC_MSTP, KC_MNXT,
    MACRO4,     _______, _______, KC_WH_U, KC_WH_D, KC_WH_L, KC_WH_R, _______,          KC_CALC, KC_MAIL, KC_WBAK, KC_WFWD, KC_WREF,          _______,          _______,
    MACRO5,     _______, _______, KC_RWIN, KC_RALT, _______, _______,                   MO(1),   _______, KC_APP,  _______,                   _______, _______, _______, _______
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

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
uint32_t last_key_down_time = 0;
uint32_t last_alarm_time = 0;

uint32_t timerStart = -1;
uint32_t timerLimit = -1;

void setTimer(uint32_t timeout) {
    timerStart = timer_read32();
    timerLimit = timeout;
}

bool process_key_down(uint16_t keycode, keyrecord_t *record) {
    last_key_down_time = timer_read32();
    switch (keycode) {
        case ALT_TAB:
            if (!is_alt_tab_active) {
                is_alt_tab_active = true;
                register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
            break;
    }
    return true;
}

bool process_key_up(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case MACRO1:
            SEND_STRING(STR(MACRO1_STRING));
            break;
        case MACRO2:
            SEND_STRING(STR(MACRO2_STRING));
            break;
        case MACRO3:
            SEND_STRING(STR(MACRO3_STRING));
            break;
        case MACRO4:
            SEND_STRING(STR(MACRO4_STRING));
            break;
        case MACRO5:
            if (timerLimit == -1) {
                setTimer(10000);
            } else if (timerLimit == 10000) {
                setTimer(60000);
            }
            break;
        case ALT_TAB:
            unregister_code(KC_TAB);
            break;
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

char statusLine[22];
char infoLine[18];
char serialBuffer[64];
int serialPtr = 0;

void serial_send(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        virtser_send(str[i]);
    }
}

void cmd_ver(char* buf, int size) {
    strcat(buf, QMK_VERSION " " QMK_BUILDDATE);
}

void cmd_uptime(char* buf, int size) {
    uint32_t uptime = timer_read32();
    utoa(uptime / 1000, buf, 10);
    strcat(buf, "s ");
    utoa(uptime % 1000, buf + strlen(buf), 10);
    strcat(buf, "ms");
}

void append_attr_state(char* buf, const char* name, const bool value) {
    strcat(buf, name);
    strcat(buf, ": ");
    strcat(buf, value ? "YES" : "NO");
}

void cmd_info(char* buf, int size) {
    strcat(buf, "Henry's keyboard " QMK_KEYBOARD "/" QMK_KEYMAP "\r\n");
    strcat(buf, "> " STR(MANUFACTURER) " " STR(PRODUCT) "\r\n");
    strcat(buf, "> ");
    append_attr_state(buf, "nkro", keymap_config.nkro);
}

void cmd_reset(char* buf, int size) {
    serial_send("Bye!\r\n");
    bootloader_jump();
}

void cmd_rgb(char* buf, int size) {
    if (!rgblight_is_enabled()) {
        strcat(buf, "RGB is OFF");
        return;
    }
    strcat(buf, "mode: ");
    utoa(rgblight_get_mode(), buf + strlen(buf), 10);
    strcat(buf, " hue: ");
    utoa(rgblight_get_hue(), buf + strlen(buf), 10);
    strcat(buf, " sat: ");
    utoa(rgblight_get_sat(), buf + strlen(buf), 10);
    strcat(buf, " val: ");
    utoa(rgblight_get_val(), buf + strlen(buf), 10);
    strcat(buf, " speed: ");
    utoa(rgblight_get_speed(), buf + strlen(buf), 10);
}

void cmd_timer(char* buf, int size) {
    setTimer(atol(serialBuffer + 6));
    strcat(buf, "Timer ARMED");
}

void cmd_speed(char* buf, int size) {
    long arg = atol(serialBuffer + 6);
    rgblight_set_speed(arg);
    strcat(buf, "Speed:");
    utoa(arg, buf + strlen(buf), 10);
}

void cmd_unknown(char* buf, int size) {
    strcat(buf, "Unknown command: '");
    strcat(buf, serialBuffer);
    strcat(buf, "', len: ");
    utoa(strlen(serialBuffer), buf + strlen(buf), 10);
}

void cmd_status(char* buf, int size) {
    int i;
    for (i = 0; i < sizeof(statusLine) && serialBuffer[i + 7]!= 0; ++i) {
        statusLine[i] = serialBuffer[i + 7];
    }
    for (; i < sizeof(statusLine); ++i) statusLine[i] = ' ';
}

uint32_t time_base = 0;

void cmd_time(char* buf, int size) {
    uint32_t arg = atol(serialBuffer + 5);
    time_base = arg - timer_read32() / 1000;
}

void cmd_help(char* buf, int size);

typedef struct command {
    const char* cmd;
    void (*handler) (char* buf, int size);
} command_t;

#define DEFINE_COMMAND(name) {#name, cmd_##name}

command_t commands[] = {
    DEFINE_COMMAND(ver),
    DEFINE_COMMAND(uptime),
    DEFINE_COMMAND(info),
    DEFINE_COMMAND(help),
    DEFINE_COMMAND(rgb),
    DEFINE_COMMAND(timer),
    DEFINE_COMMAND(reset),
    DEFINE_COMMAND(speed),
    DEFINE_COMMAND(status),
    DEFINE_COMMAND(time),
};

const int numCommands = sizeof(commands) / sizeof(command_t);

void cmd_help(char* buf, int size) {
    strcat(buf, "commands: ");
    for (int i = 0; i < numCommands; ++i) {
        strcat(buf, commands[i].cmd);
        strcat(buf, " ");
    }
}

void handle_serial_command(char* buf, int size) {
    for (int i = 0; i < numCommands; ++i) {
        if (strncmp(serialBuffer, commands[i].cmd, strlen(commands[i].cmd)) == 0) {
            return (*commands[i].handler)(buf, size);
        }
    }
    cmd_unknown(buf, size);
}

void process_serial_command(void) {
    char buf[256];
    buf[0] = '>';
    buf[1] = ' ';
    buf[2] = 0;
    buf[sizeof(buf) - 1] = 0;
    handle_serial_command(buf + 2, sizeof(buf) - 2);
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

void matrix_scan_user(void) {
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 300) {
        is_alt_tab_active = false;
        unregister_code(KC_LALT);
    }
    if (timerLimit != -1 && timer_elapsed32(timerStart) > timerLimit) {
        serial_send("> TIMER!\a\r\n");
        last_alarm_time = timer_read32();
        timerLimit = -1;
    }
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
        return;
    } else if (idle_time > 15000) {
        neko_scratch();
        return;
    } else if (idle_time > 5000) {
        neko_flap();
        return;
    } else if (idle_time > 100 && idle_time < 5000) {
        neko_idle();
        return;
    }
    neko_awake();
}

inline void get_time(char* buf) {
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

void get_infoline(void) {
    if (timerLimit != -1) {
        strcpy(infoLine, "Timer: ");
        itoa((timerLimit - timer_elapsed32(timerStart)) / 1000,
                infoLine + strlen(infoLine), 10);
        strcat(infoLine, "s");
    }
}

void oled_task_user(void) {
    uint32_t idle_time = timer_elapsed32(last_key_down_time);
    if (timer_elapsed32(last_alarm_time) < idle_time) {
        idle_time = timer_elapsed32(last_alarm_time);
    }
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

    char buf[7];
    get_time(buf);
    oled_write(buf, false);
    oled_write_P(PSTR("\n"), false);

	bool anime_pause = get_mods() & MOD_MASK_CTRL;

    // render_logo(17, 0);	

    if (!anime_pause) {
        uint32_t now = timer_read32();
        if (now > last_frame_time + frame_time || idle_time < 100) {
            last_frame_time = now;
            pick_frame(idle_time);
        }
    }
    render_animation(17, 0, frame);

    oled_set_cursor(0, 2);
    memset(infoLine, ' ', sizeof(infoLine));
    infoLine[sizeof(infoLine) - 1] = 0;
    oled_write(infoLine, false);
    oled_set_cursor(0, 2);
    get_infoline();
    oled_write(infoLine, false);

    oled_set_cursor(0, 3);
    memset(statusLine, ' ', sizeof(statusLine));
    statusLine[sizeof(statusLine) - 1] = 0;
    oled_write(statusLine, false);
    oled_set_cursor(0, 3);
    statusLine[sizeof(statusLine) - 1] = 0;
    oled_write(statusLine, false);
}
