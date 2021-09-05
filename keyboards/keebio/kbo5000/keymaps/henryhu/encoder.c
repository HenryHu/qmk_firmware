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
            setInfoLine("Hue");
            if (clockwise) {
                rgblight_increase_hue();
            } else {
                rgblight_decrease_hue();
            }
        } else if (IS_LAYER_ON(1)) {
            setInfoLine("Sat");
            if (clockwise) {
                rgblight_increase_sat_noeeprom();
            } else {
                rgblight_decrease_sat_noeeprom();
            }
        } else if (IS_LAYER_ON(2)) {
            setInfoLine("Val");
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
        setInfoLine("Scale");
        if (IS_LAYER_ON(3)) {
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
                oled_set_brightness(oled_get_brightness() + 10);
            } else {
                oled_set_brightness(oled_get_brightness() - 10);
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
        strcat(infoLine, " +");
    } else {
        strcat(infoLine, " -");
    }
    return true;
}
