/*
  Copyright 2012,2013 Jun Wako <wakojun@gmail.com>

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
#include <stdint.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include "keycode.h"
#include "action.h"
#include "action_macro.h"
#include "report.h"
#include "host.h"
#include "print.h"
#include "debug.h"
#include "keymap.h"


static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    // layer 0: WASD
    {{KC_A,    KC_W},
     {KC_S,    KC_D}},
    // layer 1: cursor keys
    {{KC_LEFT, KC_UP},
     {KC_DOWN, KC_RIGHT}},
};

/*
 * Fn action definition
 */
static const uint16_t PROGMEM fn_actions[] = {
};

void hook_keymap_event(keyevent_t key) {
    // check pressed, because we get an event for both pressed an release
    if (matrix_key_count() == 4 && key.pressed) {
        // switch layer to cursor keys
        default_layer_xor((uint32_t)1<<1);
        xprintf("%s: switching layer. New State: ", __FUNCTION__);
        default_layer_debug();
        xprintf("\n");
    }
}


#define KEYMAPS_SIZE    (sizeof(keymaps) / sizeof(keymaps[0]))
#define FN_ACTIONS_SIZE (sizeof(fn_actions) / sizeof(fn_actions[0]))

/* translates key to keycode */
uint8_t keymap_key_to_keycode(uint8_t layer, keypos_t key)
{
    uint8_t keycode;

    if (layer >= KEYMAPS_SIZE) {
        // fall back to layer 0
        keycode = pgm_read_byte(&keymaps[0][(key.row)][(key.col)]);
        xprintf("%s: layer %d >= KEYMAPS_SIZE %d, falling back to layer 0!\n", __FUNCTION__, layer, KEYMAPS_SIZE);
    }
    else {
        keycode = pgm_read_byte(&keymaps[(layer)][(key.row)][(key.col)]);
    }
    xprintf("%s: layer %02d key 0X%X/0X%X => 0X%X\n", __FUNCTION__, layer, key.row, key.col, keycode);

    return keycode;
}

/* translates Fn keycode to action */
action_t keymap_fn_to_action(uint8_t keycode)
{
    xprintf("keymap_fn_to_action: keycode x%X\n", keycode);
    action_t action;
    if (FN_INDEX(keycode) < FN_ACTIONS_SIZE) {
        action.code = pgm_read_word(&fn_actions[FN_INDEX(keycode)]);
    } else {
        action.code = ACTION_NO;
    }
    return action;
}

