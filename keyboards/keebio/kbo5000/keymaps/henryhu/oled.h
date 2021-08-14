#pragma once

#include "options.h"

#ifdef ENABLE_OLED
extern char infoLine[22];
void setInfoLine(const char* buf);
#endif
