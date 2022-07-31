#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "options.h"
#include "common.h"
#include "key.h"
#include "serial.h"
#include "oled.h"
#include "alttab.h"
#include "alarm.h"

#define F11 KC_F11

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_ESC,              KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,            KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS,
    S(KC_INS),  KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_INS,  KC_PGUP,
    S(C(KC_T)), KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, KC_END,
    MACRO1,     MO(1),   KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_DEL,  KC_PGDN,
    MACRO2,     KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, RSFT_T(KC_ENTER),          KC_UP,
    ALT_TAB,    KC_LCTL, MO(2),   KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                    MO(1),   KC_SPC,  MO(2),   LT(1, KC_RGUI),            KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    RESET,               _______, _______, _______, _______, _______, _______,          _______, _______,          _______, NK_TOGG, KC_MSEL, KC_EJCT, KC_SLEP, KC_PWR,  KC_WAKE,
    RGB_TOG,    KC_PAST, KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_4, KC_KP_5, KC_KP_6,          KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_0, KC_PMNS, KC_PPLS, _______, _______, KC_MPLY, KC_MPRV,
    _______,    KC_CAPS, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, _______,                   _______, _______, _______, _______, KC_PSCR, KC_BRID, KC_BRIU, KC_NLCK, KC_MRWD, KC_MFFD,
    MACRO3,     _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_BTN5,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SLCK, KC_WSCH, _______, KC_PENT, KC_MSTP, KC_MNXT,
    MACRO4,    CMD_MODE, _______, KC_WH_U, KC_WH_D, KC_WH_L, KC_WH_R, _______,          KC_CALC, KC_MAIL, KC_WBAK, KC_WFWD, KC_PSLS,         _______,          _______,
    SET_ALARM,  _______, MO(3),   KC_RWIN, KC_RALT, MO(3),   MO(3),                     MO(1),   _______, KC_APP,  _______,                   _______, _______, _______, _______
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
