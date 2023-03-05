#pragma once

#define DYNAMIC_KEYMAP_LAYER_COUNT 3

#define TAPPING_TERM 100

#define PERMISSIVE_HOLD

#define RGBLIGHT_SLEEP
#define RGBLIGHT_DISABLE_KEYCODES
#define RGBLIGHT_DEFAULT_MODE RGBLIGHT_MODE_RAINBOW_SWIRL
#undef RGBLIGHT_ANIMATIONS

// #define RGBLIGHT_EFFECT_ALTERNATING // Enable alternating animation mode.
// #define RGBLIGHT_EFFECT_BREATHING // Enable breathing animation mode.
// #define RGBLIGHT_EFFECT_CHRISTMAS // Enable christmas animation mode.
// #define RGBLIGHT_EFFECT_KNIGHT // Enable knight animation mode.
#define RGBLIGHT_EFFECT_RAINBOW_MOOD // Enable rainbow mood animation mode.
#define RGBLIGHT_EFFECT_RAINBOW_SWIRL // Enable rainbow swirl animation mode.
// #define RGBLIGHT_EFFECT_RGB_TEST // Enable RGB test animation mode.
// #define RGBLIGHT_EFFECT_SNAKE // Enable snake animation mode.
// #define RGBLIGHT_EFFECT_STATIC_GRADIENT // Enable static gradient mode.
// #define RGBLIGHT_EFFECT_TWINKLE // Enable twinkle animation mode.

#undef LOCKING_SUPPORT_ENABLE
#define NO_ACTION_ONESHOT

#define USE_I2C
#undef SOFT_SERIAL_PIN

#define OLED_FONT_H "users/henryhu/glcdfont.c"

#define LAYER_STATE_8BIT

#undef ENCODERS_PAD_A
#undef ENCODERS_PAD_B
#define ENCODERS_PAD_A { NO_PIN, NO_PIN }
#define ENCODERS_PAD_B { NO_PIN, NO_PIN }

#define DISABLE_SYNC_TIMER

#define OLED_TIMEOUT 0
#define OLED_DISPLAY_128X64
