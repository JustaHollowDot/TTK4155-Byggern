#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"
#include "peripherals/sram.h"
#include "peripherals/adc.h"
#include "peripherals/oled.h"
#include "peripherals/button.h"

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

    while(1) {
        adc_update(&adc, &joy_stick, &sliders);

        print_adc_info(&adc, &joy_stick, &sliders);

        _delay_ms(1000);
    }
}
