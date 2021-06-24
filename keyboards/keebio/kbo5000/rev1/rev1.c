#include "kbo5000.h"
#include "split_util.h"

void matrix_init_kb(void) {
    setPinOutput(CAPS_LOCK_LED_PIN);
    matrix_init_user();
}

bool led_update_kb(led_t led_state) {
    // Only update if left half
    if (isLeftHand && led_update_user(led_state)) {
        writePin(CAPS_LOCK_LED_PIN, !led_state.caps_lock);
    }
    return true;
}

void eeconfig_init_kb(void) {
#ifdef BACKLIGHT_ENABLE
    backlight_enable();
    backlight_level(3);
#endif
#ifdef RGBLIGHT_ENABLE
    rgblight_enable(); // Enable RGB by default
    rgblight_sethsv(0, 255, 255);  // Set default HSV - red hue, full saturation, full brightness
#ifdef RGBLIGHT_ANIMATIONS
    rgblight_mode(RGBLIGHT_MODE_RAINBOW_SWIRL + 2); // set to RGB_RAINBOW_SWIRL by default
#endif
#endif

    eeconfig_update_kb(0);
    eeconfig_init_user();
}

const matrix_row_t matrix_mask[MATRIX_ROWS] =
{
//    0987654321
    0b0111111101,
    0b0111111101,
    0b0111111101,
    0b0111111101,
    0b0111111101,
    0b0111111101,
    0b1111111111,
    0b1111111111,
    0b1111111111,
    0b1111111111,
    0b1111111111,
    0b1111111111,
};
