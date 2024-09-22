#ifndef TEST_PLATFORMIO_ADC_H
#define TEST_PLATFORMIO_ADC_H

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <stdbool.h>

struct Adc {
    volatile char voltages[4];
    char min_voltage_values[4];
    char max_voltage_values[4];
};

struct Joy_stick {
    unsigned int adc_indexes[2];
    char center_voltage[2];
    char max_voltages[2];
    char min_voltages[2];

    char current_voltage[2];
    float current_angle;
};

struct Sliders {
    char current_voltage[2];
    int current_position[2];
};

volatile char *adc_memory_position = (char *) 0x1400; // 0x1400 er start-adressen til ADC

void get_new_adc_values(struct Adc adc, struct Joy_stick joy_stick) {
    adc_memory_position[0] = 1;

    _delay_us(100);

    for (int i = 0; i < 4; i++) {
        volatile char value = adc_memory_position[i]; // Leser 8-bit data fra ADC

        printf("%02X", value); //Printer til seriell aka putty
        printf(" : ");

        adc.voltages[i] = value; // Leser 8-bit data fra ADC

        if (adc.voltages[i] > adc.max_voltage_values[i]) {
            adc.max_voltage_values[i] = adc.voltages[i];
        }

        if (adc.voltages[i] < adc.min_voltage_values[i]) {
            adc.min_voltage_values[i] = adc.voltages[i];
        }
    }

    printf("\n");

    joy_stick.current_voltage[0] = adc.voltages[joy_stick.adc_indexes[0]];
    joy_stick.current_voltage[1] = adc.voltages[joy_stick.adc_indexes[1]];
}


void set_center_voltages(struct Joy_stick joy_stick, struct Adc adc) {
    joy_stick.center_voltage[0] = adc.voltages[joy_stick.adc_indexes[0]];
    joy_stick.center_voltage[1] = adc.voltages[joy_stick.adc_indexes[1]];

}

void set_min_max_voltages(struct Joy_stick joy_stick, struct Adc adc) {
    joy_stick.max_voltages[0] = adc.max_voltage_values[joy_stick.adc_indexes[0]];
    joy_stick.max_voltages[1] = adc.max_voltage_values[joy_stick.adc_indexes[1]];

    joy_stick.min_voltages[0] = adc.min_voltage_values[joy_stick.adc_indexes[0]];
    joy_stick.min_voltages[1] = adc.min_voltage_values[joy_stick.adc_indexes[1]];
}


void set_joy_stick_angle(struct Joy_stick joy_stick) {
    double delta_x = joy_stick.current_voltage[0] - joy_stick.center_voltage[0];
    double delta_y = joy_stick.current_voltage[1] - joy_stick.center_voltage[1];

    // scale x and y to a number between 0 and 100
    if (delta_x > 0) {
        delta_x = delta_x / joy_stick.max_voltages[0];
    } else {
        delta_x = delta_x / joy_stick.min_voltages[0];
    }

    if (delta_y > 0) {
        delta_y = delta_y / joy_stick.max_voltages[1];
    } else {
        delta_y = delta_y / joy_stick.min_voltages[1];
    }

    joy_stick.current_angle = (atan2(delta_x, delta_y) * 57);
}

int get_joy_stick_distance_from_center(struct Joy_stick joy_stick) {
    int distance = 0;

    // todo!

    return distance;
}

void button_init() {
    DDRE &= ~(1 << PE2);
    DDRE &= ~(1 << PE0);
}

void read_adc() {
    adc_memory_position[0] = 1;

    _delay_us(100);

    printf("ADC-verdi: ");

    for (int i = 0; i < 4; i++) {
        volatile uint8_t value = adc_memory_position[i]; // Leser 8-bit data fra ADC

        printf("%X %02X", i, value); //Printer til seriell aka putty
        printf(" : ");
    }

    printf("\n");

    bool button_joystick = PINE & 1 << PINE2;
    printf("Knapp: %X", button_joystick);
    printf("\n");
}

#endif //TEST_PLATFORMIO_ADC_H