#include QMK_KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#include "anim.h"

#ifdef ENABLE_NEKO
extern uint8_t* oled_cursor;
extern uint8_t oled_buffer[];
extern uint8_t oled_rotation_width;
void oled_set_cursor_raw(int x, int y) {
    oled_cursor = &oled_buffer[y * oled_rotation_width / 8 + x];
}

const int8_t frame_offset[] = {
    0, 2, -2, -2, -1, 2, 0, 2
};

void clear_canvas(uint8_t start_x, uint8_t x_len,
                  uint8_t start_row, uint8_t row_count) {
    for (uint8_t row = start_row; row < start_row + row_count; ++row) {
        for (uint8_t x = start_x; x < start_x + x_len; ++x) {
            oled_write_raw_byte(0, row * oled_rotation_width + x);
        }
    }
}

uint8_t frame = 0, last_frame = 255;
uint32_t last_frame_time = 0;
uint16_t frame_time = 500;

void render_animation(uint8_t col, uint8_t row) {
    if (frame == last_frame) return;
    last_frame = frame;
    clear_canvas(col * OLED_FONT_WIDTH - 2, 4 * OLED_FONT_WIDTH + 4, row, 3);
    const uint8_t start = 0x80;
    const uint8_t lineLen = 0x20;
    for (uint8_t x = 0; x < 3; ++x) {
        oled_set_cursor_raw(col * OLED_FONT_WIDTH + frame_offset[frame], (row + x) * OLED_FONT_HEIGHT);
        for (uint8_t y = 0; y < 4; ++y) {
            oled_write_char(start + x * lineLen + y + frame * 4, false);
        }
    }
}

/*
enum NekoState {
    IDLE = 0,
    FLAP = 1,
    SCRATCH = 2,
    SLEEP = 3,
};

enum NekoState neko_state = IDLE;
*/

void neko_sleep(void) {
    frame = frame == 0 ? 6 : frame == 6 ? 0 : frame == 5 ? 0 : 5;
    frame_time = 500;
}

void neko_scratch(void) {
    frame = frame == 2 ? 3 : 2;
    frame_time = 100;
}

void neko_flap(void) {
    frame = frame == 1 ? 7 : 1;
    frame_time = 200;
}

void neko_idle(void) {
    frame = 1;
    frame_time = 1000;
}

void neko_awake(void) {
    frame = 4;
    frame_time = 100;
}

void pick_frame(uint16_t idle_time) {
    uint32_t now = timer_read32();
    if (now < last_frame_time + frame_time && idle_time > 100) {
        return;
    }
    last_frame_time = now;
    if (idle_time > 30000) {
        neko_sleep();
        return;
    } else if (idle_time > 15000) {
        neko_scratch();
    } else if (idle_time > 5000) {
        neko_flap();
    } else if (idle_time > 100 && idle_time < 5000) {
        neko_idle();
    } else {
        neko_awake();
    }
}
#endif

