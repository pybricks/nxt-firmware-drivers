#ifndef  __SYSTICK_H__
#define __SYSTICK_H__

#include <stdint.h>

typedef void(*systick_hook_t)(void);

void systick_set_hook(systick_hook_t hook);

void systick_init(void);

uint32_t systick_get_ms(void);

uint32_t systick_get_us(void);

uint64_t systick_get_ns(void);

void systick_wait_ms(uint32_t ms);

void systick_wait_ns(uint32_t n);

void systick_test(void);

void systick_suspend(void);

void systick_resume(void);

#endif
