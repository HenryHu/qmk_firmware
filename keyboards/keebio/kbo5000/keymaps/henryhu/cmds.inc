    DEFINE_COMMAND(ver)
#ifdef ENABLE_UPTIME
    DEFINE_COMMAND(uptime)
#endif
#ifdef ENABLE_INFO
    DEFINE_COMMAND(info)
    DEFINE_COMMAND(prod)
#endif
#ifdef ENABLE_RGBINFO
    DEFINE_COMMAND(rgb)
#endif
    DEFINE_COMMAND(help)
#ifdef ENABLE_ALARM
    DEFINE_COMMAND(alarm)
#endif
#ifdef ENABLE_SPEED
    DEFINE_COMMAND(speed)
#endif
#ifdef ENABLE_STATUS
    DEFINE_COMMAND(status)
#endif
#ifdef ENABLE_CLOCK
    DEFINE_COMMAND(time)
#endif
#ifdef ENABLE_CMDMODE
    DEFINE_COMMAND(exit)
#endif
#ifdef ENABLE_I2C
    DEFINE_COMMAND(scan)
    DEFINE_COMMAND(addr)
    DEFINE_COMMAND(send)
    DEFINE_COMMAND(recv)
#endif
