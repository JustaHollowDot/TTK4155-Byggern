#ifndef NODE_2_SERVO_H
#define NODE_2_SERVO_H

#include <stdint.h>
#include "stdio.h"
#include "time_counter.h"

struct Servo {
    struct TimeCounter time_counter;
    uint64_t min_duty_cycle;
    uint64_t max_duty_cycle;
    uint64_t frequency;
    uint32_t angle;
};

void servo_init(struct Servo *servo, uint8_t pin, uint8_t channel, uint64_t frequency, uint64_t min_duty_cycle, uint64_t max_duty_cycle);
void servo_set_angle(struct Servo *servo, uint32_t angle);


#endif //NODE_2_SERVO_H
