#include <avr/io.h>
#include <avr/interrupt.h>


// For controlling the 7 segment brightness and the buzzer
void pwm_init() {
    cli();
    // Initialise DISP EN
    PORTB.DIRSET |= PIN1_bm;
    PORTB.OUTSET |= PIN1_bm;

    // Initialise BUZZER
    PORTB.OUTSET |= PIN0_bm;
    PORTB.DIRSET |= PIN0_bm;

    PORTMUX.TCAROUTEA = PORTMUX_TCA01_DEFAULT_gc | PORTMUX_TCA00_DEFAULT_gc;
    TCA0.SINGLE.CTRLA = TCA_SINGLE_CLKSEL_DIV1_gc;
    TCA0.SINGLE.CTRLB = TCA_SINGLE_WGMODE_SINGLESLOPE_gc | TCA_SINGLE_CMP1_bm | TCA_SINGLE_CMP0_bm; // CMP1/0 corresponds to Waveform Output 1/0
    TCA0.SINGLE.PER = 33333;
    TCA0.SINGLE.CMP1 = 33334;
    TCA0.SINGLE.CMP0 = 0;
    TCA0.SINGLE.CTRLA |= TCA_SINGLE_ENABLE_bm;
    sei();
}

ISR(TCA0_OVF_vect)
{
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}