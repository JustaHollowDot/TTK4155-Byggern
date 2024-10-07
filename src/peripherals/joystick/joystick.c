#include "joystick.h"

void joy_stick_setup(struct JoyStick *joy_stick) {
    joy_stick->adc_indexes[0] = ADC_INDEX_0;
    joy_stick->adc_indexes[1] = ADC_INDEX_1;

    joy_stick->min_voltages[0] = -1;
    joy_stick->min_voltages[1] = -1;
    joy_stick->max_voltages[0] = 0;
    joy_stick->max_voltages[1] = 0;

    joy_stick->button.port_pin_register = &BUTTON_PORT_PIN_REGISTER;
    joy_stick->button.pin = BUTTON_JOY_STICK_PIN;
    joy_stick->button.is_inverted = true;
    button_init(&joy_stick->button, &BUTTON_PORT_DIRECTION_REGISTER);
}

void joy_stick_update(struct Adc *adc, struct JoyStick *joy_stick) {
    joy_stick_set_voltages(adc, joy_stick);
    joy_stick_set_min_max_voltages(adc, joy_stick);
    joy_stick_set_angle(joy_stick);
    joy_stick_set_distance_from_center(joy_stick);
    joy_stick_set_direction(joy_stick);

    button_update(&joy_stick->button);
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
    float delta_x = joy_stick->current_voltage[0] - joy_stick->center_voltage[0];
    float delta_y = joy_stick->current_voltage[1] - joy_stick->center_voltage[1];

    /*
    // scale deltas based on the highest recorded value
    if (delta_x > 0) {
        float max_delta = (float) joy_stick->max_voltages[0] - joy_stick->center_voltage[0];
        delta_x = delta_x / max_delta;
    } else {
        float max_delta = (float) joy_stick->min_voltages[1] - joy_stick->center_voltage[1];
        delta_x = delta_x / max_delta;
    }

    if (delta_y > 0) {
        float max_delta = (float) joy_stick->max_voltages[1] - joy_stick->center_voltage[1];
        delta_y = delta_y / max_delta;
    } else {
        float max_delta = (float) joy_stick->min_voltages[1] - joy_stick->center_voltage[1];
        delta_y = delta_y / max_delta;
    }
     */

    joy_stick->current_angle = (uint16_t) (atan2f(delta_y, delta_x) * 57) + 180;
}

void joy_stick_set_distance_from_center(struct JoyStick *joy_stick) {
    float delta_x = joy_stick->current_voltage[0] - joy_stick->center_voltage[0];
    float delta_y = joy_stick->current_voltage[1] - joy_stick->center_voltage[1];

    float sq_sum = delta_x * delta_x + delta_y * delta_y;

    joy_stick->current_distance = (uint32_t) sqrt(sq_sum);
}


__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_joy_stick_info(struct JoyStick *joy_stick) {
    printf("Angle: %d, distance: %d \n", (int) joy_stick->current_angle, (int) joy_stick->current_distance);
    printf("Joy stick values: %02x : %02x \n", joy_stick->current_voltage[0], joy_stick->current_voltage[1]);
    printf("Button pressed: %s\n", joy_stick->button.is_pressed ? "true" : "false");
}

void joy_stick_set_direction(struct JoyStick *joy_stick) {
    if (joy_stick->current_distance < 50) {
        joy_stick->current_direction = CENTER;
        return;
    }

    if (joy_stick->current_angle < 45 || joy_stick->current_angle > 315) {
        joy_stick->current_direction = DOWN;
    } else if (joy_stick->current_angle < 135) {
        joy_stick->current_direction = LEFT;
    } else if (joy_stick->current_angle < 225) {
        joy_stick->current_direction = UP;
    } else {
        joy_stick->current_direction = RIGHT;
    }
}
