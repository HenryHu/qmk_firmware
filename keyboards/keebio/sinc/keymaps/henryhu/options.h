#define ENABLE_CLOCK  // 320B
#define ENABLE_INFO   //  74B
#define ENABLE_RGBINFO// 190B
#define ENABLE_ALARM  // 500B
#define ENABLE_SPEED
#define ENABLE_NEKO   // 480B + font
#define ENABLE_UPTIME //  50B
#define ENABLE_STATUS //  80B
#define ENABLE_OLED   // 3.7K
#define ENABLE_ALTTAB //  90B
#define ENABLE_MACRO  // 250B
#define ENABLE_SERIAL // 1.6K + cmds
#define ENABLE_CMDMODE// 550B + cmds
#define ENABLE_I2C

#if defined(ENABLE_SERIAL) || defined(ENABLE_CMDMODE)
#define ENABLE_CMDS
#endif

#if defined(ENABLE_CMDMODE)
#ifndef ENABLE_OLED
#warning CMDMODE depends on OLED
#endif
#define ENABLE_OLED
#endif

#define REUSE_CMDBUF
