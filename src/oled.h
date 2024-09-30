#ifndef BYGGERN_PING_PONG_OLED_H
#define BYGGERN_PING_PONG_OLED_H

#include "avr/io.h"
#include <stdbool.h>

#include "sram.h"

#define SCREEN_WIDTH 128
#define PAGE_AMOUNT 8
#define FONT_SIZE 5


struct FrameBuffer {
    // Buffer with 8 pages with 132 columns of 8 bits each.
    uint8_t frame_buffer[PAGE_AMOUNT][SCREEN_WIDTH];
};

struct Oled {
    // Buffer with 8 pages with 132 columns of 8 bits each.
    volatile uint8_t* frame_buffer_addr; // Start address for the two frames
    uint16_t frame_size;

    uint8_t current_page;
    uint8_t current_column;

    // Allow for rotating display without rewriting the whole buffer
    uint8_t first_page;
    uint8_t first_index;

};

void oled_write_data(struct Oled *oled, uint8_t data);
void oled_display_buffer(struct Oled *oled);

// todo! write more functions for different command functions
void oled_goto_page(struct Oled *oled, uint8_t page);
void oled_goto_column(struct Oled *oled, uint8_t column);
void oled_goto_pos(struct Oled *oled, uint8_t page, uint8_t column);
void oled_goto_home(struct Oled *oled);
void oled_increment_page(struct Oled *oled);

void oled_init(struct Oled *oled);
void oled_init_commands();
void oled_reset(struct Oled *oled);
void oled_clear(struct Oled *oled);
void oled_clear_line(struct Oled *oled, uint8_t page);
void oled_display(struct Oled *oled);
void oled_rotate(struct Oled *oled, uint8_t vertical_amount, uint8_t horizontal_amount);

void oled_write_char(struct Oled *oled, char c);
void oled_write_string(struct Oled *oled, char *str);
void oled_write_line(struct Oled *oled, char *str);

#endif //BYGGERN_PING_PONG_OLED_H
