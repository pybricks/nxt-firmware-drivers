#include "nxt_lcd.h"
#include "nxt_spi.h"
#include "systick.h"
#include "string.h"

static uint8_t *display = (uint8_t *)0;

void
nxt_lcd_command(uint8_t cmd)
{
  uint8_t tmp = cmd;

  nxt_spi_write(0, &tmp, 1);
}

void
nxt_lcd_set_col(uint32_t coladdr)
{
  nxt_lcd_command(0x00 | (coladdr & 0xF));
  nxt_lcd_command(0x10 | ((coladdr >> 4) & 0xF));
}

void
nxt_lcd_set_multiplex_rate(uint32_t mr)
{
  nxt_lcd_command(0x20 | (mr & 3));
}

void
nxt_lcd_set_temp_comp(uint32_t tc)
{
  nxt_lcd_command(0x24 | (tc & 3));
}

void
nxt_lcd_set_panel_loading(uint32_t hi)
{
  nxt_lcd_command(0x28 | ((hi) ? 1 : 0));
}

void
nxt_lcd_set_pump_control(uint32_t pc)
{
  nxt_lcd_command(0x2c | (pc & 3));
}

void
nxt_lcd_set_scroll_line(uint32_t sl)
{
  nxt_lcd_command(0x40 | (sl & 0x3f));
}

void
nxt_lcd_set_page_address(uint32_t pa)
{
  nxt_lcd_command(0xB0 | (pa & 0xf));
}

void
nxt_lcd_set_pot(uint32_t pot)
{
  nxt_lcd_command(0x81);
  nxt_lcd_command(pot & 0xff);
}

void
nxt_lcd_set_ram_address_control(uint32_t ac)
{
  nxt_lcd_command(0x88 | (ac & 7));
}

void
nxt_lcd_set_frame_rate(uint32_t fr)
{
  nxt_lcd_command(0xA0 | (fr & 1));
}

void
nxt_lcd_set_all_pixels_on(uint32_t on)
{
  nxt_lcd_command(0xA4 | ((on) ? 1 : 0));
}

void
nxt_lcd_inverse_display(uint32_t on)
{
  nxt_lcd_command(0xA6 | ((on) ? 1 : 0));
}

void
nxt_lcd_enable(uint32_t on)
{
  nxt_lcd_command(0xAE | ((on) ? 1 : 0));
}

void
nxt_lcd_set_map_control(uint32_t map_control)
{
  nxt_lcd_command(0xC0 | ((map_control & 3) << 1));
}

void
nxt_lcd_reset(void)
{
  nxt_lcd_command(0xE2);
}

void
nxt_lcd_set_bias_ratio(uint32_t ratio)
{
  nxt_lcd_command(0xE8 | (ratio & 3));
}

void
nxt_lcd_set_cursor_update(uint32_t on)
{
  nxt_lcd_command(0xEE | ((on) ? 1 : 0));
}

void
nxt_lcd_force_update()
{
  // Update the screen the slow way. Works with interrupts disabled
  int i;
  uint8_t *disp = display;

  for (i = 0; i < NXT_LCD_DEPTH; i++) {
    nxt_lcd_set_col(0);
    nxt_lcd_set_page_address(i);

    nxt_spi_write(1, disp, NXT_LCD_WIDTH);
    disp += NXT_LCD_WIDTH;
  }
}


void
nxt_lcd_update()
{
#define DMA_REFRESH
#ifdef DMA_REFRESH
  nxt_spi_refresh();
#else
  nxt_lcd_force_update();
#endif
}

void
nxt_lcd_power_up(void)
{

  systick_wait_ms(20);
  nxt_lcd_reset();
  systick_wait_ms(20);
  //nxt_lcd_set_multiplex_rate(3);	// 1/65
  //nxt_lcd_set_bias_ratio(3);	// 1/9
  nxt_lcd_set_pot(NXT_DEFAULT_CONTRAST);	// ?? 9V??

  //nxt_lcd_set_ram_address_control(1); // auto wrap
  nxt_lcd_set_map_control(0x02); // mirror in y

  nxt_spi_set_display(display);
}

void
nxt_lcd_init(const uint8_t *disp)
{
  display = (uint8_t *)disp;
  nxt_spi_init();

  nxt_lcd_power_up();

}
