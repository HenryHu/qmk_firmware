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
    MACRO2,     KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,                 KC_UP,
    ALT_TAB,    KC_LCTL, KC_CAPS, KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                    MO(1),   KC_SPC,  MO(2),   KC_RGUI,                   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    RESET,               RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,          _______, _______,          _______, _______, KC_MSEL, KC_EJCT, KC_SLEP, KC_PWR,  KC_WAKE,
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

bool process_key_down(uint16_t keycode, keyrecord_t *record) {
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
            SEND_STRING(MACRO1_STRING);
            break;
        case MACRO2:
            SEND_STRING(MACRO2_STRING);
            break;
        case MACRO3:
            SEND_STRING(MACRO3_STRING);
            break;
        case MACRO4:
            SEND_STRING(MACRO4_STRING);
            break;
        case MACRO5:
            SEND_STRING(MACRO5_STRING);
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

void matrix_scan_user(void) {
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > 300) {
        is_alt_tab_active = false;
        unregister_code(KC_LALT);
    }
}

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
    const uint32_t now = timer_read32();
    itoa(now / 1000, buf, 10);
    strcat(buf, "s ");
    itoa(now % 1000, buf + strlen(buf), 10);
    strcat(buf, "ms");
}

void cmd_info(char* buf, int size) {
    strcat(buf, "Henry's keyboard " QMK_KEYBOARD "/" QMK_KEYMAP);
}

void cmd_unknown(char* buf, int size) {
    strcat(buf, "Unknown command: '");
    strcat(buf, serialBuffer);
    strcat(buf, "', len: ");
    itoa(strlen(serialBuffer), buf + strlen(buf), 10);
}

void cmd_help(char* buf, int size);

typedef struct command {
    const char* cmd;
    void (*handler) (char* buf, int size);
} command_t;

command_t commands[] = {
    {"ver", cmd_ver},
    {"uptime", cmd_uptime},
    {"info", cmd_info},
    {"help", cmd_help},
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
        if (strncmp(serialBuffer, commands[i].cmd, sizeof(serialBuffer)) == 0) {
            return (*commands[i].handler)(buf, size);
        }
    }
    cmd_unknown(buf, size);
}

void process_serial_command(void) {
    char buf[64];
    buf[63] = 0;
    strcpy(buf, "> ");
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
        if (clockwise) {
            tap_code(KC_VOLU);
        } else {
            tap_code(KC_VOLD);
        }
    } else if (index == RIGHT_HALF_ENC2) {
        if (clockwise) {
            tap_code16(C(KC_PLUS));
        } else {
            tap_code16(C(KC_MINUS));
        }
    }
    return true;
}
