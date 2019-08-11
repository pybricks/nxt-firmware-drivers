#ifndef HS_H_
#define HS_H_

#include <stdint.h>

#define   HS_RX_PIN  AT91C_PIO_PA5
#define   HS_TX_PIN  AT91C_PIO_PA6
#define   HS_RTS_PIN AT91C_PIO_PA7

void hs_init(void);
int hs_enable(int baud, int buf_sz);
void hs_disable(void);
uint32_t hs_write(uint8_t *buf, uint32_t off, uint32_t len);
uint32_t hs_read(uint8_t * buf, uint32_t off, uint32_t len);
uint32_t hs_pending(void);

int hs_send(uint8_t address, uint8_t control, uint8_t *data, int offset, int len, uint16_t *CRCTab);
int hs_recv(uint8_t *data, int len, uint16_t *CRCTab, int reset);

#endif /*HS_H_*/
