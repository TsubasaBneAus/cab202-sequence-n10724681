#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

// For displaying '8' on the left side of the 7 segment display
void spi_init(void)
{
    cli();
    PORTMUX.SPIROUTEA = PORTMUX_SPI0_ALT1_gc;
    PORTC.DIR |= (PIN0_bm | PIN2_bm);
    PORTA.OUTSET = PIN1_bm;
    PORTA.DIRSET = PIN1_bm;
    SPI0.CTRLA = SPI_MASTER_bm;
    SPI0.CTRLB = SPI_SSD_bm;
    SPI0.INTCTRL = SPI_IE_bm;
    SPI0.CTRLA |= SPI_ENABLE_bm;
    sei();
}

void spi_write(uint8_t data)
{
    SPI0.DATA = data;
}

ISR(SPI0_INT_vect)
{
    PORTA.OUTCLR = PIN1_bm;
    PORTA.OUTSET = PIN1_bm;
}