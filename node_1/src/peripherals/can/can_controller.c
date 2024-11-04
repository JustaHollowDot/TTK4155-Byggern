#define F_CPU 4915200UL

#include "can_controller.h"
#include "spi.h"

void mcp_init() {
    spi_master_init();
    mcp_reset();

    _delay_ms(1);

    uint8_t value = mcp_read(MCP_CANSTAT); //Se kode for denne lenger ned
    if ((value & MODE_MASK) != MODE_CONFIG) {
        printf("MCP2515 er ikke i konfigurasjonsmodus etter reset. CANSTAT: %x \r\n", value);
    }

    uint8_t brp = 4 - 1;
    uint8_t phase_1 = 7 - 1;
    uint8_t phase_2 = 6 - 1;
    uint8_t prop_seg = 2 - 1;
    uint8_t sjw = 1 - 1;
    uint8_t blt = 1;
    uint8_t sam = 0;
    uint8_t sof = 0;
    uint8_t wakfil = 0;


    // Set up CNF1, CNF2, CNF3
    mcp_bit_modify(MCP_CNF1, 0b11111111, brp | (sjw << 3));
    mcp_bit_modify(MCP_CNF2, 0b11111111, phase_1 | (prop_seg << 3) | sam << 6 | blt << 7);
    mcp_bit_modify(MCP_CNF3, 0b11111111, phase_2 | wakfil << 6 | sof << 7);
}

void mcp_reset() {
    spi_begin_message();
    spi_write(MCP_RESET);
    spi_end_message();
}

uint8_t mcp_read(uint8_t address) {
    spi_begin_message();
    spi_write(MCP_READ);
    spi_write(address);
    uint8_t data = spi_read();
    spi_end_message();

    return data;
}

void mcp_write(uint8_t address, uint8_t data) {
    spi_begin_message();
    spi_write(MCP_WRITE);
    spi_write(address);
    spi_write(data);
    spi_end_message();
}

void mcp_request_to_send(uint8_t buffer) {
    spi_begin_message();
    buffer %= 3;
    uint8_t command = MCP_RTS_TX0;
    command &= ~(1);
    uint8_t current_buffer = 1 << buffer;
    spi_write(command + current_buffer);
    spi_end_message();
}

uint8_t mcp_read_status() {
    spi_begin_message();
    spi_write(MCP_READ_STATUS);
    uint8_t data = spi_read();
    spi_end_message();

    return data;
}

void mcp_bit_modify(uint8_t address, uint8_t mask, uint8_t data) {
    spi_begin_message();
    spi_write(MCP_BITMOD);
    spi_write(address);
    spi_write(mask);
    spi_write(data);
    spi_end_message();
}

void mcp_set_mode(uint8_t mode) {
    mcp_bit_modify(MCP_CANCTRL, 0b11100000, mode);
}