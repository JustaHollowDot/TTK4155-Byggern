
#include <stdlib.h>
#include <avr/io.h>
#include <stdio.h>

#include "sram.h"


void EXMEM_init() {
    MCUCR = (1 << SRE); // Enable SRAM i ATmega162
    SFIOR = (1 << XMM2); // Maskerer PC4-PC7 på ATmega162. Dvs passer på at vi ikke kuker med JTAG-pinnene. Sjekk "Table 4", side 32 i ATmega162-databladet.
}