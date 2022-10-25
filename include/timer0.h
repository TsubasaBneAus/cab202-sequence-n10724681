extern volatile uint8_t sequence_index;
extern volatile uint16_t timer_counter;
extern volatile uint8_t duration_array[8];
extern volatile uint16_t brightness_array[8];
extern volatile uint32_t buzzer_array[8];
extern volatile uint8_t octave_array[8];
extern volatile uint32_t cmp1buf_array[8];
extern volatile uint32_t final_perbuf;
extern volatile uint32_t final_cmp0buf;
extern volatile uint32_t final_cmp1buf;
extern volatile uint8_t sequence_state;
extern volatile uint8_t paused_sequence_state;
extern volatile uint8_t sequence_mode_2;
extern volatile uint8_t sequence_index_2;
extern volatile uint8_t read_sequence_counter;

void timer0_init();