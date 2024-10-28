#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "sam/sam3x/source/system_sam3x.h"
#include "uart.h"
#include "can.h"
#include "time.h"

#define F_CPU 84000000

//Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
//#include "uart.h"

int main()
{
    SystemInit();

    // brp = F_CPU / 2000000 - 1 = 41
    // tq = 2 * (41 + 1) / 84000000 = 0.0000005 = 0.5us
    can_init((CanInit){.brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 2, .propag = 6}, 0);

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // enable output on pb 13
    // PIOB->PIO_OER |= PIO_PB13;

    // configure_uart();
    uart_init(84000000, 4800);
    printf("Hello World\n\r");

    while (1)
    {
        // receive can message
        CanMsg msg;
        uint8_t result = can_rx(&msg);

        if (!result)
        {
            printf("No message received\n\r");
        } else {
            // print can message
            can_printmsg(msg);
            printf("\n\r");
        }

        time_spinFor(msecs(3000));
    }
    
}