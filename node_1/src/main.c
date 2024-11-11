#define F_CPU 4915200UL
#define FOSC 4915200UL

#include <avr/io.h>
#include <util/delay.h>

#include "peripherals/input/adc.h"
#include "peripherals/uart/uart.h"
#include "peripherals/oled/sram.h"
#include "peripherals/oled/oled.h"
#include "peripherals/input/button.h"
#include "peripherals/input/joystick.h"
#include "peripherals/input/slider.h"
#include "peripherals/can/can_controller.h"
#include "peripherals/can/can.h"
#include "menu/menu.h"

#include "../../shared/can_messages.h"

#define BAUD 4800
#define MYUBRR (FOSC/16/BAUD-1)

int main() {
    USART_Init(MYUBRR);
    EXMEM_init();

    printf("test\n");

    struct Adc adc = {};
    struct JoyStick joy_stick = {};
    struct Slider slider = {};
    adc_setup(&adc);

    _delay_ms(100);

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

    _delay_ms(100);
    printf("\n");
    printf("start program: \n");

    can_init();
    printf("init finished: \n");
    mcp_set_mode(MODE_NORMAL);
    printf("mode: %x\r\n", mcp_read(MCP_CANSTAT));



    // Sender melding
    struct Message message = {
            1, // Id
            6, // Lengde på dataen
            "heiiii" // Data. Maks åtte byte
            };

    // printf("Sending message -> \n");
    // can_send(&message); // Sender melding

    // Nå er meldingen sendt. Fordi vi er i loopbackmodus blir meldingen umiddelbart "mottatt" ac MCP2515.

    // Mottar melding
    /*
    struct Message receive = can_receive();
    printf("Heisann sveisann, vi har fått ei melding.\r\n");
    printf("Id: %d \r\n", receive.id);
    printf("Lengde: %d \r\n", receive.length);
    printf("Melding: %s \r\n\r\n", receive.data);
     */

    while(1) {
        adc_update(&adc);
        joy_stick_update(&adc, &joy_stick);
        print_joy_stick_info(&joy_stick);
        slider_update(&adc, &slider);

        menu_update(&current_menu, &joy_stick, &current_menu_index);
        menu_print_current_menu(&oled, &current_menu, current_menu_index);
        oled_display_buffer(&oled);

        printf("\n");
        oled_clear(&oled);

        struct JoyStickMessage joyStickMessage = {
                .id = JOYSTICK_ID,
                .length = 8,
                .x = joy_stick.current_voltage[0],
                .y = joy_stick.current_voltage[1],
                .distance = joy_stick.current_distance,
                .angle = joy_stick.current_angle,
                .direction = joy_stick.current_direction,
                .button_pressed = joy_stick.button.is_pressed
        };


        struct CanMessage can_message = {
                .joyStickMessage = joyStickMessage
        };

        // print_joy_stick_message(&can_message.joyStickMessage);

        printf("Sending message -> \n");
        can_send(&can_message.message);

        _delay_ms(1000);
    }
}

/*
void print_joy_stick_message(struct JoyStickMessage *message) {
    printf("%s: %d -> Joystick message: \n\r", __FILE__, __LINE__);
    printf("%s: %d -> \tID: %d \n\r", __FILE__, __LINE__, message->id);
    printf("%s: %d -> \tLength: %d \n\r", __FILE__, __LINE__, message->length);
    printf("%s: %d -> \tX: %d \n\r", __FILE__, __LINE__, message->x);
    printf("%s: %d -> \tY: %d \n\r", __FILE__, __LINE__, message->y);
    printf("%s: %d -> \tDistance: %d \n\r", __FILE__, __LINE__, message->distance);
    printf("%s: %d -> \tAngle: %d \n\r", __FILE__, __LINE__, message->angle);
    printf("%s: %d -> \tDirection: %d \n\r", __FILE__, __LINE__, message->direction);
    printf("%s: %d -> \tButton pressed: %s \n\r", __FILE__, __LINE__, message->button_pressed ? "true" : "false");
}
*/
