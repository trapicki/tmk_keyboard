pastrami - the simplest keyboard
=================================
copied from onekey for a starter.

Currently for [Teensy][++ 2.0].
Showing NumLock and generating keycodes at keypress for a single layer.
Version 2 with 2x2 keys and layers. Easy configurable for arbitrary key matrix sizes.

The most basic keyboard example
-------------------------------
* Needs nothing more than a Teensy++ 2.0 (or similar).
* Shows up as HID device when connected. This will work with every HID USB driver, every Operating System.
* Will send a keys code when switches are connected and pressed.
* Will show NumLock state.
* Easy to modify for changes - ports and pins centrally defined.

This example is specifically suitable as a demonstration and starting point of a DIY keyboard with a Teensy++ 2.0. Just change the defines in 3 files, flash the board and you have your keyboard.

Howto use
---------
1. Get [teensy_loader](teensy_loader) and [hid_listen](hid_listen).
2. Edit [config.h] for `MATRIX_ROWS` and `MATRIX_COLS`. Edit [matrix.c] for your pin assignments, and edit [keymap.c] for your keycode assignment.
2. Run `make -f Makefile.pjrc teensy`.
3. Run `hid_listen` to see some debug info.
4. Press NumLock on some other keyboard or press your key

Tested only with PJRC USB stack.

Modify it to your taste!
------------------------
The *Numlock* state is output to pin `D6`, which is connected to the on-board LED. If you want to use a different port, just change the defines at the beginning of [led.c].

The *keys* are read in [matrix.c]. The *row* and *col* (column) pins are defined by `PORT_ROW_x`, `BIT_ROW_x`, `PORT_COL_x`, `BIT_COL_x`, where x are ascending numbers in decimal, starting with **0**.

The pin definitions must match the definitions of `MATRIX_ROWS` and `MATRIX_COLS` in [config.h]

The default setting is F6/F7 for row0/row1 and F3/F4 for col0/col1.

Look at the parts side with the USB connector on the bottom, these pins are in the top left, just above the reset button.

                   +---------+
       row1 --  F7 |         | C7
       row0 --  F6 |   Reset | C6
                F5 |         | C5
       col1 --  F4 |         | C4
       col0 --  F3 |     LED | C3
                F2 |         | C2
                F1 |    X    | C1
                F0 |   / \   | C0
              AREF |  /   \  | E1
               GND | X µC  X | E0
                E6 |  \   /  | D7
                E7 |   \ /   | D6  LED/NumLock
                B0 |    X    | D5
                B1 |         | D4
                B2 |         | D3
                B3 |         | D2
                B4 | +-----+ | D1
                B5 | |     | | D1
                B6 | |MINI | | B7
               VCC | | USB | | GND
                   +-+-----+-+

The *keycodes* to be sent can be changed in [keymap.c].
The default keymap is:

    Layer 0       Layer 1
    Keys/WASD     Cursor movement
       |             |
       | 0  1        | 0 1
    ---+------    ---+------
       |             |
    r0 | a w      r0 | ← ↑
       |             |
    r1 | s d      r1 | ↓ →
       |             |

* Layer 0 is a somewhat rotated WASD cross, so you can already play your favourite game!
* Layer 1 gives you the cursor movement key, so you can play your favourite game now even with the right hand.

To switch layers, press (all) four keys.

To browse the code use your favorite IDE, or a source cross reference system. I prefer *GNU Global* (`htags`).

Why pastrami?
-------------
It's great and tasty, related to bastirma, and has something to do with my name.

Share and enjoy!

<trapicki@wirklich.priv.at>


[Teensy]:    https://www.pjrc.com/teensy/ "Teensy overview"
[++ 2.0]:    https://www.pjrc.com/teensy/pinout.html "Teensy pinout."
[teensy_loader]: https://www.pjrc.com/teensy/loader.html
[hid_listen]: https://www.pjrc.com/teensy/hid_listen.html
[led.c]:     led.c
[matrix.c]:  matrix.c
[keymap.c]:  keymap.c
[config.h]:  config.h
