#include QMK_KEYBOARD_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "alarm.h"
#include "common.h"

#ifdef ENABLE_ALARM
uint32_t last_alarm_time = 0;
uint32_t alarmStart = 0;
int16_t alarmLimit = 0;

void setAlarm(int16_t timeout) {
    alarmStart = timer_read32();
    alarmLimit = timeout;
}

bool alarmArmed(void) {
    return alarmLimit != 0;
}

int16_t alarmRemaining(void) {
    return alarmLimit - timer_elapsed32(alarmStart) / ms_per_sec;
}

uint32_t alarmTime(void) {
    return timer_elapsed32(last_alarm_time);
}

bool alarmTriggered(void) {
    if (alarmArmed() && alarmRemaining() < 0) {
        last_alarm_time = timer_read32();
        alarmLimit = 0;
        return true;
    }
    return false;
}

void alarmKey(void) {
    if (!alarmArmed()) {
        setAlarm(10);
    } else if (alarmLimit == 10) {
        setAlarm(60);
    }
}

void cmd_alarm(char* cmd, char* buf, int size) {
    setAlarm(atoi(cmd + 6));
    strcat(buf, "ARMED");
}
#endif
