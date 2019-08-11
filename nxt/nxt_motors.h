#ifndef __NXT_MOTORS_H__
#define __NXT_MOTORS_H__

#define NXT_N_MOTORS 3

#include <stdint.h>

int nxt_motor_get_count(uint32_t n);
void nxt_motor_set_count(uint32_t n, int count);

void nxt_motor_set_speed(uint32_t n, int speed_percent, int brake);

void nxt_motor_init(void);

// ISR points used by motor processing
void nxt_motor_1kHz_process(void);

void nxt_motor_reset_all(void);

#endif
