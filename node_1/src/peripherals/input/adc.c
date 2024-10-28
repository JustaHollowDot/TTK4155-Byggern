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

__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_adc_info(struct Adc *adc) {
    printf("ADC voltages -> %02x : %02x : %02x : %02x\n",
           adc->voltages[0], adc->voltages[1],
           adc->voltages[2], adc->voltages[3]);
}