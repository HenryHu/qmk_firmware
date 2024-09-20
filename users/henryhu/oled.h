#pragma once

#include "options.h"

#ifdef ENABLE_OLED
extern char infoLine[22];
extern const uint8_t infoLineLen;
void shutdown_oled(void);
#endif

void setInfoLine(const char* buf);
void setInfoLine_P(PGM_P buf);
void appendInfoLine(uint16_t value);
void appendInfoLine_P(PGM_P buf);
