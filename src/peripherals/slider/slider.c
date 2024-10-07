#include "slider.h"

void slider_setup(struct Slider *slider) {
    slider->adc_indexes[0] = SLIDER_INDEX_0;
    slider->adc_indexes[1] = SLIDER_INDEX_1;
}

void slider_update(struct Adc *adc, struct Slider *slider) {
    set_slider_voltages(adc, slider);
}

void set_slider_voltages(struct Adc *adc, struct Slider *slider) {
    slider->current_voltage[0] = adc->voltages[slider->adc_indexes[0]];
    slider->current_voltage[1] = adc->voltages[slider->adc_indexes[1]];
}

__attribute__((unused)) // Only used for debugging, attribute removes warning on intended usage
void print_slider_info(struct Slider *slider) {
    printf("Slider indexes -> %d : %d\n", slider->current_voltage[0], slider->current_voltage[1]);
}
