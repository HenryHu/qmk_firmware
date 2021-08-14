#pragma once

#include "options.h"

#include <stdint.h>

#ifdef ENABLE_ALARM

void setAlarm(int timeout);
bool alarmArmed(void);
uint32_t alarmRemaining(void);
uint32_t alarmTime(void);
bool alarmTriggered(void);
void alarmKey(void);
void cmd_alarm(char* cmd, char* buf, int size);

#endif
