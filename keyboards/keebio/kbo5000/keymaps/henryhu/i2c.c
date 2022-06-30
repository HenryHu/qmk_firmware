#include "i2c_master.h"
#include "common.h"
#include "i2c.h"

#ifdef ENABLE_I2C

uint8_t i2c_addr = 0;

#define I2C_TIMEOUT_MS 10

void cmd_scan(char* cmd, char* buf, int size) {
    strcat(buf, "Dev: ");
    for (uint8_t addr = 0; addr < 128; addr += 2) {
        i2c_status_t ret = i2c_start(addr, I2C_TIMEOUT_MS);
        if (ret == I2C_STATUS_SUCCESS) {
            i2c_stop();
            strcat_P(buf, PSPACE);
            appendValue(buf, addr);
        }
    }
}

void cmd_addr(char* cmd, char* buf, int size) {
    i2c_addr = atoi(cmd + 5);
}

void cmd_send(char* cmd, char* buf, int size) {
    uint8_t cur = 5;
    uint8_t data[16];
    uint8_t argi = 0;
    bool last = false;
    for (uint8_t argp = 5; ; ++argp) {
        if (cmd[argp] == ' ' || cmd[argp] == 0) {
            if (cmd[argp] == 0) last = true;
            cmd[argp] = 0;
            uint8_t arg = atoi(&cmd[cur]);
            data[argi++] = arg;
            cur = argp;
            if (last) break;
        }
    }

    i2c_status_t ret = i2c_start(i2c_addr, I2C_TIMEOUT_MS);
    if (ret != I2C_STATUS_SUCCESS) {
        strcat_P(buf, PSTR("!start"));
        return;
    }
    ret = i2c_transmit(i2c_addr, data, argi, I2C_TIMEOUT_MS);
    if (ret != I2C_STATUS_SUCCESS) {
        strcat_P(buf, PSTR("!send"));
        return;
    }
    i2c_stop();
    strcat_P(buf, PSTR("OK "));
    appendValue(buf, argi);
}
#endif
