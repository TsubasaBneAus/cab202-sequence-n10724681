#include <stdint.h>
#include "sequence.h"

char test_sequence[33];
uint8_t serial_command_test = 0;

// Decode bytes of SEQUENCE
void decode_sequence(uint8_t sequence_index, uint8_t decoded_sequence[24])
{
    uint16_t sequence_offset;
    if (serial_command_test == 0) {
        sequence_offset = 32 * sequence_index;
    }
    else
    {
        sequence_offset = 0;
    }

    uint8_t converted_sequence[32];
    uint8_t counter_1 = 0;

    for (uint16_t i = sequence_offset; i < (sequence_offset + 32); i++)
    {
        char temp_encoded;

        if (serial_command_test == 0)
        {
            temp_encoded = pgm_read_byte(&(SEQUENCE[i]));
        }
        else
        {
            temp_encoded = test_sequence[i];
        }

        if (temp_encoded >= 'A' && temp_encoded <= 'Z')
        {
            converted_sequence[counter_1] = temp_encoded - 65; // 'A' ~ 'Z'
        }
        else if (temp_encoded >= 'a' && temp_encoded <= 'z')
        {
            converted_sequence[counter_1] = temp_encoded - 71; // 'a' ~ 'z'
        }
        else if (temp_encoded >= '0' && temp_encoded <= '9')
        {
            converted_sequence[counter_1] = temp_encoded + 4; // '0' ~ '9'
        }
        else if (temp_encoded == '+')
        {
            converted_sequence[counter_1] = 62; // '+'
        }
        else
        {
            converted_sequence[counter_1] = 63; // '/'
        }
        counter_1++;
    }

    uint8_t counter_2 = 0;
    for (uint8_t i = 0; i < 29; i += 4)
    {
        decoded_sequence[counter_2] = (converted_sequence[i] << 2) | (converted_sequence[i + 1] >> 4);
        decoded_sequence[counter_2 + 1] = (converted_sequence[i + 1] << 4) | (converted_sequence[i + 2] >> 2);
        decoded_sequence[counter_2 + 2] = (converted_sequence[i + 2] << 6) | converted_sequence[i + 3];
        counter_2 += 3;
    }
}