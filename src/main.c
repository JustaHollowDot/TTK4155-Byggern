#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdbool.h>

#include "uart.h"
// #include "sram.h"
#include "peripherals/adc.h"

#define INTERNAL_MEMORY_END 0x4FF
#define OLED_COMMAND_START 0x1000
#define OLED_DATA_START 0x1200
#define ADC_START 0x1400
#define SRAM_START 0x1800

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

void EXMEM_init() {
    MCUCR = (1 << SRE); // Enable SRAM i ATmega162
    SFIOR = (1 << XMM2);
}

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();
    button_init();

    _delay_ms(100);

    struct Adc adc = {};
    struct Joy_stick joy_stick = {};
    joy_stick.adc_indexes[0] = 0;
    joy_stick.adc_indexes[1] = 1;

    struct Sliders sliders = {};
    sliders.adc_indexes[0] = 2;
    sliders.adc_indexes[1] = 3;

    printf("Getting center position on joystick");
    _delay_ms(500);

    get_new_adc_values(&adc, &joy_stick);
    set_center_voltages(&joy_stick, &adc);

    printf("Center voltages collected, continuing");

    while(1) {
        get_new_adc_values(&adc, &joy_stick);
        set_joy_stick_voltages(&adc, &joy_stick);
        set_slider_voltages(&adc, &sliders);

        set_min_max_voltages(&joy_stick, &adc);
        set_joy_stick_angle(&joy_stick);

        printf("angle: %d, distance: %d \n", (int) joy_stick.current_angle, get_joy_stick_distance_from_center(&joy_stick));
        printf("joy_stick_values: %02x : %02x \n", joy_stick.current_voltage[0], joy_stick.current_voltage[1]);
        printf("Sliders values: %02x : %02x \n", sliders.current_voltage[0], sliders.current_voltage[1]);

        _delay_ms(1000);
    }
}
