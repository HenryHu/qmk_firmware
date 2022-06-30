#include "options.h"

#ifdef ENABLE_I2C
void cmd_scan(char* cmd, char* buf, int size);
void cmd_addr(char* cmd, char* buf, int size);
void cmd_send(char* cmd, char* buf, int size);
void cmd_recv(char* cmd, char* buf, int size);
#endif
