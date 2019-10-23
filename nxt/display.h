#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>

void display_init(void);

void display_reset(void);

void display_update(void);

void display_force_update(void);

void display_clear(uint32_t updateToo);

void display_goto_xy(int x, int y);

void display_char(int c);

void display_string(const char *str);

void display_int(int val, uint32_t places);
void display_hex(uint32_t val, uint32_t places);

void display_unsigned(uint32_t val, uint32_t places);

void display_bitmap_copy(const uint8_t *data, uint32_t width, uint32_t depth, uint32_t x, uint32_t y);
void display_bitblt(uint8_t *src, int sw, int sh, int sx, int sy, uint8_t *dst, int dw, int dh, int dx, int dy, int w, int h, int rop);
void display_test(void);

uint8_t *display_get_buffer(void);

#define DEFAULT_UPDATE_PERIOD 250
int display_set_auto_update_period(int);

int display_get_update_complete_time();
extern uint32_t display_update_time;

#endif
