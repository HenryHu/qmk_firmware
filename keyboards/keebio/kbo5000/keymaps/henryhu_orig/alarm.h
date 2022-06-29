#pragma once

#include "options.h"

#include <stdint.h>

#ifdef ENABLE_ALARM

void setAlarm(int16_t timeout);
bool alarmArmed(void);
int16_t alarmRemaining(void);
uint32_t alarmTime(void);
bool alarmTriggered(void);
void alarmKey(void);
void cmd_alarm(char* cmd, char* buf, int size);

#endif
