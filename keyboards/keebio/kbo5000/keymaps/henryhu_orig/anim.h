#pragma once

#include "options.h"

#ifdef ENABLE_NEKO
void pick_frame(uint16_t idle_time);
void render_animation(uint8_t col, uint8_t row);
#endif
