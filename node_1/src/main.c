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

#define BAUD 9600
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
    menu_add_sub_menu(&main_menu, "Sub menu 2", NULL);

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

    uint32_t i = 0;
    while(1) {
        adc_update(&adc);
        joy_stick_update(&adc, &joy_stick);
        slider_update(&adc, &slider);

        menu_update(&current_menu, &joy_stick, &current_menu_index);
        menu_print_current_menu(&oled, &current_menu, current_menu_index);

        oled_display_buffer(&oled);
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

        i++;
        if (i % 100 == 0) {
            printf("%s: %d -> Sent 100 messages\n\r", __FILE__, __LINE__);
            printf("%s: %d -> Button pressed: %s\n\r", __FILE__, __LINE__, joy_stick.button.is_pressed ? "true" : "false");
        }

        can_send(&can_message.message);
        _delay_ms(10);
    }
}
