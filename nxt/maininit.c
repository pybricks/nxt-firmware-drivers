
#include "interrupts.h"
#include "aic.h"
#include "at91sam7.h"
#include "uart.h"
#include "systick.h"
#include "stdio.h"
#include "flashprog.h"
#include "nxt_avr.h"
#include "twi.h"
#include "sensors.h"

#include "nxt_avr.h"
#include "nxt_lcd.h"
#include "i2c.h"
#include "nxt_motors.h"

#include "lejos_nxt.h"

#include "display.h"
#include "sound.h"
#include "bt.h"
#include "udp.h"
#include "flashprog.h"
#include "hs.h"

#include <string.h>

void shutdown(int update_mode) {
    nxt_lcd_enable(0);
    for(;;) {
        if (update_mode) {
            nxt_avr_firmware_update_mode();
        }
        else {
            nxt_avr_power_down();
        }
    }
}

void nxt_init() {
    aic_initialise();
    sp_init();
    interrupts_enable();
    systick_init();
    sound_init();
    nxt_avr_init();
    nxt_motor_init();
    i2c_init();
    bt_init();
    hs_init();
    udp_init();
    systick_wait_ms(100); // wait for Sound to stabilize
    sound_freq(500, 100, 30);
    systick_wait_ms(1000); // wait for LCD to stabilize
    display_init();

    // Reinit for app run
    sp_init();
    display_set_auto_update_period(DEFAULT_UPDATE_PERIOD);
}

void nxt_deinit(){

    while(!buttons_get()) {
        systick_wait_ms(10);
    }

    display_reset();
    nxt_motor_reset_all();
    udp_reset();
    bt_reset();
    bt_disable();
    hs_disable();
    i2c_disable_all();
    sound_reset();

    // Go to update mode if back button pressed. Else turn off.
    if (buttons_get() == BUTTON_ESCAPE) {
        shutdown(1);
    }
    else {
        shutdown(0);
    }
}
