#include QMK_KEYBOARD_H

#include "timer.h"

#ifdef ENABLE_TIMER
uint32_t last_alarm_time = 0;
uint32_t timerStart = 0;
uint32_t timerLimit = 0;

void setTimer(int timeout) {
    timerStart = timer_read32();
    timerLimit = timeout * 1000L;
}

bool timerArmed(void) {
    return timerLimit != 0;
}

uint32_t timerRemaining(void) {
    return (timerLimit - timer_elapsed32(timerStart)) / 1000;
}

uint32_t alarmTime(void) {
    return timer_elapsed32(last_alarm_time);
}

bool timerTriggered(void) {
    if (timerArmed() && timer_elapsed32(timerStart) > timerLimit) {
        last_alarm_time = timer_read32();
        timerLimit = 0;
        return true;
    }
    return false;
}

void timerKey(void) {
    if (!timerArmed()) {
        setTimer(10);
    } else if (timerLimit == 10000) {
        setTimer(60);
    }
}

#endif
