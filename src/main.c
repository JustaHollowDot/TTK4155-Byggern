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
#include "menu/menu.h"
#include "peripherals/can/can_controller.h"

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();


    // struct Adc adc = {};
    // struct JoyStick joy_stick = {};
    // struct Slider slider = {};
    // adc_setup(&adc);

    _delay_ms(100);

    /*
    joy_stick_setup(&joy_stick);
    joy_stick_update(&adc, &joy_stick);
    joy_set_stick_center_voltages(&adc, &joy_stick);
    slider_setup(&slider);

    struct Oled oled = {};

    oled_init(&oled, (uint8_t *) SRAM_START);
    oled_clear(&oled);
    oled_display_buffer(&oled);

    struct Menu main_menu = {};
    main_menu.text = "Main menu";

    menu_add_sub_menu(&main_menu, "Sub menu", NULL);

    struct Menu current_menu = main_menu;
    uint8_t current_menu_index = 0;
    */

    _delay_ms(100);
    printf("\n");
    printf("start program: \n");

    mcp_init();
    printf("init finished: \n");
    mcp_set_mode(MCP_LOOPBACK_MODE);
    printf("mode: %x\r\n", mcp_read(MCP_CANSTAT));

    printf("Starting loop: \n");

    printf("Sending message -> ");
    mcp_write(MCP_TXB0SIDH, 0xA7); // Skriver 0xA7 til sende-buffer nr. 0
    mcp_request_to_send(0); // Sender 0xA7 fra bufferen ut på CAN-bussen
    uint8_t byte = mcp_read(MCP_RXB0SIDH); // Leser fra mottaksbuffer nr. 0
    printf("mottar: %x\r\n", byte); //Skal være samme som man sender, altså 0xA7

    _delay_ms(100000);

    while(1) {
        // adc_update(&adc);
        // joy_stick_update(&adc, &joy_stick);
        // slider_update(&adc, &slider);

        // menu_update(&current_menu, &joy_stick, &current_menu_index);
        // menu_print_current_menu(&oled, &current_menu, current_menu_index);
        // oled_display_buffer(&oled);


        _delay_ms(1000);

        // printf("\n");
        // oled_clear(&oled);
    }
}
