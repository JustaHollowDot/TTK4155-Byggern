#ifndef BYGGERN_PING_PONG_BUTTON_H
#define BYGGERN_PING_PONG_BUTTON_H

#include <avr/io.h>

// #define BUTTON_ PE0

struct Button {
    volatile uint8_t* port_pin_register;
    uint8_t pin;
    bool is_on;
};

void button_init(struct Button *button, volatile uint8_t* port_direction_register);
void button_update(struct Button *button);

#endif //BYGGERN_PING_PONG_BUTTON_H
