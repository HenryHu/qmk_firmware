#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "options.h"
#include "common.h"
#include "key.h"
#include "serport.h"
#include "oled.h"
#include "alttab.h"
#include "alarm.h"

#define MISSING _______

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_80_with_macro(
    KC_MUTE,                  KC_ESC,  KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,           KC_INS,  KC_DEL,
    S(C(KC_ESC)), KC_PSCR,    KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,         KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  MISSING, KC_BSPC, KC_HOME,
    S(C(KC_T)),   KC_SCRL,    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                  KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_PGUP,
    MACRO1,       KC_PAUS,    MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                  KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,          KC_ENT,  KC_PGDN,
    MACRO2,       KC_PAST,    KC_LSFT,          KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,         KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENTER), KC_UP,   KC_END,
    KC_MPLY,      KC_F10,     KC_LCTL, MO(2),   KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                MISSING, KC_SPC,           KC_RALT, KC_RCTL, MISSING, KC_LEFT, KC_DOWN, KC_RGHT
  ),
  [1] = LAYOUT_80_with_macro(
    _______,                  QK_BOOT, _______, _______, _______, _______, _______, _______,      _______, _______, _______, NK_TOGG, KC_MSEL, KC_EJCT,          KC_SLEP, KC_PWR,
    RGB_HUI,      RGB_HUD,    RGB_TOG, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  MISSING, _______, KC_WAKE,
    RGB_SAI,      RGB_SAD,    KC_CAPS, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, _______,               _______, _______, _______, _______, KC_PSCR, _______, _______, _______, _______,
    RGB_VAI,      RGB_VAD,    _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_BTN5,               KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SCRL, KC_WSCH,          _______, _______,
    MACRO3,       RGB_MOD,    _______,          KC_WH_U, KC_WH_D, KC_WH_L, KC_WH_R, _______,      KC_CALC, KC_MAIL, KC_WBAK, KC_WFWD, KC_PSLS, _______,          KC_MPRV, _______,
    MACRO4,       _______,    CMDMODE, _______, KC_RGUI, KC_RALT, MO(2),   MO(2),                 MISSING, _______,          KC_APP,  _______, MISSING, KC_MRWD, KC_MNXT, KC_MFFD
  ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    return record->event.pressed ?
        process_key_down(keycode, record) :
        process_key_up(keycode, record);
}

void matrix_scan_user(void) {
#ifdef ENABLE_ALTTAB
    alttab_scan();
#endif
#ifdef ENABLE_ALARM
    if (alarmTriggered()) {
#ifdef ENABLE_SERIAL
        serial_send("> ALARM!\a\r\n");
#endif
#ifdef ENABLE_OLED
        infoLine[0] = 0;
#endif
    }
#endif
}
