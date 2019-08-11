#ifndef __AT91_UART_H__
#  define __AT91_UART_H__

#include <stdint.h>

/* Main user interface */
int uart_init(uint32_t u, uint32_t baudRate, uint32_t dataBits, uint32_t stopBits, char parity);
void uart_close(uint32_t u);
int uart_holding(uint32_t u);
int uart_get_byte(uint32_t u, uint8_t *b);
int uart_put_byte(uint32_t u, uint8_t b);
void uart_put_str(uint32_t u, const uint8_t *str);
int uart_clear_rx(uint32_t u);
int uart_clear_tx(uint32_t u);
int uart_set_break(uint32_t u);
int uart_clear_break(uint32_t u);
#endif
