/*
Copyright 2012 Jun Wako <wakojun@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <avr/io.h>
#include "stdint.h"
#include "led.h"
#include "print.h"

#define BIT_LED 6
#define DDR_LED DDRD
#define PORT_LED PORTD

void led_set(uint8_t usb_led)
{
    // set direction to output
    DDR_LED  |= (1<<BIT_LED);
    if (usb_led & (1<<USB_LED_NUM_LOCK)) {
        print("NumLock on\n");
        // output high
        PORT_LED |= (1<<BIT_LED);
    } else {
        print("NumLock off\n");
        // ouput low
        PORT_LED &= ~(1<<BIT_LED);
    }
}
