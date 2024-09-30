#ifndef BYGGERN_PING_PONG_OLED_COMMANDS_H
#define BYGGERN_PING_PONG_OLED_COMMANDS_H

#include "avr/io.h"
#include <stdbool.h>


void oled_write_command(uint8_t command);
void oled_write_byte(uint8_t byte);

void set_lower_column(uint8_t lower_nibble);
void set_higher_column(uint8_t higher_nibble);
void set_memory_addressing_mode(uint8_t addressing_mode);
void set_column_address(uint8_t start_address, uint8_t end_address);
void set_page_address(uint8_t start_page, uint8_t end_page);
void set_display_start_line(uint8_t start_line);
void set_contrast(uint8_t contrast_level);
void set_segment_remap(bool flip_address_space);
void set_ignore_ram_content(bool turn_on_display);
void set_invert_display(bool set_inverse_display);
void set_multiplex_ratio(uint8_t mux_ratio);
void set_i_ref_selection(bool is_internal_i_ref);
void set_display_on(bool is_on);
void set_page_start(uint8_t start_page);
void set_invert_com_output_scan_direction(bool invert_direction);
void set_display_offset(uint8_t offset);
void set_display_clock_divide_ratio_and_oscillator_frequency(uint8_t divide_ratio, uint8_t osc_frequency);
void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2);
void set_com_pins(bool pin_configuration, bool left_right_remap);
void set_v_comh_deselect_level(uint8_t deselect_level);
void nop();



#endif //BYGGERN_PING_PONG_OLED_COMMANDS_H
