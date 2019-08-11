#ifndef __NXT_AVR_H__
#  define __NXT_AVR_H__

#include <stdint.h>

/* Main user interface */
void nxt_avr_init(void);

void nxt_avr_1kHz_update(void);

void nxt_avr_set_motor(uint32_t n, int power_percent, int brake);

void nxt_avr_power_down(void);

void nxt_avr_firmware_update_mode(void);

void nxt_avr_test_loop(void);

void nxt_avr_update(void);

uint32_t buttons_get(void);

int32_t buttons_check_event(int32_t filter);

uint32_t battery_voltage(void);

uint32_t sensor_adc(uint32_t n);

void nxt_avr_set_input_power(uint32_t n, uint32_t power_type);

void nxt_avr_set_key_click(uint32_t freq, uint32_t len, uint32_t vol);

#define BUTTON_ENTER 0x1
#define BUTTON_ESCAPE 0x8
#define BUTTON_LEFT 0x2
#define BUTTON_RIGHT 0x4

#endif
