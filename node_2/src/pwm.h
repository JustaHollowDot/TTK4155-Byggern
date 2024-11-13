#ifndef NODE_2_PWM_H
#define NODE_2_PWM_H

#include <stdint.h>
#include "sam/sam3x/include/sam.h"
#include "sam/sam3x/source/system_sam3x.h"

struct Pwm_config {
    uint8_t pin;
    uint8_t channel;
    uint32_t frequency;
    uint32_t duty_cycle;
};

void pwm_test_2();
void pwm_test_init();
void pwm_init(struct Pwm_config *pwm_config);
void pwm_set_duty_cycle(struct Pwm_config *pwm_config, uint32_t duty_cycle);
uint32_t pwm_get_duty_cycle(struct Pwm_config *pwm_config);



#endif //NODE_2_PWM_H
