#include <stdint.h>

uint8_t digit1 = 0b10001000;
uint8_t digit2 = 0b00001000;

void display_hex(uint8_t sequence_index)
{
    if (sequence_index >= 0 && sequence_index <= 15) {
        digit1 = 136;
    } else if (sequence_index >= 16 && sequence_index <= 31) {
        digit1 = 235;
    } else if (sequence_index >= 32 && sequence_index <= 47) {
        digit1 = 196;
    } else if (sequence_index >= 48 && sequence_index <= 63) {
        digit1 = 193;
    } else if (sequence_index >= 64 && sequence_index <= 79) {
        digit1 = 163;
    } else if (sequence_index >= 80 && sequence_index <= 95) {
        digit1 = 145;
    } else if (sequence_index >= 96 && sequence_index <= 111) {
        digit1 = 144;
    } else if (sequence_index >= 112 && sequence_index <= 127) {
        digit1 = 203;
    } else if (sequence_index >= 128 && sequence_index <= 143) {
        digit1 = 128;
    } else if (sequence_index >= 144 && sequence_index <= 159) {
        digit1 = 129;
    } else if (sequence_index >= 160 && sequence_index <= 175) {
        digit1 = 130;
    } else if (sequence_index >= 176 && sequence_index <= 191) {
        digit1 = 176;
    } else if (sequence_index >= 192 && sequence_index <= 207) {
        digit1 = 156;
    } else if (sequence_index >= 208 && sequence_index <= 223) {
        digit1 = 224;
    } else if (sequence_index >= 224 && sequence_index <= 239) {
        digit1 = 148;
    } else if (sequence_index >= 240 && sequence_index <= 255) {
        digit1 = 150;
    }


    if ((sequence_index % 16) == 0) {
        digit2 = 8;
    } else if ((sequence_index % 16) == 1) {
        digit2 = 107;
    } else if ((sequence_index % 16) == 2) {
        digit2 = 68;
    } else if ((sequence_index % 16) == 3) {
        digit2 = 65;
    } else if ((sequence_index % 16) == 4) {
        digit2 = 35;
    } else if ((sequence_index % 16) == 5) {
        digit2 = 17;
    } else if ((sequence_index % 16) == 6) {
        digit2 = 16;
    } else if ((sequence_index % 16) == 7) {
        digit2 = 75;
    } else if ((sequence_index % 16) == 8) {
        digit2 = 0;
    } else if ((sequence_index % 16) == 9) {
        digit2 = 1;
    } else if ((sequence_index % 16) == 10) {
        digit2 = 2;
    } else if ((sequence_index % 16) == 11) {
        digit2 = 48;
    } else if ((sequence_index % 16) == 12) {
        digit2 = 28;
    } else if ((sequence_index % 16) == 13) {
        digit2 = 96;
    } else if ((sequence_index % 16) == 14) {
        digit2 = 20;
    } else if ((sequence_index % 16) == 15) {
        digit2 = 22;
    }
}