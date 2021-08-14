#include QMK_KEYBOARD_H
#include "virtser.h"

#include "serial.h"
#include "cmds.h"

#ifdef ENABLE_SERIAL
char serialBuffer[64];
int serialPtr = 0;

void serial_send(const char* str) {
    int len = strlen(str);
    for (int i = 0; i < len; ++i) {
        if (str[i] == '\n') virtser_send('\r');
        virtser_send(str[i]);
    }
}

void process_serial_command(void) {
    char buf[256];
    buf[0] = '>';
    buf[1] = ' ';
    buf[2] = 0;
    buf[sizeof(buf) - 1] = 0;
    handle_command(serialBuffer, buf + 2, sizeof(buf) - 2);
    strcat(buf, "\r\n");
    serial_send(buf);
}

void virtser_recv(uint8_t in) {
    switch (in) {
        case 10:
        case 13:
            if (serialPtr == 0) break;
            serial_send("\r\n");
            if (serialPtr >= sizeof(serialBuffer)) serialPtr = sizeof(serialBuffer) - 1;
            serialBuffer[serialPtr] = 0;
            process_serial_command();
            serialPtr = 0;
            break;
        case 8:
        case 127:
            if (serialPtr > 0) --serialPtr;
            virtser_send(in);
            break;
        default:
            if (serialPtr >= sizeof(serialBuffer) - 1) return;
            serialBuffer[serialPtr++] = in;
            virtser_send(in);
    }
}
#endif
