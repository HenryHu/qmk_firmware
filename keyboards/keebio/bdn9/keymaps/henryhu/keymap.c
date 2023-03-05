/* Copyright 2019 Danny Nguyen <danny@keeb.io>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

enum encoder_names {
  _LEFT,
  _RIGHT,
  _MIDDLE,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    /*
        | Knob 1: Vol Dn/Up |      | Knob 2: Page Dn/Up |
        | Press: Mute       | Home | Press: Play/Pause  |
        | Hold: Layer 2     | Up   | RGB Mode           |
        | Left              | Down | Right              |
     */
    [0] = LAYOUT(
        RGB_MOD, KC_BTN1, KC_BTN2,
        MO(1)  , KC_UP  , MO(2),
        C(KC_D), KC_DOWN, C(KC_E)
    ),
    /*
        | RESET          | N/A  | Media Stop |
        | Held: Layer 2  | Home | RGB Mode   |
        | Media Previous | End  | Media Next |
     */
    [1] = LAYOUT(
        RESET  , KC_VOLU, KC_VOLD,
        _______, KC_MUTE, MO(3),
        KC_MPRV, KC_MPLY, KC_MNXT
    ),
    [2] = LAYOUT(
        RESET  , KC_PGUP, KC_PGDN,
        KC_INS , KC_HOME, _______,
        KC_BTN4, KC_END , KC_BTN5
    ),
    [3] = LAYOUT(
        RESET  , KC_BTN1, KC_BTN2,
        _______, KC_MS_U, _______,
        KC_MS_L, KC_MS_D , KC_MS_R
    ),
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    switch (get_highest_layer(layer_state)) {
        case 1:
            if (index == _LEFT) {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
            else if (index == _MIDDLE) {
                if (clockwise) {
                    tap_code(KC_MPRV);
                } else {
                    tap_code(KC_MNXT);
                }
            }
            else if (index == _RIGHT) {
                if (clockwise) {
                    register_code(KC_LALT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LALT);
                } else {
                    register_code(KC_LALT);
                    register_code(KC_LSHIFT);
                    tap_code(KC_TAB);
                    unregister_code(KC_LSHIFT);
                    unregister_code(KC_LALT);
                }
            }
        default:
            if (index == _LEFT) {
                if (clockwise) {
                    tap_code(KC_VOLU);
                } else {
                    tap_code(KC_VOLD);
                }
            }
            else if (index == _MIDDLE) {
                if (clockwise) {
                    tap_code(KC_WH_D);
                } else {
                    tap_code(KC_WH_U);
                }
            }
            else if (index == _RIGHT) {
                if (clockwise) {
                    tap_code16(A(KC_TAB));
                } else {
                    tap_code16(A(S(KC_TAB)));
                }
            }
    }
    return true;
}

/*
void raw_hid_receive(uint8_t *data, uint8_t length) {
}
*/
