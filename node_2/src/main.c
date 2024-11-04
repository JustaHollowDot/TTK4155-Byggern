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

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // uint8_t brp = 1.25 * 10^-6 - 1;
    uint8_t brp = 42 - 1;
    can_init((CanInit){.brp = brp, .phase1 = 7-1, .phase2 = 6-1, .propag = 2-1}, 1);

    // enable output on pb 13
    // PIOB->PIO_OER |= PIO_PB13;

    // configure_uart;
    uart_init(84000000, 9600);
    printf("Hello World\n\r");

    /*
    PMC->PMC_PCER0 = ID_PIOA;
    PMC->PMC_PCER0 = ID_PIOB;
    PMC->PMC_PCER0 = ID_PIOC;
    PMC->PMC_PCER0 = ID_PIOD;
     */


    // send can message
    CanMsg send_msg = {
        .id = 0x1,
        .length = 8,
        .byte = {1, 2, 3, 4, 5, 6, 7, 8}
    };

    while (1)
    {

        // send can message
        // printf("Sending message\n\r");
        // can_tx(send_msg);
        // printf("Message sent\n\r");

        // time_spinFor(msecs(3000));

        // receive can message
        /*
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
         */

        time_spinFor(msecs(1000));
    }
}