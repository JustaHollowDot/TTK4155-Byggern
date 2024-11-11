#include "IR_beam.h"


void IR_beam_init(struct IR_beam *ir_beam) {
    REG_PIOB_PER |= PIO_PER_P14;
    REG_PIOB_ODR |= PIO_ODR_P14;
    REG_PIOB_IFER |= PIO_IFER_P14;
    ir_beam->beam_blocked = false;
    ir_beam->beam_blocked_start_time = 0;
}