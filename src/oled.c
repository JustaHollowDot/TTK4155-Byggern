#define F_CPU 4915200UL

#include <util/delay.h>
#include <stdio.h>
#include "oled.h"

#define OLED_COMMAND_START 0x1000
#define OLED_DATA_START 0x1200

#include "fonts.h"

// page addressing: 0b10, horizontal: 0b00, vertical: 0b01
#define ADDRESSING_MODE 0b10

volatile uint8_t *ext_oled_command_position = (uint8_t *) OLED_COMMAND_START;
volatile uint8_t *ext_oled_data_position = (uint8_t *) OLED_DATA_START;

void oled_init(/*struct Oled *oled*/) {
    oled_write_command(0xae); // display off
    oled_write_command(0xa1); //segment remap
    oled_write_command(0xda); //common pads hardware: alternative
    oled_write_command(0x12);
    oled_write_command(0xc8); //common output scan direction:com63~com0
    oled_write_command(0xa8); //multiplex ration mode:63
    oled_write_command(0x3f);
    oled_write_command(0xd5); //display divide ratio/osc. freq. mode
    oled_write_command(0x80);
    oled_write_command(0x81); //contrast control
    oled_write_command(0x50);
    oled_write_command(0xd9); //set pre-charge period
    oled_write_command(0x21);
    oled_write_command(0x20); //Set Memory Addressing Mode
    oled_write_command(0x02);
    oled_write_command(0xdb); //VCOM deselect level mode
    oled_write_command(0x30);
    oled_write_command(0xad); //master configuration
    oled_write_command(0x00);
    oled_write_command(0xa4); //out follows RAM content
    oled_write_command(0xa6); //set normal display
    oled_write_command(0xaf); // display on

    _delay_ms(1000);

    printf("blink!");
    oled_write_command(0xa5);
    _delay_ms(1000);

    oled_write_command(0xa4);
    _delay_ms(1000);


    oled_write_command(0x00 + 0b0000); // Lower nibble
    oled_write_command(0x10 + 0b0000); // Higher nibble
}

void oled_write_command(uint8_t command) {
    volatile uint8_t *ext_oled_command_position = (uint8_t *) OLED_COMMAND_START;
    ext_oled_command_position[0] = command;
}

void oled_write_byte(/*struct Oled *oled, */uint8_t data) {
    ext_oled_data_position[0] = data;

    /*
    uint8_t current_buffer = (oled->displayed_buffer + 1) % 2;
    uint8_t current_index = oled->current_index[current_buffer];

    if (current_index < SCREEN_WIDTH - 1) {
        oled->current_index[current_buffer]++;
    } else {
        oled->current_index[current_buffer] = 0;

        oled->current_page[current_buffer]++;
        oled->current_page[current_buffer] %= PAGE_AMOUNT;
    }
     */
}

void oled_write_char(/*struct Oled *oled, */char c) {
    for (int i = 0; i < FONT_SIZE; ++i) {
        uint8_t byte = pgm_read_byte(&(font5[c - 32][i]));
        oled_write_byte(/*oled, */byte);
    }
}
