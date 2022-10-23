#include <avr/io.h>

void display_and_button_init(void) {
    PORTA.DIRSET |= PIN1_bm;
    PORTA.OUTSET |= PIN1_bm;
    PORTA.OUTCLR |= PIN1_bm;

    PORTB.DIRSET |= PIN5_bm;
    PORTB.OUTSET |= PIN5_bm;

    PORTA.PIN4CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN5CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN6CTRL = PORT_PULLUPEN_bm;
    PORTA.PIN7CTRL = PORT_PULLUPEN_bm;
}