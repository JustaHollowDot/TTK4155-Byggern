#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "sram.h"
#include "adc.h"
#include "oled.h"
#include "button.h"

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();
    buttons_init();

    printf("\n");
    printf("a\n");

    _delay_ms(100);

    struct Adc adc = {};
    struct Joy_stick joy_stick = {};
    struct Sliders sliders = {};
    adc_setup(&adc, &joy_stick, &sliders);

    printf("\n");
    struct Oled oled = {};
    oled_init();

    printf("c");

    while(1) {
        oled_write_char(/*&oled, */'a');
        printf("a");
        // adc_update(&adc, &joy_stick, &sliders);

        // print_adc_info(&adc, &joy_stick, &sliders);

        _delay_ms(1000);
    }
}
