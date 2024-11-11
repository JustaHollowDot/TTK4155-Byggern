#ifndef NODE_2_IR_BEAM_H
#define NODE_2_IR_BEAM_H

#include "sam/sam3x/include/sam.h"
#include <stdint.h>
#include "stdbool.h"

struct IR_beam {
    bool beam_blocked;
    uint64_t beam_blocked_start_time;
};

void IR_beam_init(struct IR_beam *ir_beam);

#endif //NODE_2_IR_BEAM_H
