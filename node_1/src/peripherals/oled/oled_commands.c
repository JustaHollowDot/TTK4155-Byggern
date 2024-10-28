#include <stdio.h>
#include "oled_commands.h"

#define OLED_COMMAND_START 0x1000
#define OLED_DATA_START 0x1200

volatile uint8_t *ext_oled_command_position = (uint8_t *) OLED_COMMAND_START;
volatile uint8_t *ext_oled_data_position = (uint8_t *) OLED_DATA_START;

void oled_write_command(uint8_t command) {
    ext_oled_command_position[0] = command;
}

void oled_write_byte(uint8_t byte) {
    ext_oled_data_position[0] = byte;
}

void set_lower_column(uint8_t lower_nibble) {
    lower_nibble = lower_nibble & 0b1111;
    oled_write_command(0x00 + lower_nibble);
}

void set_higher_column(uint8_t higher_nibble) {
    higher_nibble = higher_nibble >> 4;
    higher_nibble = higher_nibble & 0b1111;
    oled_write_command(0x10 + higher_nibble);
}

void set_memory_addressing_mode(uint8_t addressing_mode) {
    addressing_mode &= 0b11; // Removes bits unnecessary bits

    oled_write_command(0x20);

    if (addressing_mode == 0b10) {
        oled_write_command(0b00);
    } else if (addressing_mode == 0b01) {
        oled_write_command(0b01);
    } else {
        oled_write_command(0b10);
    }
}

void set_column_address(uint8_t start_address, uint8_t end_address) {
    oled_write_command(0x21);
    oled_write_command(start_address);
    oled_write_command(end_address);
}

void set_page_address(uint8_t start_page, uint8_t end_page) {
    oled_write_command(0x22);
    oled_write_command(start_page);
    oled_write_command(end_page);
}

void set_display_start_line(uint8_t start_line) {
    start_line &= 0b111111; // Removes bits used for command
    oled_write_command(0x40 + start_line);
}

void set_contrast(uint8_t contrast_level) {
    oled_write_command(0x81);
    oled_write_command(contrast_level);
}

void set_segment_remap(bool flip_address_space) {
    flip_address_space &= 0b1; // Removes bits used for command in case there is more bits set
    oled_write_command(0xA0 + flip_address_space);
}

void set_ignore_ram_content(bool turn_on_display) {
    turn_on_display &= 0b1; // Removes bits used for command in case there is more bits set
    oled_write_command(0xA4 + turn_on_display);
}

void set_invert_display(bool set_inverse_display) {
    set_inverse_display &= 0b1; // Removes bits used for command in case there is more bits set
    oled_write_command(0xA6 + set_inverse_display);
}

void set_multiplex_ratio(uint8_t mux_ratio) {
    mux_ratio &= 0b111111; // Removes unnecessary bits
    if (mux_ratio <= 14) { // Invalid entries, defaulting to lowest value
        mux_ratio = 0b1111;
    }

    oled_write_command(0xA8);
    oled_write_command(mux_ratio);
}

void set_i_ref_selection(bool is_internal_i_ref) {
    oled_write_command(0xAD);
    is_internal_i_ref &= 1; // Removes bits used for command in case there is more bits set
    oled_write_command(is_internal_i_ref << 4);
}

void set_display_on(bool is_on) {
    is_on &= 1; // Removes bits used for command in case there is more bits set
    oled_write_command(0xAE + is_on);
}

void set_page_start(uint8_t start_page) {
    start_page &= 0b111;
    oled_write_command(0xB0 + start_page);
}

void set_invert_com_output_scan_direction(bool invert_direction) {
    invert_direction &= 0b1; // Removes bits used for command in case there is more bits set
    oled_write_command(0xC0 + (invert_direction << 3));
}

void set_display_offset(uint8_t offset) {
    oled_write_command(0xD3);
    oled_write_command(offset);
}

void set_display_clock_divide_ratio_and_oscillator_frequency(uint8_t divide_ratio, uint8_t osc_frequency) {
    divide_ratio &= 0b1111;
    osc_frequency &= 0b1111;
    osc_frequency = osc_frequency << 4;

    oled_write_command(0xD5);
    oled_write_command(osc_frequency + divide_ratio);
}

void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2) {
    phase_1 &= 0b1111;
    phase_2 &= 0b1111;
    phase_2 = phase_2 << 4;

    oled_write_command(0xD9);
    oled_write_command(phase_2 + phase_1);
}

void set_com_pins(bool pin_configuration, bool left_right_remap) {
    pin_configuration &= 0b1; // Removes bits used for command in case there is more bits set
    left_right_remap &= 0b1; // Removes bits used for command in case there is more bits set

    uint8_t bit_map = (pin_configuration << 4) + (left_right_remap << 5) + 0b10;
    oled_write_command(0xDA);
    oled_write_command(bit_map);
}

void set_v_comh_deselect_level(uint8_t deselect_level) {
    deselect_level &= 0b111;
    deselect_level = deselect_level << 4;

    oled_write_command(0xDB);
    oled_write_command(deselect_level);
}

void nop() {
    oled_write_command(0xE3);
}



