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
