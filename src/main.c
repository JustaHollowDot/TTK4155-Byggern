#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#include "peripherals/uart/uart.h"
#include "peripherals/sram/sram.h"
#include "peripherals/adc/adc.h"
#include "peripherals/oled/oled.h"
#include "peripherals/button/button.h"
#include "peripherals/joystick/joystick.h"
#include "peripherals/slider/slider.h"
#include "menu.h"

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

void print_test() {
    printf("test\n");
}

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();

    _delay_ms(100);

    struct Adc adc = {};
    struct JoyStick joy_stick = {};
    struct Slider slider = {};
    adc_setup(&adc);
    joy_stick_setup(&joy_stick);
    joy_set_stick_center_voltages(&adc, &joy_stick);
    slider_setup(&slider);

    struct Oled oled = {};

    oled_init(&oled, (uint8_t *) SRAM_START);
    oled_clear(&oled);
    oled_display_buffer(&oled);

    struct Menu main_menu = {};
    main_menu.text = "Main menu";

    menu_add_sub_menu(&main_menu, "Sub menu", print_test);
    menu_add_sub_menu(&main_menu, "Sub menu2", print_test);
    menu_add_sub_menu(&main_menu, "Sub menu3", print_test);

    struct Menu current_menu = main_menu;
    uint8_t current_menu_index = 0;

    _delay_ms(100);

    while(1) {
        adc_update(&adc);
        joy_stick_update(&adc, &joy_stick);
        slider_update(&adc, &slider);

        print_adc_info(&adc);
        print_joy_stick_info(&joy_stick);
        print_slider_info(&slider);
        printf("\n");

        if (joy_stick.button.is_pressed) {
            printf("Button pressed, menu index: %d\n", current_menu_index);
        }

        menu_update(&current_menu, &joy_stick, &current_menu_index);
        menu_print_current_menu(&oled, &current_menu, current_menu_index);
        oled_display_buffer(&oled);

        _delay_ms(1000);


        // for (int i = 0; i < PAGE_AMOUNT; ++i) {
            // oled_write_line(&oled, "Loop finished");
        // }

        oled_clear(&oled);
    }
}
