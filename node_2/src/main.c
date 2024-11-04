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

    can_init((CanInit){.brp = F_CPU/2000000-1, .phase1 = 5, .phase2 = 1, .propag = 6}, 0);


    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    // enable output on pb 13
    // PIOB->PIO_OER |= PIO_PB13;

    // configure_uart();
    uart_init(84000000, 4800);
    printf("Hello World\n\r");

    /*
    PMC->PMC_PCER0 = ID_PIOA;
    PMC->PMC_PCER0 = ID_PIOB;
    PMC->PMC_PCER0 = ID_PIOC;
    PMC->PMC_PCER0 = ID_PIOD;
     */
    PMC->PMC_PCER0 = ID_CAN0;


    // send can message
    CanMsg send_msg = {
        .id = 0x0,
        .length = 8,
        .byte = {1, 2, 3, 4, 5, 6, 7, 8}
    };

    while (1)
    {
        /*
        // send can message
        printf("Sending message\n\r");
        can_tx(send_msg);
        printf("Message sent\n\r");
         */

        // time_spinFor(msecs(3000));

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

        time_spinFor(msecs(100));
    }
    
}