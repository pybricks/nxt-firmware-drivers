#include "display.h"
#include "at91sam7.h"
#include "nxt_spi.h"
#include "init.h"


void
data_abort_C(void *pc)
{
  // Need to reset the link to the display to see any output
  nxt_spi_init();
  display_reset();
  display_goto_xy(0, 0);
  display_string("Data abort");
  display_goto_xy(0, 1);
  display_string("PC   ");
  display_hex((uintptr_t)pc, 8);
  display_goto_xy(0, 2);
  display_string("AASR ");
  display_hex(*AT91C_MC_AASR, 8);
  display_goto_xy(0, 3);
  display_string("ASR  ");
  display_hex(*AT91C_MC_ASR, 8);

  display_force_update();
}
