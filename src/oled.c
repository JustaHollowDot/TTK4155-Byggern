#define F_CPU 4915200UL

#include <util/delay.h>
#include <string.h>
#include "oled.h"
#include "oled_commands.h"

#include "fonts.h"

void oled_write_data(struct Oled *oled, uint8_t data) {
    oled->frame_buffer_addr[oled->current_page * SCREEN_WIDTH + oled->current_column] = data;

    oled->current_column++;

    if (oled->current_column >= SCREEN_WIDTH) {
        oled_increment_page(oled);
    }
}

void oled_display_buffer(struct Oled *oled) {
    uint8_t previous_page = oled->current_page;
    uint8_t previous_index = oled->current_column;

    oled_goto_pos(oled, 0, 0);

    for (int i = 0; i < PAGE_AMOUNT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            uint8_t current_byte = oled->frame_buffer_addr[i * SCREEN_WIDTH + j];
            oled_write_byte(current_byte);
        }
    }

    oled_goto_pos(oled, previous_page, previous_index);
}

void oled_write_char(struct Oled *oled, char c) {
    if (oled->current_column + FONT_SIZE > SCREEN_WIDTH) {
        oled_increment_page(oled);
    }

    for (int i = 0; i < FONT_SIZE; ++i) {
        uint8_t byte = pgm_read_byte(&(font5[c - 32][i]));
        oled_write_data(oled, byte);
    }
}

void oled_init(struct Oled *oled) {
    oled_init_commands();
    oled_clear(oled);

    oled->frame_buffer_addr = (uint8_t*) SRAM_START;
    oled->frame_size = 1024;
}

void oled_init_commands() {
    set_display_on(false);

    set_segment_remap(true);
    set_com_pins(true, false);
    set_invert_com_output_scan_direction(true);
    set_multiplex_ratio(0x3F);
    set_display_clock_divide_ratio_and_oscillator_frequency(0b0000, 0b1000);
    set_contrast(0x50);
    set_pre_charge_period(0b0001, 0b0010);
    set_memory_addressing_mode(0b10);
    set_v_comh_deselect_level(0b011);
    set_i_ref_selection(false);
    set_ignore_ram_content(false);
    set_invert_display(false);

    set_display_on(true);
}

void oled_reset(struct Oled *oled) {
    oled_init_commands();
    oled_clear(oled);
}

void oled_clear(struct Oled *oled) {
    oled_goto_pos(oled, 0, 0);

    for (int i = 0; i < PAGE_AMOUNT; ++i) {
        for (int j = 0; j < SCREEN_WIDTH; ++j) {
            oled_write_data(oled, 0);
        }
    }

    oled_goto_pos(oled, 0, 0);
}

void oled_clear_line(struct Oled *oled, uint8_t page) {
    uint8_t current_page = oled->current_page;
    uint8_t current_index = oled->current_column;

    oled_goto_page(oled, page);

    for (int j = 0; j < SCREEN_WIDTH; ++j) {
        oled_write_data(oled, 0);
    }

    oled_goto_pos(oled, current_page, current_index);
}

void oled_goto_page(struct Oled *oled, uint8_t page) {
    oled->current_page = page;
    oled->current_page %= PAGE_AMOUNT;

    set_page_start(page);
}

void oled_goto_column(struct Oled *oled, uint8_t column) {
    oled->current_column = column;
    oled->current_column %= SCREEN_WIDTH;

    uint8_t lower_nibble = column & 0b00001111;
    uint8_t upper_nibble = column & 0b11110000;
    upper_nibble = upper_nibble >> 4;

    set_lower_column(lower_nibble);
    set_higher_column(upper_nibble);
}

void oled_goto_pos(struct Oled *oled, uint8_t page, uint8_t column) {
    oled_goto_page(oled, page);
    oled_goto_column(oled, column);
}

void oled_increment_page(struct Oled *oled) {
    oled_goto_page(oled, oled->current_page + 1);
    oled_goto_column(oled, 0);
}

void oled_write_string(struct Oled *oled, char *str) {
    uint8_t str_len = strlen(str);

    for (int i = 0; i < str_len; ++i) {
        oled_write_char(oled, str[i]);
    }
}

void oled_write_line(struct Oled *oled, char *str) {
    oled_write_string(oled, str);
    oled_increment_page(oled);
}


