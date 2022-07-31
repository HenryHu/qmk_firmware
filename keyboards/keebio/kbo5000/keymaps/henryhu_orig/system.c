#include QMK_KEYBOARD_H

#include "options.h"

#include "oled.h"
#include "clock.h"

void shutdown_user(void) {
#ifdef ENABLE_OLED
    shutdown_oled();
#endif
}
