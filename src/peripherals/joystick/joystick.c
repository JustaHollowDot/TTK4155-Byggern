#include "joystick.h"

void joy_stick_setup(struct JoyStick *joy_stick) {
    joy_stick->adc_indexes[0] = ADC_INDEX_0;
    joy_stick->adc_indexes[1] = ADC_INDEX_1;

}

void joy_stick_update(struct Adc *adc, struct JoyStick *joy_stick) {
    joy_stick_set_voltages(adc, joy_stick);
    joy_stick_set_min_max_voltages(adc, joy_stick);
    joy_stick_set_angle(joy_stick);
    joy_stick_get_distance_from_center(joy_stick);
}

void joy_stick_set_voltages(struct Adc *adc, struct JoyStick *joy_stick) {
    joy_stick->current_voltage[0] = adc->voltages[joy_stick->adc_indexes[0]];
    joy_stick->current_voltage[1] = adc->voltages[joy_stick->adc_indexes[1]];
}


void joy_set_stick_center_voltages(struct Adc *adc, struct JoyStick *joy_stick) {
    printf("Getting center position on joystick");
    _delay_ms(500);

    adc_update(adc);

    joy_stick->center_voltage[0] = adc->voltages[joy_stick->adc_indexes[0]];
    joy_stick->center_voltage[1] = adc->voltages[joy_stick->adc_indexes[1]];

    printf("Center voltages collected, continuing");
}

void joy_stick_set_min_max_voltages(struct Adc *adc, struct JoyStick *joy_stick) {
    joy_stick->max_voltages[0] = adc->max_voltage_values[joy_stick->adc_indexes[0]];
    joy_stick->max_voltages[1] = adc->max_voltage_values[joy_stick->adc_indexes[1]];

    joy_stick->min_voltages[0] = adc->min_voltage_values[joy_stick->adc_indexes[0]];
    joy_stick->min_voltages[1] = adc->min_voltage_values[joy_stick->adc_indexes[1]];
}


void joy_stick_set_angle(struct JoyStick *joy_stick) {
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

int joy_stick_get_distance_from_center(struct JoyStick *joy_stick) {
    int delta_x = joy_stick->current_voltage[0] - joy_stick->center_voltage[0];
    int delta_y = joy_stick->current_voltage[1] - joy_stick->center_voltage[1];

    int sq_sum = delta_x * delta_x + delta_y * delta_y;

    return (int) sqrt(sq_sum);
}


/*
__attribute__((unused))
void print_adc_info(struct Adc *adc, struct JoyStick *joy_stick, struct Slider *sliders) {
    printf("angle: %d, distance: %d \n", (int) joy_stick->current_angle, get_joy_stick_distance_from_center(joy_stick));
    printf("joy_stick_values: %02x : %02x \n", joy_stick->current_voltage[0], joy_stick->current_voltage[1]);
    printf("Slider values: %02x : %02x \n", sliders->current_voltage[0], sliders->current_voltage[1]);
}
*/