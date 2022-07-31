#pragma once

#include "options.h"

#ifdef ENABLE_CLOCK
void get_time(char* buf);
void cmd_time(char* cmd, char* buf, int size);
#endif
