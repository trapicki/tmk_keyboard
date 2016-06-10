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

// helper to define keymaps easier.
// The middle thumb keys are both taken from col 7. K37 is thus used in the 5th row between K47 and K48.
#define KEYMAP(K00, K01, K02, K03, K04, K05, K06, K07,      K08, K09, K0A, K0B, K0C, K0D, K0E, K0F, \
               K10, K11, K12, K13, K14, K15, K16, K17,      K18, K19, K1A, K1B, K1C, K1D, K1E, K1F, \
               K20, K21, K22, K23, K24, K25, K26, K27,      K28, K29, K2A, K2B, K2C, K2D, K2E, K2F, \
               K30, K31, K32, K33, K34, K35, K36,           K38 ,K39, K3A, K3B, K3C, K3D, K3E, K3F, \
               K40, K41, K42, K43, K44, K45, K46, K47, K37, K48, K49, K4A, K4B, K4C, K4D, K4E, K4F \
)                                                                     \
{                                                                     \
  { KC_##K00, KC_##K01, KC_##K02, KC_##K03, KC_##K04, KC_##K05, KC_##K06, KC_##K07, \
    KC_##K08, KC_##K09, KC_##K0A, KC_##K0B, KC_##K0C, KC_##K0D, KC_##K0E, KC_##K0F }, \
  { KC_##K10, KC_##K11, KC_##K12, KC_##K13, KC_##K14, KC_##K15, KC_##K16, KC_##K17, \
    KC_##K18, KC_##K19, KC_##K1A, KC_##K1B, KC_##K1C, KC_##K1D, KC_##K1E, KC_##K1F }, \
  { KC_##K20, KC_##K21, KC_##K22, KC_##K23, KC_##K24, KC_##K25, KC_##K26, KC_##K27, \
    KC_##K28, KC_##K29, KC_##K2A, KC_##K2B, KC_##K2C, KC_##K2D, KC_##K2E, KC_##K2F }, \
  { KC_##K30, KC_##K31, KC_##K32, KC_##K33, KC_##K34, KC_##K35, KC_##K36, KC_##K37, \
    KC_##K38, KC_##K39, KC_##K3A, KC_##K3B, KC_##K3C, KC_##K3D, KC_##K3E, KC_##K3F }, \
  { KC_##K40, KC_##K41, KC_##K42, KC_##K43, KC_##K44, KC_##K45, KC_##K46, KC_##K47, \
    KC_##K48, KC_##K49, KC_##K4A, KC_##K4B, KC_##K4C, KC_##K4D, KC_##K4E, KC_##K4F }, \
}


static const uint8_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Layer 0: Traditional QWERTY with thumb keys
  KEYMAP(ESC , GRV , 1   , 2   , 3   , 4   , 5   ,   NO      , 6   , 7   , 8   , 9   , 0   , MINS, EQL , BSPC, \
         TAB , TAB , Q   , W   , E   , R   , T   ,   NO      , Y   , U   , I   , O   , P   , LBRC, RBRC, BSLS, \
         CAPS, CAPS, A   , S   , D   , F   , G   ,   NO      , H   , J   , K   , L   , SCLN, QUOT, ENT , ENT , \
         LSFT, LSFT, Z   , X   , C   , V   , B               , N   , M   , COMM, DOT , SLSH, RSFT, RSFT, RSFT, \
         LCTL, LGUI, LALT, NO  , NO  , BSPC, DEL , FN0 , FN1 , ENT , SPC , NO  , NO  , RALT, RGUI, APP , RCTL \
         ),

// Layer 1: F-Keys, NumPad, Cursor etc.
  KEYMAP(INS , TRNS, F1  , F2  , F3  , F4  , F5  , TRNS,       F6  , F7  , F8  , F9  , F10 , F11 , F12 , DEL , \
         TRNS, TRNS, TRNS, PGUP, UP  , PGDN, TRNS, TRNS,       TRNS, KP_7, KP_8, KP_9, PMNS, PSLS, TRNS, TRNS, \
         TRNS, TRNS, TRNS, LEFT, DOWN, RGHT, TRNS, TRNS,       TRNS, KP_4, KP_5, KP_6, PPLS, PAST, TRNS, TRNS, \
         TRNS, TRNS, TRNS, HOME, TRNS, END , TRNS,             TRNS, KP_1, KP_2, KP_3, PENT, PSCR, SLCK, PAUS, \
         TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, TRNS, KP_0, KP_0, PCMM, PENT, TRNS, TRNS, TRNS, TRNS \
         )
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

