# VIA_ENABLE = yes
MOUSEKEY_ENABLE = yes
# POINTING_DEVICE_ENABLE = yes
CONSOLE_ENABLE = no
VIRTSER_ENABLE = yes
BACKLIGHT_ENABLE = no
MOUSE_SHARED_EP = yes
KEYBOARD_SHARED_EP = yes
# KEY_LOCK_ENABLE = yes
# UNICODE_ENABLE = yes
NKRO_ENABLE = yes
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no
BOOTMAGIC_ENABLE = no
OLED_ENABLE = yes
WPM_ENABLE = no

SRC += oled.c alarm.c status.c cmdmode.c clock.c anim.c cmds.c serial.c encoder.c key.c alttab.c common.c system.c
