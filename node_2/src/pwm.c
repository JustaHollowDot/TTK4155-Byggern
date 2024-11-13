#include "pwm.h"
#include "stdio.h"

#define PASSWRD_PWM 0x50574dfc
#define PASSWD_PMC 0x504d4300
#define PASSWD_PIO 0x50494f00
#define DC_LO 2650
#define DC_HI 5300

uint32_t last_dc;

void pwm_test_2() {
// Datasheet page 973
    // Select Instance=PWM; Signal=PWML2 (channel 2); I/O Line=PA20 (P20, Arduino pin 43, see pinout diagram) ; Peripheral=B
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PWM power on , see datasheet page 38

    PWM->PWM_DIS = PWM_DIS_CHID0;         // Disable PWM channel 2

    // Select Instance=PWM; Signal=PWML2 (channel 2); I/O Line=PA20 (P20, Arduino pin 43, see pinout diagram) ; Peripheral=B
    PMC->PMC_PCER0 |= PMC_PCER0_PID12;                    // PIOA power on

    PIOB->PIO_PDR |= PIO_PDR_P12;                         // The GPIO don't drive the pin, this is the peripheral

    PIOB->PIO_ABSR |= PIO_ABSR_P12;                       // Set PWM pin perhipheral type B , datasheet page 974

    // Set registers for PWM channel 2
    PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(1);    // Set the PWM clock rate to 84MHz (84MHz/1). Adjust DIVA for the resolution you are looking for

    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA;     // The period is left aligned, clock source as CLKA on channel 2

    PWM->PWM_CH_NUM[0].PWM_CPRD = 1024;                  // Channel 2 : Set the PWM frequency (84MHz/1)/PWM_CPRD = 100KHz ;

    PWM->PWM_CH_NUM[0].PWM_CDTY = 300;                  // Channel 2: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % = 50%;

    PWM->PWM_ENA = PWM_ENA_CHID0;

    // Alternately, you can use this format :  REG_PWM_CPRD2 = 840;
}

void pwm_test_init() {
    PMC->PMC_WPMR = PASSWD_PMC; //disable write protection
    PMC->PMC_SCER = 0x00000700; //enable all programmable clocks
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; //enable clock on port B
    PMC->PMC_PCER0 |= PMC_PCER0_PID11; //enable clock on port A
    PMC->PMC_PCER1 |= PMC_PCER1_PID36; // enable clock on PWM
    PMC->PMC_PCER0 |= PMC_PCER0_PID13; //enable peripheral clock on port C



    //now we set this waveform out on pin 25 on port B
    PIOC->PIO_WPMR = PASSWD_PIO; //disable write protection

    PIOC->PIO_OER |= PIO_PC18; // enable output on pin 8 port A
    PIOC->PIO_PER &= ~PIO_PC18; //disable PIO control on PA8 aka enable peripheral control
    PIOC->PIO_PDR |= PIO_PC18; //set pin 19 port C as output
    PIOC->PIO_ABSR |= PIO_ABSR_P18; //enable peripheral B (pwm high) on pin


    /*
    PIOA->PIO_OER |= PIO_PB16;
    PIOA->PIO_PER &= ~PIO_PB16;
    printf("Peripheral control reg portb :%x \n\r", REG_PIOA_ABSR);
    PIOA->PIO_ABSR |= PIO_ABSR_P16;
    PIOB->PIO_PDR |= PIO_PDR_P16;
     */

    //pwm signal setup
    //PWM->PWM_WPCR = PASSWRD_PWM;
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);     // Set the PWM clock rate to 2MHz (84MHz/42)
    REG_PWM_CMR6 = (PWM_CMR_CALG | PWM_CMR_CPRE_CLKA );
    REG_PWM_CMR6 = (PWM_CMR_CPRE_MCK_DIV_32 | PWM_CMR_CPOL);
    REG_PWM_CPRD6 = 52500;
    REG_PWM_CDTY6 = DC_LO;
    REG_PWM_ENA = PWM_ENA_CHID6;
    last_dc = 2650;

}


void pwm_init(struct Pwm_config *pwm_config) {
    PMC->PMC_WPMR = PASSWD_PMC;                           // Disable write protection
    REG_PWM_WPCR = PWM_WPCR_WPCMD_Pos;
    REG_PMC_PCER1 |= (1 << (ID_PWM - 32));                // Enable PWM

    // PIOB->PIO_OER |= PIO_PB12; // enable output on pin 12 port B
    // PIOB->PIO_PER &= ~PIO_PB12; //disable PIO control on PB12 aka enable peripheral control
    PIOB->PIO_PDR |= PIO_PB12; //set pin 12 port B as output
    PIOB->PIO_ABSR |= PIO_ABSR_P12; //enable peripheral B (pwm high) on pin

    REG_PIOB_ABSR |= PIO_ABSR_P16;                        // Set PWM pin perhipheral type A or B, in this case B
    REG_PIOB_PDR |= PIO_PDR_P16;                          // Set PWM pin to an output

    REG_PWM_SCM |= PWM_SCM_SYNC0;
    REG_PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);     // Set the PWM clock rate to 2MHz (84MHz/42)
    REG_PWM_CMR0 = PWM_CMR_CALG | PWM_CMR_CPRE_CLKA;      // Enable dual slope PWM and set the clock source as CLKA
    REG_PWM_SCM |= PWM_SCM_UPDM_MODE0;
    REG_PWM_CMPV0 = 0;
    REG_PWM_CMPM0 = PWM_CMPM_CEN;
    REG_PWM_CPRD0 = pwm_config->frequency;                // Set the PWM frequency 2MHz/(2 * 20000) = 50Hz
    REG_PWM_CDTY0 = pwm_config->duty_cycle;               // Set the PWM duty cycle to 1500 - centre the servo
    REG_PWM_ENA = PWM_ENA_CHID0;                          // Enable the PWM channel
    REG_PWM_SCUC |= PWM_SCUC_UPDULOCK;
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
