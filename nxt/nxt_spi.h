#ifndef __NXT_SPI_H__
#define __NXT_SPI_H__

#include <stdint.h>

/*
 * Note that this is not a normal SPI interface, 
 * it is a bodged version as used by the NXT's 
 * display.
 *
 * The display does not use MISO because you can
 * only write to it in serial mode.
 *
 * Instead, the MISO pin is not used by the SPI
 * and is instead driven as a PIO pin for controlling CD.
 */

void nxt_spi_init(void);
void nxt_spi_write(uint32_t CD, const uint8_t *data, uint32_t nBytes);
void nxt_spi_set_display(const uint8_t *disp);
void nxt_spi_refresh(void);

// Time required to perform a complete refresh of the screen.
#define DISPLAY_UPDATE_TIME 6
#endif
