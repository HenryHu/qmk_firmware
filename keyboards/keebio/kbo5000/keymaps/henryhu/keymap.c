#include QMK_KEYBOARD_H

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
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [0] = LAYOUT_all(
    KC_ESC,                 KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,            KC_F9,   KC_F10,  KC_F11,  KC_F12,  KC_PSCR, KC_SLCK, KC_PAUS,
    S(KC_INS),     KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,    KC_6,             KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, KC_EQL,  KC_DEL,  KC_BSPC, KC_INS,  KC_PGUP,
    S(C(KC_T)),    KC_TAB,  KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                      KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    KC_LBRC, KC_RBRC, KC_BSLS, KC_HOME, KC_END,
    MACRO1, LT(1, KC_CAPS), KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                      KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT, KC_NUHS, KC_ENT,  KC_DEL,  KC_PGDN,
    MACRO2,        KC_LSFT, KC_NUBS, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,             KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_SFTENT,                 KC_UP,
    KC_LCTL,       KC_LCTL, MO(1),   KC_LGUI, KC_LALT, KC_SPC,  KC_SPC,                    MO(1),   KC_SPC,  MO(2),   KC_RGUI,                   KC_RCTL, KC_LEFT, KC_DOWN, KC_RGHT
  ),

  [1] = LAYOUT_all(
    RESET,            RGB_HUI, RGB_HUD, RGB_SAI, RGB_SAD, RGB_VAI, RGB_VAD,          _______, _______,          _______, _______, KC_MSEL, KC_EJCT, KC_SLEP, KC_PWR,  KC_WAKE,
    RGB_TOG, _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,            KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  _______, _______, KC_MPLY, KC_MPRV,
    RGB_MOD, _______, KC_BTN1, KC_MS_U, KC_BTN2, KC_BTN3, _______,                   _______, _______, _______, _______, KC_PSCR, KC_BRID, KC_BRIU, KC_NLCK, KC_MRWD, KC_MFFD,
    MACRO3,  _______, KC_MS_L, KC_MS_D, KC_MS_R, KC_BTN4, KC_BTN5,                   KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, KC_SLCK, KC_WSCH, _______, KC_PENT, KC_MSTP, KC_MNXT,
    MACRO4,  _______, _______, KC_WH_U, KC_WH_D, KC_WH_L, KC_WH_R, _______,          KC_CALC, KC_MAIL, KC_WBAK, KC_WFWD, KC_WREF,          _______,          _______,
    MACRO5,  _______, _______, KC_RWIN, KC_RALT, _______, _______,                   MO(1),   _______, KC_APP,  _______,                   _______, _______, _______, _______
  ),

  [2] = LAYOUT_all(
    _______,          _______, _______, _______, _______, _______, _______,          _______, _______,          _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
    _______, _______, _______, _______, _______, _______, _______, _______,          _______, _______, _______, _______, _______,          _______,          _______,
    _______, _______, _______, _______, _______, _______, _______,                   _______, _______, _______, _______,                   _______, _______, _______, _______
  )
};

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
            tap_code(KC_BRIU);
        } else {
            tap_code(KC_BRID);
        }
    }
    return true;
}
