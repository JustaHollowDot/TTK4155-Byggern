#include "can_messages.h"
#include "stdio.h"

struct JoyStickMessage create_joy_stick_message(uint8_t x, uint8_t y, uint16_t distance, uint16_t angle, uint8_t direction, bool button_pressed) {
    struct JoyStickMessage message = {
            .id = JOYSTICK_ID,
            .length = 8,
            .x = x,
            .y = y,
            .distance = distance,
            .angle = angle,
            .direction = direction,
            .button_pressed = button_pressed
    };

    return message;
}

