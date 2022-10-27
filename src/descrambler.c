#include <stdint.h>
#include <stdio.h>
#include "decoder.h"

// Descramble bytes of SEQUENCE
void descramble_sequence(uint8_t sequence_index, uint8_t decoded_sequence[24], uint8_t descrambled_sequence[24])
{
    uint32_t state_lfsr_1 = 0x10724681;
    uint8_t lsb;
    uint32_t mask = 0xB4BCD35C;
    uint32_t state_lfsr_2;

    if ((sequence_index == 0) || (serial_command_test == 2))
    {
        state_lfsr_2 = state_lfsr_1;
    }
    else
    {
        state_lfsr_2 = state_lfsr_1;
        for (uint8_t i = 0; i < sequence_index; i++)
        {
            for (uint8_t i = 0; i < 24; i++)
            {
                lsb = state_lfsr_2 & 0x1;
                state_lfsr_2 = state_lfsr_2 >> 1;
                if (lsb == 1)
                {
                    if (lsb == 1)
                    {
                        state_lfsr_2 = state_lfsr_2 ^ mask;
                    }
                }
            }
        }
    }

    if (serial_command_test == 3)
    {
        for (uint8_t i = 0; i < 24; i++)
        {
            descrambled_sequence[i] = decoded_sequence[i];
        }
    }
    else
    {
        for (uint8_t i = 0; i < 24; i++)
        {
            descrambled_sequence[i] = (state_lfsr_2 & 0xFF) ^ (decoded_sequence[i]);
            lsb = state_lfsr_2 & 0x1;
            state_lfsr_2 = state_lfsr_2 >> 1;
            if (lsb == 1)
            {
                state_lfsr_2 = state_lfsr_2 ^ mask;
            }
        }
    }
}