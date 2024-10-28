#ifndef BYGGERN_PING_PONG_KOPI_SPI_H
#define BYGGERN_PING_PONG_KOPI_SPI_H

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#define DDR_SPI DDRB
#define DD_SS PB4
#define DD_MOSI PB5
#define DD_MISO PB6
#define DD_SCK PB7


void spi_master_init();
void spi_write(uint8_t cData);
unsigned char spi_read();

void spi_begin_message();
void spi_end_message();

#endif //BYGGERN_PING_PONG_KOPI_SPI_H