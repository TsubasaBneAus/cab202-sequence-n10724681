#include <avr/io.h>
#include <stdint.h>

void uart_init(void)
{
    PORTB.DIRSET = PIN2_bm;
    USART0.BAUD = 1389;
    USART0.CTRLB = USART_RXEN_bm | USART_TXEN_bm;
}

uint8_t uart_has_data(void) {
    return USART0.STATUS & USART_RXCIF_bm;
}

uint8_t uart_getc(void)
{
    while (!(USART0.STATUS & USART_RXCIF_bm));
    return USART0.RXDATAL;
}

void uart_putc(uint8_t c)
{
    while (!(USART0.STATUS & USART_DREIF_bm)); // Wait for TXDATA empty
    USART0.TXDATAL = c;
}