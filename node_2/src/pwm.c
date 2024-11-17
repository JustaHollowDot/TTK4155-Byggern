#include "pwm.h"
#include "stdio.h"

#define PASSWRD_PWM 0x50574dfc
#define PASSWD_PMC 0x504d4300
#define PASSWD_PIO 0x50494f00
#define DC_LO 2650
#define DC_HI 5300

uint32_t last_dc;

void pwm_test_2() {
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PWM power on , see datasheet page 38
    PWM->PWM_DIS = PWM_DIS_CHID0;                         // Disable PWM channel 0
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;                    // PIOA power on

    PIOB->PIO_PDR |= PIO_PDR_P12;                         // The GPIO don't drive the pin, this is the peripheral
    PIOB->PIO_ABSR |= PIO_ABSR_P12;                       // Set PWM pin perhipheral type B , datasheet page 974

    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA;      // The period is left aligned, clock source as CLKA on channel 0
    PWM->PWM_CH_NUM[0].PWM_CPRD = 4000;                  // Channel 0 : Set the PWM frequency (84MHz/42)/PWM_CPRD = 100KHz ;
    PWM->PWM_CH_NUM[0].PWM_CDTY = 0;                     // Channel 0: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % = 50%;

    PWM->PWM_ENA = PWM_ENA_CHID0;
}

void pwm_set_duty_cycle(struct Pwm_config *pwm_config, uint32_t duty_cycle) {
    REG_PWM_SCM &= ~PWM_SCM_UPDM_MODE0;

    REG_PWM_SCM |= PWM_SCM_SYNC0;
    REG_PWM_ENA |= PWM_ENA_CHID0;

    REG_PWM_CDTYUPD0 = duty_cycle;

    REG_PWM_SCUC |= PWM_SCUC_UPDULOCK;
}

