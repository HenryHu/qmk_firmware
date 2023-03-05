#include QMK_KEYBOARD_H
#include "serport.h"

#include <string.h>

#ifdef ENABLE_SERIAL
#include "virtser.h"

#include "cmds.h"
#include "cmdmode.h"

#if defined(REUSE_CMDBUF) && defined(ENABLE_CMDMODE)
#define serialBuffer cmdBuf
#define serialPtr cmdPtr
#else
#ifdef REUSE_CMDBUF
#warning REUSE_CMDBUF depends on ENABLE_CMDMODE
#endif
char serialBuffer[32];
uint8_t serialPtr = 0;
#endif

#ifndef ENABLE_CMDMODE
char cmdRet[32];
#endif

void serial_send(const char* str) {
    while (*str != 0) {
        if (*str == '\n') virtser_send('\r');
        virtser_send(*str);
        ++str;
    }
}

void process_serial_command(void) {
    cmdRet[0] = '>';
    cmdRet[1] = ' ';
    cmdRet[2] = 0;
    handle_command(serialBuffer, cmdRet + 2, sizeof(cmdRet) - 2);
    strcat_P(cmdRet, PSTR("\n"));
    serial_send(cmdRet);
}

void virtser_recv(uint8_t in) {
    switch (in) {
        case 10:
        case 13:
            if (serialPtr == 0) break;
            serial_send("\n");
            if (serialPtr >= sizeof(serialBuffer)) serialPtr = sizeof(serialBuffer) - 1;
            serialBuffer[serialPtr] = 0;
            process_serial_command();
            serialPtr = 0;
            return;
        case 8:
        case 127:
            if (serialPtr > 0) --serialPtr;
            break;
        default:
            if (serialPtr >= sizeof(serialBuffer) - 1) return;
            serialBuffer[serialPtr++] = in;
    }
    virtser_send(in);
}
#endif
