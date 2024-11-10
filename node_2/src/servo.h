#ifndef NODE_2_SERVO_H
#define NODE_2_SERVO_H

#include <stdint.h>
#include "pwm.h"

struct Servo {
    struct Pwm_config pwm_config;
    uint32_t min_duty_cycle;
    uint32_t max_duty_cycle;
};

void servo_init(struct Servo *servo, uint8_t pin, uint8_t channel, uint32_t frequency, uint32_t min_duty_cycle, uint32_t max_duty_cycle);
void servo_set_angle(struct Servo *servo, uint32_t angle);


#endif //NODE_2_SERVO_H
