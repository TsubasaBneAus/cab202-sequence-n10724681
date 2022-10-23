#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include "timer0.h"
#include "timer1.h"
#include "display_and_button.h"
#include "spi.h"
#include "display_hex.h"
#include "pwm.h"
#include "read_sequence.h"
#include "adc.h"
#include "qutyserial.h"

void display_hex(uint8_t sequence_index);

int main(void)
{
    sequence_index = 0;
    sequence_state = 9;
    timer_counter = 0;

    serial_init();

    display_and_button_init();
    spi_init();
    adc_init();
    pwm_init();
    timer0_init();
    timer1_init();

    uint8_t pb_current = 0xFF;
    uint8_t pb_previous = 0xFF;
    uint8_t pb_changed;
    uint8_t pb_falling;

    typedef enum
    {
        SELECT_SEQUENCE,
        EXECUTE_SEQUENCE,
        PAUSE_SEQUENCE,
        TEST
    } STATE;

    STATE sequence_mode_1 = SELECT_SEQUENCE;
    uint8_t sequence_mode_3 = 0;

    while (1)
    {
        pb_previous = pb_current;
        pb_current = pb_debounced;

        pb_changed = pb_current ^ pb_previous;
        pb_falling = pb_changed & ~pb_current;

        switch (sequence_mode_1)
        {
        case SELECT_SEQUENCE:
            if (pb_falling & PIN4_bm)
            {
                // S1 pressed
                while (~PORTA.IN & PIN4_bm)
                {
                    sequence_index = (ADC0.RESULT >> 8);
                    display_hex(sequence_index);
                }
            }
            else if (pb_falling & PIN5_bm)
            {
                // S2 pressed
                if (sequence_index == 0)
                {
                    display_hex(sequence_index);
                }
                else
                {
                    sequence_index--;
                    display_hex(sequence_index);
                }
            }
            else if (pb_falling & PIN6_bm)
            {
                // S3 pressed
                if (sequence_index == 255)
                {
                    display_hex(sequence_index);
                }
                else
                {
                    sequence_index++;
                    display_hex(sequence_index);
                }
            }
            else if (pb_falling & PIN7_bm)
            {
                // S4 pressed
                sequence_index_2 = sequence_index;
                read_sequence();
                sequence_mode_1 = EXECUTE_SEQUENCE;
                sequence_mode_2 = 0;
                sequence_state = 0;
                timer_counter = 0;
            }
            break;

        case EXECUTE_SEQUENCE:
            if (sequence_mode_3 == 1)
            {
                sequence_mode_1 = PAUSE_SEQUENCE;
                sequence_mode_3 = 0;
                break;
            }

            if (pb_falling & PIN7_bm)
            {
                // S4 pressed
                paused_sequence_state = sequence_state;
                sequence_mode_1 = PAUSE_SEQUENCE;
            }

            if (sequence_mode_2 == 2)
            {
                display_hex(sequence_index_2);
                sequence_mode_1 = SELECT_SEQUENCE;
                sequence_mode_2 = 0;
            }

            break;

        case PAUSE_SEQUENCE:
            if (paused_sequence_state == 0) {
                sequence_state = paused_sequence_state;
                timer_counter = 0;
            } else {
                sequence_state = paused_sequence_state - 1;
                timer_counter = duration_array[paused_sequence_state - 1];
            }
            // sequence_state = paused_sequence_state;
            // timer_counter = duration_array[paused_sequence_state];
            // timer_counter = 0;
            if (pb_falling & PIN5_bm)
            {
                // S2 pressed
                sequence_state = 8;
                sequence_mode_1 = SELECT_SEQUENCE;
                sequence_mode_2 = 0;
            }
            else if (pb_falling & PIN6_bm)
            {
                // S3 pressed
                if (paused_sequence_state <= 7)
                {
                    paused_sequence_state++;
                }
                sequence_mode_1 = EXECUTE_SEQUENCE;
                sequence_mode_2 = 0;
                sequence_mode_3 = 1;
            }
            else if (pb_falling & PIN7_bm)
            {
                // S4 pressed
                if (paused_sequence_state <= 7)
                {
                    paused_sequence_state++;
                }
                sequence_mode_1 = EXECUTE_SEQUENCE;
                sequence_mode_2 = 0;
            }

            if (sequence_mode_2 == 2)
            {
                sequence_mode_1 = SELECT_SEQUENCE;
                sequence_mode_2 = 0;
            }

            break;

        case TEST:
            break;
        }
    }
}