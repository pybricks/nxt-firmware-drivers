#ifndef __NXT_LCD_H__
#define __NXT_LCD_H__

#include <stdint.h>

#define NXT_LCD_WIDTH 100
#define NXT_LCD_DEPTH 8
#define NXT_DEFAULT_CONTRAST 0x60

void nxt_lcd_init(const uint8_t *disp);
void nxt_lcd_power_up(void);
void nxt_lcd_power_down(void);
void nxt_lcd_update();
void nxt_lcd_force_update();
void nxt_lcd_set_pot(uint32_t val);
void nxt_lcd_enable(uint32_t on);
#endif
