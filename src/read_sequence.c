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

    for (uint8_t i = 0; i <= 21; i += 3)
    {
        duration_array[counter] = descrambled_sequence[i];
        brightness_array[counter] = descrambled_sequence[i + 1];
        octave_array[counter] = (descrambled_sequence[i + 2] >> 4);
        note = (descrambled_sequence[i + 2] & 0b00001111);
        if (octave_array[counter] == 0)
        {
            buzzer_array[counter] = 0;
        }
        else
        {
            switch (note)
            {
            case 0:
                buzzer_array[counter] = 60606;
                break;
            case 1:
                buzzer_array[counter] = 57471;
                break;
            case 2:
                buzzer_array[counter] = 53763;
                break;
            case 3:
                buzzer_array[counter] = 51282;
                break;
            case 4:
                buzzer_array[counter] = 48309;
                break;
            case 5:
                buzzer_array[counter] = 45662;
                break;
            case 6:
                buzzer_array[counter] = 42735;
                break;
            case 7:
                buzzer_array[counter] = 40650;
                break;
            case 8:
                buzzer_array[counter] = 38314;
                break;
            case 9:
                buzzer_array[counter] = 36232;
                break;
            case 10:
                buzzer_array[counter] = 34014;
                break;
            case 11:
                buzzer_array[counter] = 32051;
                break;
            }

            for (uint8_t i = 0; i < (octave_array[counter] - 1); i++)
            {
                buzzer_array[counter] = buzzer_array[counter] >> 1;
            }

            cmp1buf_array[counter] = (buzzer_array[counter] * (brightness_array[counter] + 1)) >> 8;
        }

        counter++;
    }
}