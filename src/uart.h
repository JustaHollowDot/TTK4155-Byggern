#ifndef TEST_PLATFORMIO_UART_H
#define TEST_PLATFORMIO_UART_H

#include <avr/io.h>
#include <stdio.h>

int uart_putchar(char c, FILE* file);
int uart_getchar(FILE* file);
void USART_Init(unsigned int ubrr);

#endif //TEST_PLATFORMIO_UART_H
