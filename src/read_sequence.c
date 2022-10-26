#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include "decoder.h"
#include "descrambler.h"
#include "timer0.h"

void read_sequence()
{
    uint8_t decoded_sequence[24];
    uint8_t descrambled_sequence[24];

    // Decode base64 strings
    decode_sequence(sequence_index, decoded_sequence);

    // Descramble a decoded string
    descramble_sequence(sequence_index, decoded_sequence, descrambled_sequence);

    // Extract each sequence data by 3 bytes and store each bits data
    // to duration_array, octave_array, note, and brightness_array respectively
    uint8_t counter = 0;
    uint8_t note;
    uint16_t frequency;

    for (uint8_t i = 0; i <= 21; i += 3)
    {
        duration_array[counter] = descrambled_sequence[i];
        brightness_array[counter] = descrambled_sequence[i + 1];
        octave_array[counter] = (descrambled_sequence[i + 2] >> 4);
        note = (descrambled_sequence[i + 2] & 0b00001111);
        if (octave_array[counter] == 0)
        {
            buzzer_array[counter] = 0;
            frequency = 0;
        }
        else
        {
            switch (note)
            {
            case 0:
                frequency = 55;
                break;
            case 1:
                frequency = 58;
                break;
            case 2:
                frequency = 62;
                break;
            case 3:
                frequency = 65;
                break;
            case 4:
                frequency = 69;
                break;
            case 5:
                frequency = 73;
                break;
            case 6:
                frequency = 78;
                break;
            case 7:
                frequency = 82;
                break;
            case 8:
                frequency = 87;
                break;
            case 9:
                frequency = 92;
                break;
            case 10:
                frequency = 98;
                break;
            case 11:
                frequency = 104;
                break;
            }

            for (uint8_t i = 0; i < (octave_array[counter] - 1); i++)
            {
                frequency = frequency * 2;
            }

            buzzer_array[counter] = 3333333 / frequency;
            cmp1buf_array[counter] = (buzzer_array[counter] * (brightness_array[counter] + 1)) >> 8;
        }

        counter++;
    }
}