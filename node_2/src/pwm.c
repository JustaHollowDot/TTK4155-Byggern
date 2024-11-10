#include "pwm.h"

void pwm_init(struct Pwm_config *pwm_config) {
    REG_PMC_PCER1 |= PMC_PCER1_PID36;                     // Enable PWM
    REG_PIOB_ABSR |= PIO_ABSR_P16;                        // Set PWM pin perhipheral type A or B, in this case B
    REG_PIOB_PDR |= PIO_PDR_P16;                          // Set PWM pin to an output
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);     // Set the PWM clock rate to 2MHz (84MHz/42)
    REG_PWM_CMR0 = PWM_CMR_CALG | PWM_CMR_CPRE_CLKA;      // Enable dual slope PWM and set the clock source as CLKA
    REG_PWM_CPRD0 = pwm_config->frequency;                // Set the PWM frequency 2MHz/(2 * 20000) = 50Hz
    REG_PWM_CDTY0 = pwm_config->duty_cycle;               // Set the PWM duty cycle to 1500 - centre the servo
    REG_PWM_ENA = PWM_ENA_CHID0;                          // Enable the PWM channel

}

void pwm_set_duty_cycle(struct Pwm_config *pwm_config, uint32_t duty_cycle) {
    // Start manual write of duty cycle
    REG_PWM_SCM &= ~PWM_SCM_UPDM_MODE0;

    // Define the current channel to be updated
    REG_PWM_SCM |= PWM_SCM_SYNC0;

    // Enable the channels to be updated
    REG_PWM_ENA |= PWM_ENA_CHID0;

    // Write the new duty cycle
    REG_PWM_CDTYUPD0 = duty_cycle;

    // trigger update
    REG_PWM_SCUC |= PWM_SCUC_UPDULOCK;
}

uint32_t pwm_get_duty_cycle(struct Pwm_config *pwm_config) {
    return 0;
}
