#include "servo.h"

void servo_init(struct Servo *servo, uint8_t pin, uint8_t channel, uint64_t frequency, uint64_t min_duty_cycle, uint64_t max_duty_cycle) {
    struct TimeCounter time_counter = {
            .pin = pin,
            .channel = channel,
            .ra = 0,
            .rb = 0,
            .rc = 52500 // 20 ms
    };

    servo->time_counter = time_counter;
    servo->min_duty_cycle = min_duty_cycle;
    servo->max_duty_cycle = max_duty_cycle;
    servo->frequency = frequency;

    tc_init();
    time_counter_set_frequency(&time_counter, usecs(20000));
    float test_val = ((float) min_duty_cycle) / frequency;
    time_counter_set_duty_cycle(&time_counter, test_val);
}

void servo_set_angle(struct Servo *servo, uint32_t angle) {
    if (angle > 180) {
        angle = 180;
    }

    servo->angle = angle;

    float test_val = ((float) servo->angle) / 180;
    uint64_t added_duty_cycle = (servo->max_duty_cycle - servo->min_duty_cycle) * test_val;
    uint64_t duty_cycle = servo->min_duty_cycle + added_duty_cycle;

    if (duty_cycle > servo->max_duty_cycle) {
        printf("File: %s, Line: %d -> Error duty cycle is too high: \n\r", __FILE__, __LINE__);
        duty_cycle = servo->max_duty_cycle;
    } else if (duty_cycle < servo->min_duty_cycle) {
        printf("File: %s, Line: %d -> Error duty cycle is too low: \n\r", __FILE__, __LINE__);
        duty_cycle = servo->min_duty_cycle;
    }

    time_counter_set_duty_cycle(&servo->time_counter, ((float) duty_cycle) / servo->frequency);
}
