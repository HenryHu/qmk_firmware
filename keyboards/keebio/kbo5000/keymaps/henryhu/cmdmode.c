#include QMK_KEYBOARD_H

#include <stdbool.h>

#include "cmdmode.h"
#include "oled.h"
#include "common.h"

#ifdef ENABLE_CMDMODE
bool command_mode = false;
char cmdBuf[64];
int cmdPtr = 0;
char cmdRet[256];
uint8_t cmdRetPtr = 0;

char get_char_for_key(uint16_t keycode) {
    keycode = keycode & 0xFF;
    if (keycode >= KC_A && keycode <= KC_Z) {
        return keycode - KC_A + 'a';
    }
    if (keycode >= KC_1 && keycode <= KC_9) {
        return keycode - KC_1 + '1';
    }
    if (keycode == KC_0) return '0';
    if (keycode == KC_SPACE) return ' ';
    if (keycode == KC_BSPACE) return '\b';
    if (keycode == KC_ENTER) return '\n';
    return 0;
}

int find_next_end(void) {
    int end = cmdRetPtr;
    while (cmdRet[end] != 0 && cmdRet[end] != '\n' && end < cmdRetPtr + sizeof(infoLine) - 1) ++end;
    return end;
}

void print_until_newline(void) {
    int end = find_next_end();
    char ch = cmdRet[end];
    cmdRet[end] = 0;
    setInfoLine(&cmdRet[cmdRetPtr]);
    cmdRet[end] = ch;
}

void command_process(void) {
    cmdRet[0] = 0;
    handle_command(cmdBuf, cmdRet, sizeof(cmdRet));
    cmdRetPtr = 0;
    print_until_newline();
}

bool command_mode_key(uint16_t keycode, keyrecord_t *record) {
    char ch = get_char_for_key(keycode);
    if (ch == '\n') {
        if (cmdPtr > 0) {
            command_process();
            cmdPtr = 0;
            memset(cmdBuf, 0, sizeof(cmdBuf));
        } else {
            int end = find_next_end();
            if (cmdRet[end] == 0) {
                cmdRetPtr = 0;
            } else if (cmdRet[end] == '\n') {
                cmdRetPtr = end + 1;
            } else {
                cmdRetPtr = end;
            }
            print_until_newline();
        }
        return false;
    }
    if (ch == '\b') {
        if (cmdPtr > 0) --cmdPtr;
        cmdBuf[cmdPtr] = 0;
    } else if (ch != 0) {
        if (cmdPtr < sizeof(cmdBuf)) cmdBuf[cmdPtr++] = ch;
    }
    infoLine[0] = '?';
    strlcpy(infoLine + 1, cmdBuf, sizeof(infoLine) - 2);
    strcat(infoLine, "_");
    return ch == 0;
}

void cmd_exit(char* cmd, char* buf, int size) {
    command_mode = false;
}
#endif
