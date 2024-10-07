#ifndef BYGGERN_PING_PONG_JOYSTICK_H
#define BYGGERN_PING_PONG_JOYSTICK_H

#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../adc/adc.h"
#include "../button/button.h"

#define ADC_INDEX_0 0
#define ADC_INDEX_1 1
#define BUTTON_JOY_STICK_PIN PE2
#define BUTTON_PORT_PIN_REGISTER PINE
#define BUTTON_PORT_DIRECTION_REGISTER DDRE

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    CENTER
};

struct JoyStick {
    struct Button button;

    uint8_t adc_indexes[2];
    uint8_t center_voltage[2];
    uint8_t max_voltages[2];
    uint8_t min_voltages[2];

    uint8_t current_voltage[2];
    uint16_t current_distance;
    uint16_t current_angle;

    enum Direction current_direction;
};

void joy_stick_setup(struct JoyStick *joy_stick);
void joy_stick_update(struct Adc *adc, struct JoyStick *joy_stick);
void joy_stick_set_voltages(struct Adc *adc, struct JoyStick *joy_stick);
void joy_set_stick_center_voltages(struct Adc *adc, struct JoyStick *joy_stick);
void joy_stick_set_min_max_voltages(struct Adc *adc, struct JoyStick *joy_stick);
void joy_stick_set_angle(struct JoyStick *joy_stick);
void joy_stick_set_distance_from_center(struct JoyStick *joy_stick);
enum Direction joy_stick_get_direction(struct JoyStick *joy_stick);

__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_joy_stick_info(struct JoyStick *joy_stick);

#endif //BYGGERN_PING_PONG_JOYSTICK_H
