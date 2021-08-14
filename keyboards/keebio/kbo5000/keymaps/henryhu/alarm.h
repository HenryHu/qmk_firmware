#pragma once

#include "options.h"

#include <stdint.h>

#ifdef ENABLE_TIMER

void setTimer(int timeout);
bool timerArmed(void);
uint32_t timerRemaining(void);
uint32_t alarmTime(void);
bool timerTriggered(void);
void timerKey(void);

#endif
