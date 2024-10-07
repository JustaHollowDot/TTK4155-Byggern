#define F_CPU 4915200UL

#include "adc.h"

#define ADC_START 0x1400

volatile char *adc_memory_position = (char *) ADC_START; // 0x1400 er start-adressen til ADC

void adc_setup(struct Adc *adc) {
    for (int i = 0; i < 4; ++i) {
        adc->min_voltage_values[i] = -1;
        adc->max_voltage_values[i] = 0;
    }
}

void adc_update(struct Adc *adc) {
    adc_get_new_values(adc);
}

void adc_get_new_values(struct Adc *adc) {
    adc_memory_position[0] = 1;

    _delay_us(100);

    for (int i = 0; i < 4; i++) {
        volatile uint8_t value = adc_memory_position[i];

        adc->voltages[i] = value;

        if (adc->voltages[i] > adc->max_voltage_values[i]) {
            adc->max_voltage_values[i] = adc->voltages[i];
        }

        if (adc->voltages[i] < adc->min_voltage_values[i]) {
            adc->min_voltage_values[i] = adc->voltages[i];
        }
    }
}

/*
__attribute__((unused))
void print_adc_info(struct Adc *adc, struct JoyStick *joy_stick, struct Slider *sliders) {
    printf("angle: %d, distance: %d \n", (int) joy_stick->current_angle, get_joy_stick_distance_from_center(joy_stick));
    printf("joy_stick_values: %02x : %02x \n", joy_stick->current_voltage[0], joy_stick->current_voltage[1]);
    printf("Slider values: %02x : %02x \n", sliders->current_voltage[0], sliders->current_voltage[1]);
}
*/