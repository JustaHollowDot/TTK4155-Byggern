#ifndef TEST_PLATFORMIO_SRAM_H
#define TEST_PLATFORMIO_SRAM_H

#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#define SRAM_START 0x1800

void EXMEM_init();
void SRAM_test(void);

#endif