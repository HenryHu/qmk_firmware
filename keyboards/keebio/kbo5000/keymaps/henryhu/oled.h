#pragma once

#include "options.h"

#ifdef ENABLE_OLED
extern char infoLine[22];
void shutdown_oled(void);
#endif

void setInfoLine(const char* buf);
