#define F_CPU 4915200UL

#include "spi.h"

void spi_master_init(void) {
    DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPIE);

    spi_end_message();
}

void spi_write(uint8_t cData) {
    SPDR = cData;

    while(!(SPSR & (1<<SPIF))) {}
}

uint8_t spi_read() {
    SPDR = 0xFF;

    while(!(SPSR & (1<<SPIF))) {}

    return SPDR;
}

void spi_begin_message() {
    PORTB &= ~(1 << DD_SS);
}

void spi_end_message() {
    PORTB |= (1 << DD_SS);
}
