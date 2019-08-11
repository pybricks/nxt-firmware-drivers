#ifndef __AT91_TWI_H__
#define __AT91_TWI_H__

#include <stdint.h>

/* Main user interface */
int twi_init(void);

void twi_start_write(uint32_t dev_addr, const uint8_t *data, uint32_t nBytes);
void twi_start_read(uint32_t dev_addr, uint8_t *data, uint32_t nBytes);
int twi_status(void);
void twi_reset(void);

#endif
