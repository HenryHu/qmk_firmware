#define ENABLE_CLOCK  // 320B
#define ENABLE_INFO   // 360B
#define ENABLE_TIMER  // 500B
// #define ENABLE_SPEED
#define ENABLE_NEKO   // 480B + font
#define ENABLE_UPTIME //  50B
#define ENABLE_STATUS //  80B
#define ENABLE_OLED   // 3.7K
#define ENABLE_ALTTAB //  90B
#define ENABLE_MACRO  // 250B
#define ENABLE_SERIAL // 700B + cmds
#define ENABLE_CMDMODE// 550B + cmds

#if defined(ENABLE_SERIAL) || defined(ENABLE_CMDMODE)
#define ENABLE_CMDS
#endif

#if defined(ENABLE_CMDMODE)
#define ENABLE_OLED
#endif
