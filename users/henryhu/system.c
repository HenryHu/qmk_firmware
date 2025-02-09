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

void matrix_scan_user(void) {
#ifdef ENABLE_ALTTAB
    alttab_scan();
#endif
#ifdef ENABLE_ALARM
    if (alarmTriggered()) {
#ifdef ENABLE_SERIAL
        serial_send("> ALARM!\a\r\n");
#endif
#ifdef ENABLE_OLED
        infoLine[0] = 0;
#endif
    }
#endif
}
