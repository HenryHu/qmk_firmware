#pragma once

#include "options.h"

#include <stdint.h>

#include "cmds.h"

#ifdef ENABLE_CMDMODE
extern bool command_mode;
extern char cmdRet[64];

bool command_mode_key(uint8_t keycode, keyrecord_t *record);
void cmd_exit(char* cmd, char* buf, int size);
#endif
