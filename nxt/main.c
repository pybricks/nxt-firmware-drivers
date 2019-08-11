
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
#include "constants.h"

#include <string.h>

void shutdown(int update_mode) {
    nxt_lcd_enable(false);
    for(;;) {
        if (update_mode) {
            nxt_avr_firmware_update_mode();
        }
        else {
            nxt_avr_power_down();
        }
    }
}


/***************************************************************************
 * int nxt_main *--------------------------------------------------------------------------
 ***************************************************************************/

void device_test(void);

int nxt_main() {
    sp_init();
    display_set_auto_update_period(DEFAULT_UPDATE_PERIOD);

    // Run user app
    device_test();

    display_reset();
    nxt_motor_reset_all();
    udp_reset();
    bt_reset();
    bt_disable();
    hs_disable();
    i2c_disable_all();
    sound_reset();
    return 0;
}

void device_test(void) {
    int iterator = 0;
    uint32_t buttons;
    uint32_t motor_mode = 0;

    while (!buttons_get()) {
        display_clear(0);

        if ((iterator & 15) == 0) {
            motor_mode = (iterator >> 4) & 3;
            switch (motor_mode) {

            case 0:
            nxt_motor_set_speed(2, -100, 0);
            break;
            case 1:
            nxt_motor_set_speed(2, 100, 0);
            break;
            case 2:
            nxt_motor_set_speed(2, 0, 0);
            break;
            case 3:
            nxt_motor_set_speed(2, 0, 1);
            break;
            }
        }
        iterator++;

        if ((iterator %10) < 5) {
            buttons = buttons_get();

            display_goto_xy(iterator & 7, 0);
            display_string("LEJOS NXT");

            display_goto_xy(0, 1);
            display_string("TIME ");
            display_unsigned(systick_get_ms(), 0);

            display_goto_xy(0, 2);
            display_string("BATTERY ");
            display_unsigned(battery_voltage(), 0);

            display_goto_xy(0, 3);
            display_string("BUTTONS ");
            if (buttons & 1)
            display_string("0 ");
            if (buttons & 2)
            display_string("1 ");
            if (buttons & 4)
            display_string("2 ");
            if (buttons & 8)
            display_string("3 ");

            display_goto_xy(0, 4);
            display_string("ADC ");
            display_unsigned(sensor_adc(0), 5);
            display_unsigned(sensor_adc(1), 5);
            display_goto_xy(0, 5);
            display_string("    ");
            display_unsigned(sensor_adc(2), 5);
            display_unsigned(sensor_adc(3), 5);

            display_update();
            systick_wait_ms(500);

        }
        else {
            display_goto_xy(iterator & 7, 0);
            display_string("LEJOS NXT");

            display_goto_xy(0, 1);
            display_string("TIME ");
            display_unsigned(systick_get_ms(), 0);
            display_goto_xy(0, 2);
            switch (motor_mode) {
                case 0:
                display_string("MOTORS REV");
                break;
                case 1:
                display_string("MOTORS FWD");
                break;
                case 2:
                display_string("MOTORS COAST");
                break;
                case 3:
                display_string("MOTORS BRAKE");
                break;
            }

            display_goto_xy(1, 4);
            display_int(nxt_motor_get_count(0), 0);
            display_goto_xy(1, 5);
            display_int(nxt_motor_get_count(1), 0);
            display_goto_xy(1, 6);
            display_int(nxt_motor_get_count(2), 0);

            display_update();
            systick_wait_ms(500);
        }
    }
}

void main(void)
{
    /* When we get here:
    * PLL and flash have been initialised and
    * interrupts are off, but the AIC has not been initialised.
    */
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
    
    // Repeatedly run "app"
    nxt_main();

    // Go to update mode if back button pressed. Else turn off.
    if (buttons_get() == BUTTON_ESCAPE) {
        shutdown(1);
    }
    else {
        shutdown(0);
    }
}
