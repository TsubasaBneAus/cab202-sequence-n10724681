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
#include "uart_init.h"
#include "qutyserial.h"

void display_hex(uint8_t sequence_index);

int main(void)
{
    sequence_index = 0;
    sequence_state = 9;
    timer_counter = 0;

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
    } STATE_1;

    STATE_1 sequence_mode_1 = SELECT_SEQUENCE;
    uint8_t sequence_mode_3 = 0;

    typedef enum
    {
        START,
        ESCAPE,
        ID,
        PAYLOAD
    } STATE_2;

    STATE_2 cmd_messages = START;

    typedef enum
    {
        CMD_SEQ,
        CMD_TEST,
        CMD_EXIT,
        CMD_PAUSE,
        CMD_STEP,
        CMD_SYNC,
        CMD_SEQIDX,
        CMD_TEST_SEQ,
        CMD_TEST_SEQNS,
        NO_CMD
    } STATE_3;

    STATE_3 serial_command = NO_CMD;

    char buf[128];
    uint8_t sequence_digit = 0;

    display_and_button_init();
    spi_init();
    adc_init();
    pwm_init();
    timer0_init();
    timer1_init();
    uart_init();

    while (1)
    {
        if (uart_has_data())
        {
            uint8_t c = uart_getc();

            switch (cmd_messages)
            {
            case START:
                if (c == '\\')
                {
                    sprintf(buf, "");
                    cmd_messages = ESCAPE;
                }
                else
                {
                    sprintf(buf, "#NACK\n");
                    cmd_messages = START;
                }
                break;

            case ESCAPE:
                if (c == '\\')
                {
                    sprintf(buf, "");
                    cmd_messages = ID;
                }
                else
                {
                    sprintf(buf, "#NACK\n");
                    cmd_messages = START;
                }
                break;

            case ID:
                switch (c)
                {
                case 's':
                    sequence_mode_1 = SELECT_SEQUENCE;
                    serial_command = CMD_SEQ;
                    sprintf(buf, "#ACK\n");
                    break;

                case 't':
                    serial_command = CMD_TEST;
                    cmd_messages = START;
                    sprintf(buf, "#ACK\n");
                    break;

                case 'e':
                    sequence_mode_1 = PAUSE_SEQUENCE;
                    serial_command = CMD_EXIT;
                    sprintf(buf, "#ACK\n");
                    break;

                case 'p':
                    sequence_mode_1 = EXECUTE_SEQUENCE;
                    serial_command = CMD_PAUSE;
                    sprintf(buf, "#ACK\n");
                    break;

                case 'n':
                    sequence_mode_1 = PAUSE_SEQUENCE;
                    serial_command = CMD_STEP;
                    sprintf(buf, "#ACK\n");
                    break;

                case 'y':
                    serial_command = CMD_SYNC;
                    cmd_messages = START;
                    sprintf(buf, "#ACK\n");
                    break;

                case 0x69:
                    serial_command = CMD_SEQIDX;
                    cmd_messages = PAYLOAD;
                    break;

                case 'd':
                    serial_command = CMD_TEST_SEQ;
                    cmd_messages = PAYLOAD;
                    break;

                case 'u':
                    serial_command = CMD_TEST_SEQNS;
                    cmd_messages = PAYLOAD;
                    break;

                default:
                    sprintf(buf, "#NACK\n");
                    serial_command = NO_CMD;
                    cmd_messages = START;
                    break;
                }
                break;

            case PAYLOAD:
                if (serial_command == CMD_SEQIDX)
                {
                    if ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'))
                    {
                        if (c >= '0' && c <= '9') {
                            c = c - 48;
                        } else {
                            c = c - 87;
                        }

                        if (sequence_digit == 0)
                        {
                            sequence_index = c;
                            sequence_digit = 1;
                            cmd_messages = PAYLOAD;
                            sprintf(buf, "");
                        }
                        else
                        {
                            sequence_index = (sequence_index << 4) | c;
                            display_hex(sequence_index);
                            sequence_digit = 0;
                            cmd_messages = START;
                            serial_command = NO_CMD;
                            sprintf(buf, "#ACK\n");
                        }
                    }
                    else
                    {
                        cmd_messages = START;
                        serial_command = NO_CMD;
                        sprintf(buf, "#NACK\n");
                    }
                    break;
                }
                else if (serial_command == CMD_TEST_SEQ)
                {
                    break;
                }
                else if (serial_command == CMD_TEST_SEQNS)
                {
                    break;
                }
            }

            const char *ptr = buf;
            while (*ptr != '\0')
            {
                while (!(USART0.STATUS & USART_DREIF_bm))
                    ;
                USART0.TXDATAL = *ptr;
                ptr++;
            }
        }

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
            else if ((pb_falling & PIN7_bm) || (serial_command == CMD_SEQ))
            {
                // S4 pressed or CMD_SEQ executed
                sequence_index_2 = sequence_index;
                read_sequence_counter = 0;
                read_sequence();
                sequence_mode_1 = EXECUTE_SEQUENCE;
                sequence_mode_2 = 0;
                sequence_state = 0;
                timer_counter = 0;
                cmd_messages = START;
                serial_command = NO_CMD;
            }
            break;

        case EXECUTE_SEQUENCE:
            if (sequence_mode_3 == 1)
            {
                sequence_mode_1 = PAUSE_SEQUENCE;
                sequence_mode_3 = 0;
                break;
            }

            if ((pb_falling & PIN7_bm) || (serial_command == CMD_PAUSE))
            {
                // S4 pressed or CMD_PAUSE executed
                paused_sequence_state = sequence_state;
                sequence_mode_1 = PAUSE_SEQUENCE;
                cmd_messages = START;
                serial_command = NO_CMD;
            }

            if (sequence_mode_2 == 2)
            {
                sequence_index = sequence_index_2;
                display_hex(sequence_index);
                sequence_mode_1 = SELECT_SEQUENCE;
                sequence_mode_2 = 0;
            }

            break;

        case PAUSE_SEQUENCE:
            if (paused_sequence_state == 0)
            {
                sequence_state = paused_sequence_state;
                timer_counter = 0;
            }
            else if (paused_sequence_state == 0 && read_sequence_counter > 0)
            {
                TCA0.SINGLE.PERBUF = final_perbuf;
                TCA0.SINGLE.CMP1BUF = final_cmp1buf;
                TCA0.SINGLE.CMP0BUF = final_cmp0buf;
            }
            else
            {
                sequence_state = paused_sequence_state - 1;
                timer_counter = duration_array[paused_sequence_state - 1];
            }

            if ((pb_falling & PIN5_bm) || (serial_command == CMD_EXIT))
            {
                // S2 pressed or CMD_EXIT executed
                sequence_state = 8;
                sequence_mode_1 = SELECT_SEQUENCE;
                sequence_mode_2 = 0;
                cmd_messages = START;
                serial_command = NO_CMD;
            }
            else if ((pb_falling & PIN6_bm) || (serial_command == CMD_STEP))
            {
                // S3 pressed or CMD_STEP executed
                if (paused_sequence_state <= 7)
                {
                    paused_sequence_state++;
                }
                else if (paused_sequence_state == 8)
                {
                    sequence_state = 7;
                    timer_counter = duration_array[7];
                }

                if (duration_array[paused_sequence_state] == 0)
                {
                    sequence_state = 8;
                    sequence_mode_1 = EXECUTE_SEQUENCE;
                    sequence_mode_2 = 2;
                    cmd_messages = START;
                    serial_command = NO_CMD;
                    break;
                }

                sequence_mode_1 = EXECUTE_SEQUENCE;
                sequence_mode_2 = 0;
                sequence_mode_3 = 1;
                cmd_messages = START;
                serial_command = NO_CMD;
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