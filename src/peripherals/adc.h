#ifndef TEST_PLATFORMIO_ADC_H
#define TEST_PLATFORMIO_ADC_H

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

struct Adc {
    volatile uint8_t voltages[4];
    uint8_t min_voltage_values[4];
    uint8_t max_voltage_values[4];
};

struct Joy_stick {
    uint8_t adc_indexes[2];
    uint8_t center_voltage[2];
    uint8_t max_voltages[2];
    uint8_t min_voltages[2];

    uint8_t current_voltage[2];
    float current_angle;
};

struct Sliders {
    uint8_t adc_indexes[2];
    uint8_t current_voltage[2];
    // uint8_t current_position[2];
};


void get_new_adc_values(struct Adc *adc);
void set_joy_stick_voltages(struct Adc *adc, struct Joy_stick *joy_stick);
void set_slider_voltages(struct Adc *adc, struct Sliders *sliders);
void set_center_voltages(struct Joy_stick *joy_stick, struct Adc *adc);
void set_min_max_voltages(struct Joy_stick *joy_stick, struct Adc *adc);
void set_joy_stick_angle(struct Joy_stick *joy_stick);
int get_joy_stick_distance_from_center(struct Joy_stick *joy_stick);
void button_init();

#endif //TEST_PLATFORMIO_ADC_H