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

/*
 * scan matrix
 */
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>
#include <util/delay.h>
#include "print.h"
#include "debug.h"
#include "util.h"
#include "matrix.h"


#ifndef DEBOUNCE
#   define DEBOUNCE	5
#endif

/// comment/uncomment according to number of MATRIX_ROWS and MATRIX_COLS in config.h
#define PORT_COL_0 F
#define BIT_COL_0 3
#define PORT_COL_1 F
#define BIT_COL_1 4
//#define PORT_COL_2 F
//#define BIT_COL_2 4
//#define PORT_COL_3 F
//#define BIT_COL_3 4
//#define PORT_COL_4 F
//#define BIT_COL_4 4
//#define PORT_COL_5 F
//#define BIT_COL_5 4
//#define PORT_COL_6 F
//#define BIT_COL_6 4
//#define PORT_COL_7 F
//#define BIT_COL_7 4
//#define PORT_COL_8 F
//#define BIT_COL_8 4
//#define PORT_COL_9 F
//#define BIT_COL_9 4
//#define PORT_COL_10 F
//#define BIT_COL_10 4
//#define PORT_COL_11 F
//#define BIT_COL_11 4
//#define PORT_COL_12 F
//#define BIT_COL_12 4
//#define PORT_COL_13 F
//#define BIT_COL_13 4
//#define PORT_COL_14 F
//#define BIT_COL_14 4
//#define PORT_COL_15 F
//#define BIT_COL_15 4

#define PORT_ROW_0 F
#define BIT_ROW_0 6
#define PORT_ROW_1 F
#define BIT_ROW_1 7
//#define PORT_ROW_2 F
//#define BIT_ROW_2 7
//#define PORT_ROW_3 F
//#define BIT_ROW_3 7
//#define PORT_ROW_4 F
//#define BIT_ROW_4 7
//#define PORT_ROW_5 F
//#define BIT_ROW_5 7
//#define PORT_ROW_6 F
//#define BIT_ROW_6 7
//#define PORT_ROW_7 F
//#define BIT_ROW_7 7
///


// some preprocessort tricks:
// CAT concatenates
// CAT_INDIRECT introduces additional indirection, giving thre preprocessor one more turn to resolve macros.
#define CAT(a, b) a ## b
#define CAT_INDIRECT(a, b) CAT(a, b)

// define the right register
#define DDR(a) CAT_INDIRECT(DDR, PORT_##a)
#define PIN(a) CAT_INDIRECT(PIN, PORT_##a)
#define PORT(a) CAT_INDIRECT(PORT, PORT_##a)

static uint8_t debouncing = DEBOUNCE;

/* matrix state(1:on, 0:off) */
static matrix_row_t matrix[MATRIX_ROWS];
static matrix_row_t matrix_debouncing[MATRIX_ROWS];

static matrix_row_t read_cols(void);
static void init_cols(void);
static void unselect_rows(void);
static void select_row(uint8_t row);

inline
uint8_t matrix_rows(void)
{
    return MATRIX_ROWS;
}

inline
uint8_t matrix_cols(void)
{
    return MATRIX_COLS;
}

void matrix_init(void)
{
    debug_enable = true;
    debug_matrix = true;
    //debug_mouse = true;
    // initialize row and col
    unselect_rows();
    init_cols();

    // initialize matrix state: all keys off
    for (uint8_t i=0; i < MATRIX_ROWS; i++) {
        matrix[i] = 0;
        matrix_debouncing[i] = 0;
    }
}

uint8_t matrix_scan(void)
{
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        select_row(i);
        _delay_us(30);  // without this wait read unstable value.
        matrix_row_t cols = read_cols();
        if (matrix_debouncing[i] != cols) {
            matrix_debouncing[i] = cols;
            if (debouncing) {
                debug("bounce!: "); debug_hex(debouncing); debug("\n");
            }
            debouncing = DEBOUNCE;
        }
        unselect_rows();
    }

    if (debouncing) {
        if (--debouncing) {
            _delay_ms(1);
        } else {
            for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
                matrix[i] = matrix_debouncing[i];
            }
        }
    }

    return 1;
}

bool matrix_is_modified(void)
{
    if (debouncing) return false;
    return true;
}

inline
bool matrix_is_on(uint8_t row, uint8_t col)
{
    return (matrix[row] & ((matrix_row_t)1<<col));
}

inline
matrix_row_t matrix_get_row(uint8_t row)
{
    return matrix[row];
}

void matrix_print(void)
{
    print("r/c 0123456789ABCDEF\n");
    for (uint8_t row = 0; row < MATRIX_ROWS; row++) {
        xprintf("%X:  %016b\n", row, bitrev16(matrix_get_row(row)));
    }
}


uint8_t matrix_key_count(void)
{
    uint8_t count = 0;
    for (uint8_t i = 0; i < MATRIX_ROWS; i++) {
        count += bitpop16(matrix[i]);
    }
    return count;
}

static void  init_cols(void)
{
// Input with pull-up(DDR:0, PORT:1)
#define INIT_COL(a)  DDR(COL_##a)  &= ~(1<<BIT_COL_##a);\
                     PORT(COL_##a) |=  (1<<BIT_COL_##a)
    /// comment/uncomment according to number of MATRIX_ROWS and MATRIX_COLS in config.h
    INIT_COL(0);
    INIT_COL(1);
    /*
    INIT_COL(2);
    INIT_COL(3);
    INIT_COL(4);
    INIT_COL(5);
    INIT_COL(6);
    INIT_COL(7);
    INIT_COL(8);
    INIT_COL(9);
    INIT_COL(10);
    INIT_COL(11);
    INIT_COL(12);
    INIT_COL(13);
    INIT_COL(14);
    INIT_COL(15);
    */
    ///
#undef INITCOL
}

static matrix_row_t read_cols(void)
{
    //
#define ADD_COL(col) (PIN(COL_##col) & (1<<BIT_COL_##col) ? 0 : (1<<col))
    return (
            /// comment/uncomment according to number of MATRIX_ROWS and MATRIX_COLS in config.h
            ADD_COL(0)
          | ADD_COL(1)
            /*
          | ADD_COL(2)
          | ADD_COL(3)
          | ADD_COL(4)
          | ADD_COL(5)
          | ADD_COL(6)
          | ADD_COL(7)
          | ADD_COL(8)
          | ADD_COL(9)
          | ADD_COL(10)
          | ADD_COL(11)
          | ADD_COL(12)
          | ADD_COL(13)
          | ADD_COL(14)
          | ADD_COL(15)
            */
            ///
     );
#undef ADD_COL
}

static void unselect_rows(void)
{
// Hi-Z(DDR:0, PORT:0) to unselect
#define UNSELECT_ROW(row) DDR(ROW_##row &= ~(1<<BIT_ROW_##row)); \
                     PORT(ROW_##row &= ~(1<<BIT_ROW_##row))
    /// comment/uncomment according to number of MATRIX_ROWS and MATRIX_COLS in config.h
    UNSELECT_ROW(0);
    UNSELECT_ROW(1);
    /*
    UNSELECT_ROW(2);
    UNSELECT_ROW(3);
    UNSELECT_ROW(4);
    UNSELECT_ROW(5);
    UNSELECT_ROW(6);
    UNSELECT_ROW(7);
    */
    ///
#undef UNSELECT_ROW
}

static void select_row(uint8_t row)
{
// Output low(DDR:1, PORT:0) to select
#define ADD_CASE(row) case row: \
    DDR(ROW_##row)  |=  (1<<BIT_ROW_##row);           \
    PORT(ROW_##row) &= ~(1<<BIT_ROW_##row);           \
    break
    switch (row) {
        /// comment/uncomment according to number of MATRIX_ROWS and MATRIX_COLS in config.h
        ADD_CASE(0);
        ADD_CASE(1);
        /*
        ADD_CASE(2);
        ADD_CASE(3);
        ADD_CASE(4);
        ADD_CASE(5);
        ADD_CASE(6);
        ADD_CASE(7);
        */
        ///
    }
#undef ADD_CASE
}
