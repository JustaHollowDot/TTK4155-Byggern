#include "button.h"

#define BUTTON_0 PE2 // todo! give descriptive names
#define BUTTON_1 PE0

void buttons_init() {
    DDRE &= ~(1 << PE2);
    DDRE &= ~(1 << PE0);
}

