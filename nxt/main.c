
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

#include "maininit.h"

#include <string.h>


void device_test(void) {

    unsigned long contents[64];

    display_goto_xy(0, 0);
    display_string("Test flash");

    int page = 1;
    if (flash_get_page_buffer(page) == NULL) {
        display_string("BAD PAGE");
    }
    else {
        // Read 4 x 4 bytes, increment and flash back
        flash_read_page(contents, page);
        for (int i = 0; i < 4; i++) {
            display_goto_xy(0, i+1);
            display_unsigned(contents[i], 4);
            contents[i] = contents[i] > 15 ? i : contents[i] + 1;
        }
        int res = flash_write_page(contents, page);
        display_update();

        // Display results and start page
        display_goto_xy(0, 5);
        display_int(res, 4);
        display_goto_xy(0, 6);
        display_unsigned(flash_start_page, 8);
    }

    
    systick_wait_ms(1000);

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
    nxt_init();

    // Run user app
    device_test();

    nxt_deinit();
}
