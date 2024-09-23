#include "adc.h"

#define ADC_START 0x1400
#define ADC_INDEX_0 0
#define ADC_INDEX_1 1
#define SLIDER_INDEX_0 2
#define SLIDER_INDEX_1 3

volatile char *adc_memory_position = (char *) ADC_START; // 0x1400 er start-adressen til ADC

void adc_setup(struct Adc *adc, struct Joy_stick *joy_stick, struct Sliders *sliders) {
    joy_stick->adc_indexes[0] = ADC_INDEX_0;
    joy_stick->adc_indexes[1] = ADC_INDEX_1;

    sliders->adc_indexes[0] = SLIDER_INDEX_0;
    sliders->adc_indexes[1] = SLIDER_INDEX_1;

    printf("Getting center position on joystick");
    _delay_ms(500);

    get_new_adc_values(adc);
    set_center_voltages(joy_stick, adc);

    printf("Center voltages collected, continuing");
}

void adc_update(struct Adc *adc, struct Joy_stick *joy_stick, struct Sliders *sliders) {

    get_new_adc_values(adc);
    set_joy_stick_voltages(adc, joy_stick);
    set_slider_voltages(adc, sliders);

    set_min_max_voltages(joy_stick, adc);
    set_joy_stick_angle(joy_stick);
}


void get_new_adc_values(struct Adc *adc) {
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

void set_joy_stick_voltages(struct Adc *adc, struct Joy_stick *joy_stick) {
    joy_stick->current_voltage[0] = adc->voltages[joy_stick->adc_indexes[0]];
    joy_stick->current_voltage[1] = adc->voltages[joy_stick->adc_indexes[1]];
}

void set_slider_voltages(struct Adc *adc, struct Sliders *sliders) {
    sliders->current_voltage[0] = adc->voltages[sliders->adc_indexes[0]];
    sliders->current_voltage[1] = adc->voltages[sliders->adc_indexes[1]];
}


void set_center_voltages(struct Joy_stick *joy_stick, struct Adc *adc) {
    joy_stick->center_voltage[0] = adc->voltages[joy_stick->adc_indexes[0]];
    joy_stick->center_voltage[1] = adc->voltages[joy_stick->adc_indexes[1]];
}

void set_min_max_voltages(struct Joy_stick *joy_stick, struct Adc *adc) {
    joy_stick->max_voltages[0] = adc->max_voltage_values[joy_stick->adc_indexes[0]];
    joy_stick->max_voltages[1] = adc->max_voltage_values[joy_stick->adc_indexes[1]];

    joy_stick->min_voltages[0] = adc->min_voltage_values[joy_stick->adc_indexes[0]];
    joy_stick->min_voltages[1] = adc->min_voltage_values[joy_stick->adc_indexes[1]];
}


void set_joy_stick_angle(struct Joy_stick *joy_stick) {
    double delta_x = joy_stick->current_voltage[0] - joy_stick->center_voltage[0];
    double delta_y = joy_stick->current_voltage[1] - joy_stick->center_voltage[1];

    /*
    // scale deltas based on the highest recorded value
    if (delta_x > 0) {
        delta_x = delta_x / joy_stick->max_voltages[0];
    } else {
        delta_x = delta_x / joy_stick->min_voltages[0];
    }

    if (delta_y > 0) {
        delta_y = delta_y / joy_stick->max_voltages[1];
    } else {
        delta_y = delta_y / joy_stick->min_voltages[1];
    }
    */

    joy_stick->current_angle = (float) (atan2(delta_y, delta_x) * 57);
}

int get_joy_stick_distance_from_center(struct Joy_stick *joy_stick) {
    int delta_x = joy_stick->current_voltage[0] - joy_stick->center_voltage[0];
    int delta_y = joy_stick->current_voltage[1] - joy_stick->center_voltage[1];

    int sq_sum = delta_x * delta_x + delta_y * delta_y;

    return (int) sqrt(sq_sum);
}

__attribute__((unused))
void print_adc_info(struct Adc *adc, struct Joy_stick *joy_stick, struct Sliders *sliders) {

    printf("angle: %d, distance: %d \n", (int) joy_stick->current_angle, get_joy_stick_distance_from_center(joy_stick));
    printf("joy_stick_values: %02x : %02x \n", joy_stick->current_voltage[0], joy_stick->current_voltage[1]);
    printf("Sliders values: %02x : %02x \n", sliders->current_voltage[0], sliders->current_voltage[1]);
}

