#pragma once

bool process_key_down(uint16_t keycode, keyrecord_t *record);
bool process_key_up(uint16_t keycode, keyrecord_t *record);
uint32_t idleTime(void);
void updateKeyDownTime(void);
