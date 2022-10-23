#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "spi.h"
#include "display_hex.h"

uint8_t pb_debounced = 0xFF;
uint8_t display_state = 0;

void timer1_init() {
    cli();
    TCB1.CTRLB = TCB_CNTMODE_INT_gc;
    CPUINT.LVL1VEC = 25;
    TCB1.CCMP = 33333;
    TCB1.INTCTRL = TCB_CAPT_bm;
    TCB1.CTRLA = TCB_CLKSEL_DIV1_gc | TCB_ENABLE_bm;
    sei();
}

ISR(TCB1_INT_vect)
{
    // Process button debouncing
    static uint8_t count0 = 0;
    static uint8_t count1 = 0;

    uint8_t pb_current_state = PORTA.IN;
    uint8_t pb_changed = pb_current_state ^ pb_debounced;

    count1 = (count1 ^ count0) & pb_changed;
    count0 = ~count0 & pb_changed;

    pb_debounced ^= (count0 & count1);

    if (display_state == 0)
    {
        // display the first number on LHS
        spi_write(digit1);
        display_state = 1;
    }
    else
    {
        // display the second number on RHS
        spi_write(digit2);
        display_state = 0;
    }
    TCB1.INTFLAGS = TCB_CAPT_bm;
}