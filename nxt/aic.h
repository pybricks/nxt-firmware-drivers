/* Driver for the AT91SAM7's Advanced Interrupt Controller (AIC).
 *
 * The AIC is responsible for queuing interrupts from other
 * peripherals on the board. It then hands them one by one to the ARM
 * CPU core for handling, according to each peripheral's configured
 * priority.
 */

#ifndef __AIC_H__
#define __AIC_H__

#include <stdint.h>

void aic_initialise(void);
void aic_set_vector(uint32_t vector, uint32_t mode, void (*isr)(void));
void aic_mask_on(uint32_t vector);
void aic_mask_off(uint32_t vector);
void aic_clear(uint32_t mask);

/* Priority levels for interrupt lines. */
#define AIC_INT_LEVEL_LOWEST 1
#define AIC_INT_LEVEL_LOW    2
#define AIC_INT_LEVEL_NORMAL 4
#define AIC_INT_LEVEL_ABOVE_NORMAL 5

#endif
