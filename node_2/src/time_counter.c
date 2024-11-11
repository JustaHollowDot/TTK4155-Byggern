#include "time_counter.h"
#include "sam/sam3x/include/sam.h"
#include "sam/sam3x/source/system_sam3x.h"
#include "stdio.h"
#include "time.h"

#define WAVEFORM_CONFIG 0x0009c002
#define PASSWD 0x54494D00
#define PASSWD_PIO 0x50494f00
#define PASSWD_PMC 0x504d4300

/*
void tc_init(){
    //first we enable thje peripheral clock for TC0
    PMC->PMC_WPMR = PASSWD_PMC; //disable write protection
    PMC->PMC_SCER = 0x00000700; //enable all programmable clocks
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; //enable clock on port B
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // enable clock on TC0

    //now we set this waveform out on pin 25 on port B
    PIOB->PIO_WPMR = PASSWD_PIO; //disable write protection
    //PIOB->PIO_OER |= PIO_PB25;
    PIOB->PIO_PDR |= PIO_PB15;
    PIOB->PIO_PER &= ~PIO_PB25; //disable PIO control on PB25 aka enable peripheral control
    PIOB->PIO_ABSR |=  PIO_PB25; //enable peripheral B (tioa0) on pin 25 port B


    REG_TC0_WPMR = PASSWD; //turn off write protection with password "TIM"
    REG_TC0_CCR0 = 0x00000000; //enable clock on tc0 channel 0
    REG_TC0_CCR0 = 0x00000001;
    REG_TC0_CMR0 = (TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);
    REG_TC0_RB0 = 0x8000;
    REG_TC0_RA0 = 0x6c66; //TIOA sets on Ra compare, which now happens at abt 18.5 ms (277450)
    REG_TC0_RC0 = 0x7530; // Set Rc aka period to 20 ms (number is 30 000)


} */

void tc_init(){
    //first we enable thje peripheral clock for TC0
    PMC->PMC_WPMR = PASSWD_PMC; //disable write protection
    PMC->PMC_SCER = 0x00000700; //enable all programmable clocks
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; //enable clock on port B
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // enable clock on TC0

    //now we set this waveform out on pin 25 on port B
    PIOB->PIO_WPMR = PASSWD_PIO; //disable write protection
    //PIOB->PIO_OER |= PIO_PB25;
    PIOB->PIO_PDR |= PIO_PB25;
    //PIOB->PIO_PER &= ~PIO_PB25; //disable PIO control on PB25 aka enable peripheral control
    PIOB->PIO_ABSR |=  PIO_PB25; //enable peripheral B (tioa0) on pin 25 port B


    REG_TC0_WPMR = PASSWD; //turn off write protection with password "TIM"
    TC0->TC_CHANNEL[0].TC_CMR &= ~(TC_CMR_WAVE); //enable capture mode
    // TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK2 | TC_CMR_CPCTRG; //set mck / 32 and enable interrupt on rc compare
    TC0->TC_CHANNEL[0].TC_CMR = (TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);
    // TC0->TC_CHANNEL[0].TC_RA = 0x6c66; // TIOA sets on RA compare, which now happens at about 18.5 ms (277450)
    // TC0->TC_CHANNEL[0].TC_RB = 0x8000;
    TC0->TC_CHANNEL[0].TC_RC = 52500; // Set RC aka period to 20 ms
    TC0->TC_CHANNEL[0].TC_CCR  = TC_CCR_CLKEN | TC_CCR_SWTRG;

    //REG_TC0_CCR0 = 0x00000000; //enable clock on tc0 channel 0
    /* REG_TC0_CCR0 = 0x00000001;
    REG_TC0_CMR0 = (TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);
    REG_TC0_RB0 = 0x8000;
    REG_TC0_RA0 = 0x6c66; //TIOA sets on Ra compare, which now happens at abt 18.5 ms (277450)
    REG_TC0_RC0 = 0x7530; // Set Rc aka period to 20 ms (number is 30 000) */
}


/*
void tc_init() {
    // First, we enable the peripheral clock for TC0
    PMC->PMC_WPMR = PASSWD_PMC; // Disable write protection
    PMC->PMC_SCER = 0x00000700; // Enable all programmable clocks
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; // Enable clock on port B
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // Enable clock on TC0

    // Now we set this waveform out on pin 25 on port B
    PIOB->PIO_WPMR = PASSWD_PIO; // Disable write protection
    PIOB->PIO_PDR |= PIO_PB15;
    PIOB->PIO_PER &= ~PIO_PB25; // Disable PIO control on PB25 aka enable peripheral control
    PIOB->PIO_ABSR |= PIO_PB25; // Enable peripheral B (TIOA0) on pin 25 port B

    TC0->TC_CHANNEL[0].TC_WPMR = PASSWD; // Turn off write protection with password "TIM"
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN; // Enable clock on TC0 channel 0
    TC0->TC_CHANNEL[0].TC_CMR = (TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);
    TC0->TC_CHANNEL[0].TC_RA = 0x6c66; // TIOA sets on RA compare, which now happens at about 18.5 ms (277450)
    TC0->TC_CHANNEL[0].TC_RB = 0x8000;
    TC0->TC_CHANNEL[0].TC_RC = 0x7530; // Set RC aka period to 20 ms (number is 30,000)
}*/


void time_counter_init(struct TimeCounter *time_counter) {
    /*
    // set mode to 0
    REG_TC0_CMR0 |= TC_CMR_WAVE;

    // set waveform selection to UP mode
    REG_TC0_CMR0 |= TC_CMR_WAVSEL_UP_RC;

    // set clock source to MCK/8
    REG_TC0_CMR0 |= TC_CMR_TCCLKS_TIMER_CLOCK2;

    REG_TC0_RA0 = time_counter->ra;
    REG_TC0_RB0 = time_counter->rb;
    REG_TC0_RC0 = time_counter->rc;

    // PIOB->PIO_OER |= PIO_PB26;
    // PIOB->PIO_ABSR |= PIO_PB25B_TIOA0;

    PIOB->PIO_ABSR &= ~PIO_PB26B_TCLK0;

    // enable peripheral clock
    PMC->PMC_PCER0 |= PMC_PCER0_PID27;
     */


    /*
    *TC1_CCR = TC_CLKDIS; // Disable the Clock Counter
    *PIO_PDR = (1 << PIOTIOA1) | (1 << PIOTIOB1); // Define TIOA1 and TIOB1 as peripheral

    PMC->PMC_PCER0 |= PMC_PCER0_PID27; //TC0 CH0

    // PIOB->PIO_PDR |= PIO_PDR_P25; //Disable GPIO on B.25
    PIOB->PIO_ABSR |= PIO_ABSR_P25;
     */

    PMC->PMC_WPMR = PASSWD_PMC; //disable write protection
    PMC->PMC_SCER = 0x00000700; //enable all programmable clocks
    PMC->PMC_PCER0 |= PMC_PCER0_PID12; //enable clock on port B
    PMC->PMC_PCER0 |= PMC_PCER0_PID27; // enable clock on TC0

    //now we set this waveform out on pin 25 on port B
    PIOB->PIO_WPMR = PASSWD_PIO; //disable write protection
    //PIOB->PIO_OER |= PIO_PB25;
    PIOB->PIO_PDR |= PIO_PB15;
    PIOB->PIO_PER &= ~PIO_PB25; //disable PIO control on PB25 aka enable peripheral control
    PIOB->PIO_ABSR |=  PIO_PB25; //enable peripheral B (tioa0) on pin 25 port B


    REG_TC0_WPMR = PASSWD; //turn off write protection with password "TIM"
    REG_TC0_CCR0 = 0x00000000; //enable clock on tc0 channel 0
    REG_TC0_CCR0 = 0x00000001;
    // REG_TC0_CMR0 = (TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR | TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK3);

    TC0->TC_CHANNEL[0].TC_CMR =
            TC_CMR_TCCLKS_TIMER_CLOCK1 |
            TC_CMR_BURST_NONE |
            TC_CMR_EEVTEDG_NONE |
            TC_CMR_EEVT_XC0 |
            TC_CMR_WAVSEL_UP_RC |
            TC_CMR_WAVE |
            TC_CMR_ACPA_SET |
            TC_CMR_ACPC_CLEAR |
            TC_CMR_ASWTRG_NONE |
            TC_CMR_AEEVT_NONE |
            TC_CMR_BCPB_NONE |
            TC_CMR_BCPC_NONE |
            TC_CMR_BSWTRG_NONE ;

    REG_TC0_RB0 = 0x8000;
    REG_TC0_RA0 = 0x6c66; //TIOA sets on Ra compare, which now happens at abt 18.5 ms (277450)
    REG_TC0_RC0 = 0x7530; // Set Rc aka period to 20 ms (number is 30 000)
}

void time_counter_set_duty_cycle(struct TimeCounter *time_counter, float duty_cycle) {
    time_counter->ra = time_counter->rc - (time_counter->rc * duty_cycle);
    time_counter->rb = time_counter->ra;

    REG_TC0_RA0 = time_counter->ra;
    REG_TC0_RB0 = time_counter->rb;
}

void time_counter_set_frequency(struct TimeCounter *time_counter, uint64_t frequency) {
    time_counter->rc = frequency / 32;
    REG_TC0_RC0 = time_counter->rc;

    // bound ra and rb to be in range of 0 to rc
    if (time_counter->ra > time_counter->rc) {
        time_counter->ra = time_counter->rc;
    }
    if (time_counter->rb > time_counter->rc) {
        time_counter->rb = time_counter->rc;
    }

    REG_TC0_RA0 = time_counter->ra;
    REG_TC0_RB0 = time_counter->rb;
}

uint32_t time_counter_get_duty_cycle(struct TimeCounter *time_counter) {
    return 0;
}
