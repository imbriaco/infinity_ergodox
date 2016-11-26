/*
Copyright 2016 Fred Sundvik <fsundvik@gmail.com>
Jun Wako <wakojun@gmail.com>

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

#include "keymap_common.h"

const uint8_t keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    KEYMAP(  // layer 0 : default
        // left hand
        EQL, 1,   2,   3,   4,   5,   ESC,
        TAB, Q,   W,   E,   R,   T,   FN4,
        FN3, A,   S,   D,   F,   G,
        LSFT,Z,   X,   C,   V,   B,   LGUI,
        FN0, GRV, BSLS,LEFT,RGHT,
                                      FN6, FN7,
                                           HOME,
                                 BSPC,DEL, END,
        // right hand
             FN1, 6,   7,   8,   9,   0,   MINS,
             FN5, Y,   U,   I,   O,   P,   BSLS,
                  H,   J,   K,   L,   SCLN,QUOT,
             FN0, N,   M,   COMM,DOT, SLSH,RSFT,
                       UP,  DOWN,LBRC,RBRC,FN2,
        RALT,RCTL,
        PGUP,
        PGDN,ENT, SPC
    ),

    KEYMAP(  // layer 1 : function and symbol keys
        // left hand
        TRNS,F1,  F2,  F3,  F4,  F5,  F11,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,FN3,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,
                                      TRNS,TRNS,
                                           TRNS,
                                 TRNS,TRNS,TRNS,
        // right hand
             F12, F6,  F7,  F8,  F9,  F10, TRNS,
             TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
                  TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
             TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
                       TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,
        TRNS,
        TRNS,TRNS,TRNS
    ),

    KEYMAP(  // layer 2 : numpad + keyboard functions
        // left hand
        BTLD,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS, FN3,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,TRNS,
        TRNS,TRNS,TRNS,TRNS,TRNS,
                                      TRNS,TRNS,
                                           TRNS,
                                 TRNS,TRNS,TRNS,
        // right hand
             TRNS,NLCK,PSLS,PAST,PAST,PMNS,BSPC,
             TRNS,NO,  P7,  P8,  P9,  PMNS,BSPC,
                  NO,  P4,  P5,  P6,  PPLS,PENT,
             TRNS,NO,  P1,  P2,  P3,  PPLS,PENT,
                       P0,  PDOT,SLSH,PENT,PENT,
        TRNS,TRNS,
        TRNS,
        TRNS,TRNS,TRNS
    ),
};

enum function_id {
  COPY_KEY,
  PASTE_KEY
};

const uint16_t fn_actions[] = {
    ACTION_LAYER_MOMENTARY(1),                      // FN0 - hold down for Layer1 (function keys)
    ACTION_LAYER_TOGGLE(2),                         // FN1 - switch to Layer2 (numpad + keyboard features)
    ACTION_LAYER_SET(0, ON_PRESS),                  // FN2 - switch to Layer0
    ACTION_MODS_TAP_KEY(MOD_LCTL, KC_ESC),          // FN3 - LCTL on tap, ESC on hold
    ACTION_MODS_KEY(MOD_LGUI | MOD_LSFT, KC_LBRC),  // FN4 - CMD + Shift + [
    ACTION_MODS_KEY(MOD_LGUI | MOD_LSFT, KC_RBRC),  // FN5 - CMD + Shift + ]
    ACTION_FUNCTION_TAP(COPY_KEY),                  // FN6 - LGui on hold, Copy on tap
    ACTION_FUNCTION_TAP(PASTE_KEY),                 // FN7 - LAlt on hold, Paste on tap
};

void action_function(keyrecord_t *record, uint8_t id, uint8_t opt)
{
    /*
     * This allows my LGui and LAlt keys to double as copy and paste
     * macros when tapped.
     */
    if (id == COPY_KEY || id == PASTE_KEY) {
        /* Pass through LGui or LAlt on hold */
        if (record->tap.count == 0 || record->tap.interrupted) {
            uint8_t weak_mods;

            if (id == COPY_KEY) {
                weak_mods = MOD_LGUI;
            } else {
                weak_mods = MOD_LALT;
            }

            if (record->event.pressed) {
                add_weak_mods(weak_mods);
            } else {
                del_weak_mods(weak_mods);

                /* It appears to be necessary to send a report here to
                 * avoid having LGui get "stuck" on. */
                send_keyboard_report();
            }
        /* Send CMD+c or CMD+v on tap */
        } else {
            uint8_t keycode;

            if (id == COPY_KEY) {
              keycode = KC_C;
            } else {
              keycode = KC_V;
            }

            if (record->event.pressed) {
                add_weak_mods(MOD_LGUI);
                add_key(keycode);
                send_keyboard_report();
            } else {
                del_weak_mods(MOD_LGUI);
                del_key(keycode);
                send_keyboard_report();
            }
        }
    }
}
