#include <stdbool.h>
#include "button.h"

void button_init(struct Button *button, volatile uint8_t* port_direction_register) {
    *port_direction_register &= ~(1 << button->pin);
}

void button_update(struct Button *button) {
    button->is_pressed = *button->port_pin_register & (1 << button->pin);
    if (button->is_inverted) {
        button->is_pressed = !button->is_pressed;
    }
}