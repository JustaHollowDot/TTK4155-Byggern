#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uart.h"
#include "sram.h"
#include "adc.h"

#define INTERNAL_MEMORY_END 0x4FF
#define OLED_COMMAND_START 0x1000
#define OLED_DATA_START 0x1200
#define ADC_START 0x1400
#define SRAM_START 0x1800

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();


    volatile char *adc = (char *) 0x1400; // 0x1400 er start-adressen til ADC

    _delay_ms(100);

    DDRE &= ~(1 << PE0);
    DDRE &= ~(1 << PE2);

    printf("Klar til å måle: \n");

    while(1) {
        adc[0] = 1;

        _delay_ms(100);

        printf("ADC-verdi: ");

        for (int i = 0; i < 4; i++) {
            volatile uint8_t value = adc[i]; // Leser 8-bit data fra ADC

            printf("%X %02X", i, value); //Printer til seriell aka putty
            printf(" : ");
        }

        printf("\n");

        bool button_joystick = PINE & 1 << PINE2;
        printf("Knapp: %X", button_joystick);
        printf("\n");

        _delay_ms(1000); //Bare sånn at det ikke blir helt kaos i putty
    }
}
