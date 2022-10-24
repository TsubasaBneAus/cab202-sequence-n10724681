#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include "display_hex.h"
#include "read_sequence.h"

// STATE sequence_mode;
uint8_t sequence_index;
uint16_t timer_counter = 0;
uint8_t duration_array[8];
uint16_t brightness_array[8];
uint32_t buzzer_array[8];
uint8_t octave_array[8];
uint32_t cmp1buf_array[8];
uint32_t final_perbuf;
uint32_t final_cmp0buf;
uint32_t final_cmp1buf;
uint8_t sequence_state = 0;
uint8_t paused_sequence_state = 0;
uint8_t sequence_mode_2 = 0;
uint8_t sequence_index_2;
uint8_t read_sequence_counter;

// Timer for DURATION
void timer0_init()
{
    cli();
    TCB0.CTRLB = TCB_CNTMODE_INT_gc;                 // Configure TCB0 in periodic interrupt mode
    TCB0.CCMP = 43666;                               // Set interval for 13.1 ms (254 clocks @ 3.3 MHz) (13.1 * 10 ^ (-3)) / (300 * 10 ^ (-9)) = 43666.666
    TCB0.INTCTRL = TCB_CAPT_bm;                      // CAPT interrupt enable
    TCB0.CTRLA = TCB_CLKSEL_DIV1_gc | TCB_ENABLE_bm; // Enable
    sei();
}

ISR(TCB0_INT_vect)
{
    if (sequence_state == 7 && timer_counter == duration_array[7])
    {
        // The 8th sequence finishes
        // Read the next sequence
        sequence_index++;
        read_sequence_counter++;
        read_sequence();
        timer_counter = 0;
        sequence_state = 0;
        paused_sequence_state = 0;
    }

    if (sequence_state == 0)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[0] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == 0)
            {
                // The 1st sequence starts
                TCA0.SINGLE.CMP1BUF = 0;
                TCA0.SINGLE.PERBUF = 33333;
                digit1 = 0b10000000;
                digit2 = 0b01111111;

                if (octave_array[0] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[0];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[0];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[0] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[0];
                }
            }
            else if (timer_counter == duration_array[0])
            {
                // The 1st sequence finishes
                sequence_state = 1;
                timer_counter = 0;

                // The 2nd sequence starts
                if (octave_array[1] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[1];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[1];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[1] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[1];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 1)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[1] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[1])
            {
                // The 2nd sequence finishes
                sequence_state = 2;
                timer_counter = 0;

                // The 3rd sequence starts
                if (octave_array[2] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[2];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[2];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[2] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[2];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 2)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[2] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[2])
            {
                // The 3rd sequence finishes
                sequence_state = 3;
                timer_counter = 0;

                // The 4th sequence starts
                if (octave_array[3] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[3];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[3];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[3] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[3];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 3)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[3] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[3])
            {
                // The 4th sequence finishes
                sequence_state = 4;
                timer_counter = 0;

                // The 5th sequence starts
                if (octave_array[4] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[4];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[4];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[4] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[4];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 4)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[4] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[4])
            {
                // The 5th sequence finishes
                sequence_state = 5;
                timer_counter = 0;

                // The 6th sequence starts
                if (octave_array[5] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[5];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[5];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[5] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[5];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 5)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[5] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[5])
            {
                // The 6th sequence finishes
                sequence_state = 6;
                timer_counter = 0;

                // The 7th sequence starts
                if (octave_array[6] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[6];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[6];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[6] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[6];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 6)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[6] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }
            else if (timer_counter == duration_array[6])
            {
                // The 7th sequence finishes
                sequence_state = 7;
                timer_counter = 0;

                // The 8th sequence starts
                if (octave_array[7] == 0)
                {
                    TCA0.SINGLE.PERBUF = 255;
                    TCA0.SINGLE.CMP0BUF = 0;
                    TCA0.SINGLE.CMP1BUF = brightness_array[7];
                    final_perbuf = 255;
                    final_cmp0buf = 0;
                    final_cmp1buf = brightness_array[7];
                }
                else
                {
                    TCA0.SINGLE.PERBUF = buzzer_array[7];
                    TCA0.SINGLE.CMP0BUF = buzzer_array[7] / 2;
                    TCA0.SINGLE.CMP1BUF = cmp1buf_array[7];
                    final_perbuf = buzzer_array[7];
                    final_cmp0buf = buzzer_array[7] / 2;
                    final_cmp1buf = cmp1buf_array[7];
                }
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 7)
    {
        if (sequence_mode_2 == 1)
        {
            // Pause the current sequence
            timer_counter = 0;
        }
        else if (sequence_mode_2 == 0)
        {
            if (duration_array[7] == 0)
            {
                // Stop the current sequence
                sequence_mode_2 = 2;
                TCA0.SINGLE.PERBUF = 33333;
                TCA0.SINGLE.CMP1BUF = 33333;
                TCA0.SINGLE.CMP0BUF = 0;
                sequence_state = 9;
                timer_counter = 0;
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
            }

            timer_counter++;
        }
    }
    else if (sequence_state == 8)
    {
        sequence_mode_2 = 2;
        TCA0.SINGLE.PERBUF = 33333;
        TCA0.SINGLE.CMP1BUF = 33333;
        TCA0.SINGLE.CMP0BUF = 0;
        sequence_state = 9;
        timer_counter = 0;
        display_hex(sequence_index);
    }

    TCB0.INTFLAGS = TCB_CAPT_bm;
}