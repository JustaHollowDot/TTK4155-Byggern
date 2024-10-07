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


void adc_setup(struct Adc *adc);
void adc_update(struct Adc *adc);

void adc_get_new_values(struct Adc *adc);

__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_adc_info(struct Adc *adc);

#endif //TEST_PLATFORMIO_ADC_H