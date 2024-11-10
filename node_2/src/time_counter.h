#ifndef NODE_2_TIME_COUNTER_H
#define NODE_2_TIME_COUNTER_H

#define F_CPU 84000000

#include <stdint.h>

#include "time.h"

struct TimeCounter {
    uint8_t pin;
    uint8_t channel;
    uint64_t ra;
    uint64_t rb;
    uint64_t rc;
};

void tc_init();
void time_counter_init(struct TimeCounter *time_counter);
void time_counter_set_duty_cycle(struct TimeCounter *time_counter, float duty_cycle);
void time_counter_set_frequency(struct TimeCounter *time_counter, uint64_t frequency);
uint32_t time_counter_get_duty_cycle(struct TimeCounter *time_counter);

#endif //NODE_2_TIME_COUNTER_H
