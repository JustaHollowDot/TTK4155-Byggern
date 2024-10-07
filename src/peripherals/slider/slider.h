#ifndef BYGGERN_PING_PONG_SLIDER_H
#define BYGGERN_PING_PONG_SLIDER_H

#define F_CPU 4915200UL

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

#include "../adc/adc.h"

#define SLIDER_INDEX_0 2
#define SLIDER_INDEX_1 3

struct Slider {
    uint8_t adc_indexes[2];
    uint8_t current_voltage[2];
    // uint8_t current_position[2];
};

void slider_setup(struct Slider *slider);
void slider_update(struct Adc *adc, struct Slider *slider);

void set_slider_voltages(struct Adc *adc, struct Slider *slider);

__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_slider_info(struct Slider *slider);

#endif //BYGGERN_PING_PONG_SLIDER_H
