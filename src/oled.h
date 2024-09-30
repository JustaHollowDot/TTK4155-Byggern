#ifndef BYGGERN_PING_PONG_OLED_H
#define BYGGERN_PING_PONG_OLED_H

#include "avr/io.h"
#include <stdbool.h>


#define SCREEN_WIDTH 128
#define PAGE_AMOUNT 8
#define FONT_SIZE 5

struct Oled {
    // Two circular buffers with 8 pages with 132 columns of 8 bits each.
    // uint8_t frame_buffers[2][PAGE_AMOUNT][SCREEN_WIDTH];

    // Allow for rotating display without rewriting the whole buffer
    uint8_t first_page[2];
    uint8_t first_index[2];

    uint8_t current_page[2];
    uint8_t current_index[2]; // Current index inside current_page

    uint8_t current_buffer;
    bool has_changed[2];
    // bool is_write_mode;
};

void oled_write_command(uint8_t command);
void oled_write_byte(struct Oled *oled, uint8_t data);

// todo! write more functions for different command functions
void oled_goto_page(struct Oled *oled, uint8_t page);
void oled_goto_column(struct Oled *oled, uint8_t column); // todo! implement different functions based on visual or text mode
void oled_goto_pos(struct Oled *oled, uint8_t page, uint8_t column);
void oled_goto_home(struct Oled *oled);
void oled_increment_page(struct Oled *oled);

void oled_init(/*struct Oled *oled*/);
void oled_reset(struct Oled *oled);
void oled_clear(struct Oled *oled);
void oled_clear_line(struct Oled *oled, uint8_t page);
void oled_display(struct Oled *oled);
void oled_rotate(struct Oled *oled, uint8_t vertical_amount, uint8_t horizontal_amount);

void oled_write_char(struct Oled *oled, char c);
void oled_write_string(struct Oled *oled, char *str);
void oled_write_line(struct Oled *oled, char *str);

#endif //BYGGERN_PING_PONG_OLED_H
