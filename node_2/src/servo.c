#include "servo.h"

void servo_init(struct Servo *servo, uint8_t pin, uint8_t channel, uint32_t frequency, uint32_t min_duty_cycle, uint32_t max_duty_cycle) {
    struct Pwm_config pwm_config = {
            .pin = pin,
            .channel = channel,
            .frequency = frequency,
            .duty_cycle = (min_duty_cycle + max_duty_cycle) / 2
    };

    pwm_init(&pwm_config);

    servo->pwm_config = pwm_config;
    servo->min_duty_cycle = min_duty_cycle;
    servo->max_duty_cycle = max_duty_cycle;
}

void servo_set_angle(struct Servo *servo, uint32_t angle) {
    if (angle > 180) {
        angle = 180;
    }

    uint32_t duty_cycle = servo->min_duty_cycle + (servo->max_duty_cycle - servo->min_duty_cycle) * angle / 180;
    pwm_set_duty_cycle(&servo->pwm_config, duty_cycle);
}
