pastrami - the simplest keyboard.
=================================
copied from onekey for a starter.

Currently for [Teensy][][++ 2.0][], showing NumLock and generating an "a" keycode at keypress.

The most basic keyboard example
-------------------------------
* Needs nothing more than a Teensy++ 2.0 (or similar).
* Shows up as HID device when connected. This will work with every HID USB driver, every Operating System.
* Will send a key code when a switch is connected and pressed, or at least the right pins are connected/shorted.
* Will show NumLock state.
* Easy to modify for changes - ports and pins centrally defined.

This example is specifically suitable as a demonstration and starting point of a DIY keyboard with a Teensy++ 2.0. Just flash the board and it will be a keyboard.

Howto use
---------
1. Get [teensy_loader][] and [hid_listen].
2. Run `make -f Makefile.pjrc teensy`.
3. Run `hid_listen` to see some debug info.
4. Press NumLock on some other keyboard or press your key

Tested only with PJRC USB stack.

Modify it to your taste!
------------------------
The *Numlock* state is output to pin `D6`, which is connected to the on-board LED. If you want to use a different port, just change the defines at the beginning of [led.c].

The *key* is read in [matrix.c]. The *row* pin is by default `F6`, the *column* pin is `F7`. if you look at the parts side with the USB connector on the left, these two pins are in the top row the two right most, just above the reset button.

                                   row1  col1
                                      |  |
    /---------------------------------F6-F7-\
    |                  ^                    |
    |                 / \              Re   |
    MINI             < µC>             set  |
    USB               \ /     LED           |
    |                  v     /              |
    \--------------D6-------/-------------- /
                    |      /
                    NumLock

The *keycode* to be sent can be changed in [keymap.c].

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
