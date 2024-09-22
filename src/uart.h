#ifndef TEST_PLATFORMIO_UART_H
#define TEST_PLATFORMIO_UART_H

#include <avr/io.h>
#include <stdio.h>

int uart_putchar (char c, FILE* file) {
    if (c == '\n')
        uart_putchar('\r', file);
    loop_until_bit_is_set (UCSR0A, UDRE0);
    UDR0 = c;
    return 0;
}

int uart_getchar (FILE* file) {
    while (!(UCSR0A & (0x01 << RXC0)));
    return UDR0;
}

void USART_Init( unsigned int ubrr ) {
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (1<<URSEL0)|(1<<USBS0)|(3<<UCSZ00);

    fdevopen(uart_putchar, uart_getchar);
}

#endif //TEST_PLATFORMIO_UART_H
