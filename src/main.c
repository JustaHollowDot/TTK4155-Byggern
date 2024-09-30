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

    _delay_ms(100);

    struct Adc adc = {};
    struct Joy_stick joy_stick = {};
    struct Sliders sliders = {};
    adc_setup(&adc, &joy_stick, &sliders);

    struct Oled oled = {};

    oled_init();

    oled_clear(&oled);

    while(1) {
        for (int i = 0; i < PAGE_AMOUNT; ++i) {
            oled_write_line(&oled, "test string");
            // adc_update(&adc, &joy_stick, &sliders);

            // print_adc_info(&adc, &joy_stick, &sliders);
            _delay_ms(1000);
        }

        oled_clear(&oled);
    }
}
