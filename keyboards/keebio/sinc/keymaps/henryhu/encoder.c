#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "key.h"
#include "oled.h"
#include "common.h"

enum encoder_names {
  LEFT_HALF_ENC = 0,
};

const uint16_t init_time = 500;
const uint8_t oled_brightness_step = 25;

bool encoder_update_user(uint8_t index, bool clockwise) {
    clockwise = !clockwise;
    if (timer_read() < init_time) return false;
    updateKeyDownTime();
    if (index == LEFT_HALF_ENC) {
        if (IS_LAYER_ON(1)) {
            if (clockwise) {
                rgblight_step();
                setInfoLine("RGB +: ");
                appendInfoLine(rgblight_get_mode());
            } else {
                rgblight_step_reverse();
                setInfoLine("RGB -: ");
                appendInfoLine(rgblight_get_mode());
            }
            return false;
        } else if (IS_LAYER_ON(2)) {
            setInfoLine("OLED");
            if (clockwise) {
                oled_set_brightness(oled_get_brightness() + oled_brightness_step);
            } else {
                oled_set_brightness(oled_get_brightness() - oled_brightness_step);
            }
        } else {
            setInfoLine("Vol");
            if (clockwise) {
                tap_code(KC_VOLU);
            } else {
                tap_code(KC_VOLD);
            }
        }
    }
    if (clockwise) {
        strcat_P(infoLine, PSTR(" +"));
    } else {
        strcat_P(infoLine, PSTR(" -"));
    }
    return false;
}
