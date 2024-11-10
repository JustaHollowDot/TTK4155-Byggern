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
    printf("\r\n\r\n");
    printf("%s : %d -> Hello World\n\r", __FILE__, __LINE__);
    printf("\r\n");

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    struct Servo servo;
    servo_init(&servo, 0, 0, usecs(20000), usecs(900), usecs(2100));



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