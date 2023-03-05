#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "key.h"
#include "oled.h"
#include "common.h"

enum encoder_names {
  LEFT_HALF_ENC = 0,
  RIGHT_HALF_ENC1 = 2,
  RIGHT_HALF_ENC2,
};

const uint16_t init_time = 500;
const uint8_t oled_brightness_step = 25;

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
            setInfoLine_P(PSTR("Hue"));
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
        } else if (IS_LAYER_ON(1)) {
            setInfoLine_P(PSTR("Sat"));
            if (clockwise) {
                rgblight_increase_sat_noeeprom();
            } else {
                rgblight_decrease_sat_noeeprom();
            }
        } else if (IS_LAYER_ON(2)) {
            setInfoLine_P(PSTR("Val"));
            if (clockwise) {
                rgblight_increase_val_noeeprom();
            } else {
                rgblight_decrease_val_noeeprom();
            }
        } else {
            setInfoLine("Vol");
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    } else if (index == RIGHT_HALF_ENC2) {
        if (IS_LAYER_ON(3)) {
            setInfoLine("Scale");
            if (clockwise) {
                tap_code16(C(KC_EQL));
            } else {
                tap_code16(C(KC_MINUS));
            }
        } else if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_step();
                setInfoLine("RGB +: ");
                appendInfoLine(rgblight_get_mode());
            } else {
                rgblight_step_reverse();
                setInfoLine("RGB -: ");
                appendInfoLine(rgblight_get_mode());
            }
            return true;
        } else if (IS_LAYER_ON(2)) {
            setInfoLine("OLED");
            if (clockwise) {
                oled_set_brightness(oled_get_brightness() + oled_brightness_step);
            } else {
                oled_set_brightness(oled_get_brightness() - oled_brightness_step);
            }
        } else {
            setInfoLine("Wheel");
            if (clockwise) {
                tap_code(KC_WH_D);
            } else {
                tap_code(KC_WH_U);
            }
        }
    }
    if (clockwise) {
        strcat_P(infoLine, PSTR(" +"));
    } else {
        strcat_P(infoLine, PSTR(" -"));
    }
    return true;
}
