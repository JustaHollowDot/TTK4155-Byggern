#include <stdio.h>
#include <stdarg.h>
#include "sam/sam3x/include/sam.h"
#include "sam/sam3x/source/system_sam3x.h"
#include "time.h"
#include "uart.h"
#include "can.h"
#include "time.h"
#include "pwm.h"
#include "time_counter.h"
#include "servo.h"
#include "time_counter.h"


#define F_CPU 84000000

//Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
//#include "uart.h"

int main()
{
    SystemInit();
    uart_init(84000000, 9600);

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    tc_init();

    struct TimeCounter time_counter = {
            .ra = 0,
            .rb = 0,
            .rc = 52500
    };
    time_counter_set_duty_cycle(&time_counter, 0.3f);

    for ( ; ; ) {
    }



    time_counter_init(&time_counter);
    
    // time_counter_set_frequency(&time_counter, msecs(20));
    // time_counter_set_duty_cycle(&time_counter, 0.5);


    can_init((CanInit){.brp = 42 - 1, .phase1 = 7-1, .phase2 = 6-1, .propag = 2-1}, 1);

    // enable output on pb 13
    // PIOB->PIO_OER |= PIO_PB13;

    // configure_uart;
    printf("Hello World\n\r");

    // send can message
    CanMsg send_msg = {
        .id = 0x1,
        .length = 8,
        .byte = {1, 2, 3, 4, 5, 6, 7, 8}
    };

    while (1)
    {
        // print TC counter value

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