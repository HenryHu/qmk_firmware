#include QMK_KEYBOARD_H

#include <stdbool.h>
#include <stdint.h>

#include "alttab.h"

#ifdef ENABLE_ALTTAB
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

const uint16_t alt_tab_timeout = 300;

void alttab_scan(void) {
    if (is_alt_tab_active && timer_elapsed(alt_tab_timer) > alt_tab_timeout) {
        is_alt_tab_active = false;
        unregister_code(KC_LALT);
    }
}

void alttab_down(void) {
    if (!is_alt_tab_active) {
        is_alt_tab_active = true;
        register_code(KC_LALT);
    }
    alt_tab_timer = timer_read();
    register_code(KC_TAB);
}

void alttab_up(void) {
    unregister_code(KC_TAB);
}
#endif
