#include QMK_KEYBOARD_H

#include "options.h"

#include "oled.h"
#include "clock.h"

bool shutdown_user(bool bootloader) {
#ifdef ENABLE_OLED
    shutdown_oled();
#endif
    return true;
}
