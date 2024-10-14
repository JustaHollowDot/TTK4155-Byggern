#include "can_controller.h"
#include "../spi/spi.h"

void can_controller_init() {
    spi_master_init();

    _delay_ms(1);
}

uint8_t can_controller_read(uint8_t address) {
    spi_begin_message();
    spi_write(0b11);
    spi_write(address);
    uint8_t data = spi_read();
    spi_end_message();

    return data;
}

void can_controller_write(uint8_t address, uint8_t data) {
    spi_begin_message();
    spi_write(0b10);
    spi_write(address);
    spi_write(data);
    spi_end_message();
}

void can_controller_request_to_send(uint8_t buffer) {
    spi_begin_message();
    buffer %= 3;
    uint8_t command = 0b10000000;
    uint8_t current_buffer = 1 << buffer;
    spi_write(command + current_buffer);
    spi_end_message();
}

uint8_t can_controller_read_status() {
    spi_begin_message();
    spi_write(0b10100000);
    uint8_t data = spi_read();
    spi_end_message();

    return data;
}

void can_controller_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    spi_begin_message();
    spi_write(0b101);
    spi_write(address);
    spi_write(mask);
    spi_write(data);
    spi_end_message();

}

void can_controller_reset() {
    spi_begin_message();
    spi_write(0b11000000);
    spi_end_message();
}

