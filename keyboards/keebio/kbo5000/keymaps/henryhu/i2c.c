#include "i2c_master.h"
#include "common.h"
#include "i2c.h"

#ifdef ENABLE_I2C

void cmd_scan(char* cmd, char* buf, int size) {
    strcat(buf, "Dev: ");
    for (uint8_t addr = 0; addr < 128; addr += 2) {
        i2c_status_t ret = i2c_start(addr, 10);
        if (ret == I2C_STATUS_SUCCESS) {
            i2c_stop();
            strcat_P(buf, PSPACE);
            appendValue(buf, addr);
        }
    }
}
#endif
