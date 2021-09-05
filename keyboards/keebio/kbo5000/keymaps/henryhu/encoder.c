#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "key.h"
#include "oled.h"

enum encoder_names {
  LEFT_HALF_ENC = 0,
  RIGHT_HALF_ENC1 = 2,
  RIGHT_HALF_ENC2,
};

const uint16_t init_time = 500;

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (timer_read() < init_time) return true;
    updateKeyDownTime();
    /*
     * if (index == LEFT_HALF_ENC) {
        if (clockwise) {
            tap_code(KC_PGDN);
        } else {
            tap_code(KC_PGUP);
        }
    } else */
    if (index == RIGHT_HALF_ENC1) {
        if (IS_LAYER_ON(3)) {
            if (clockwise) {
                rgblight_increase_hue();
                setInfoLine("Hue UP");
            } else {
                rgblight_decrease_hue();
                setInfoLine("Hue DOWN");
            }
        } else if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_increase_sat_noeeprom();
                setInfoLine("Sat UP");
            } else {
                rgblight_decrease_sat_noeeprom();
                setInfoLine("Sat DOWN");
            }
        } else if (IS_LAYER_ON(2)) {
            if (clockwise) {
                rgblight_increase_val_noeeprom();
                setInfoLine("Val UP");
            } else {
                rgblight_decrease_val_noeeprom();
                setInfoLine("Val DOWN");
            }
        } else {
            if (clockwise) {
                tap_code(KC_VOLU);
                setInfoLine("Vol UP");
            } else {
                tap_code(KC_VOLD);
                setInfoLine("Vol DOWN");
            }
        }
    } else if (index == RIGHT_HALF_ENC2) {
        if (IS_LAYER_ON(3)) {
            if (clockwise) {
                tap_code16(C(KC_EQL));
                setInfoLine("Scale UP");
            } else {
                tap_code16(C(KC_MINUS));
                setInfoLine("Scale DOWN");
            }
        } else if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_step();
                setInfoLine("RGB NEXT");
            } else {
                rgblight_step_reverse();
                setInfoLine("RGB PREV");
            }
        } else if (IS_LAYER_ON(2)) {
            if (clockwise) {
                oled_set_brightness(oled_get_brightness() + 10);
                setInfoLine("OLED UP");
            } else {
                oled_set_brightness(oled_get_brightness() - 10);
                setInfoLine("OLED DOWN");
            }
        } else {
            if (clockwise) {
                tap_code(KC_WH_D);
                setInfoLine("Wheel UP");
            } else {
                tap_code(KC_WH_U);
                setInfoLine("Wheel DOWN");
            }
        }
    }
    return true;
}
