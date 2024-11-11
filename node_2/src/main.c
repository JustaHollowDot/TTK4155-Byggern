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
#include "../../shared/can_messages.h"


#define F_CPU 84000000

//Import UART from Node 2 starter code, then edit include path accordingly. Also, remember to update the makefile
//#include "uart.h"

int main()
{
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    uart_init(84000000, 9600);

    printf("\r\n\r\n");
    printf("%s : %d -> Hello World\n\r", __FILE__, __LINE__);
    printf("\r\n");

    can_init((CanInit){.brp = 42 - 1, .phase1 = 7-1, .phase2 = 6-1, .propag = 2-1}, 1);


    struct Servo servo;
    servo_init(&servo, 0, 0, usecs(20000), usecs(900), usecs(2100));

    // send can message
    CanMsg send_msg = {
        .id = 0x1,
        .length = 8,
        .byte = {1, 2, 3, 4, 5, 6, 7, 8}
    };

    struct JoyStickMessage joyStickMessage;

    while (1)
    {
        printf("%s : %d -> Setting servo to 0 degrees\n\r", __FILE__, __LINE__);
        servo_set_angle(&servo, 0);
        time_spinFor(msecs(1000));

        printf("%s : %d -> Setting servo to 90 degrees\n\r", __FILE__, __LINE__);
        servo_set_angle(&servo, 90);
        time_spinFor(msecs(1000));

        printf("%s : %d -> Setting servo to 180 degrees\n\r", __FILE__, __LINE__);
        servo_set_angle(&servo, 180);
        time_spinFor(msecs(1000));


        // send can message
        // printf("Sending message\n\r");
        // can_tx(send_msg);
        // printf("Message sent\n\r");

        // time_spinFor(msecs(3000));

        // receive can message

        CanMsg msg;
        uint8_t result = can_rx(&msg);

        struct CanMessage canMessage;
        canMessage.message.id = msg.id;
        canMessage.message.length = msg.length;
        for (uint8_t i = 0; i < msg.length; i++)
        {
            canMessage.message.data[i] = msg.byte[i];
        }

        joyStickMessage = canMessage.joyStickMessage;

        if (result) {
            // print can message
            // can_printmsg(msg);

            print_joy_stick_message(&joyStickMessage);
            printf("\n\r");
        } else {
            printf("No message received\n\r");
        }

        time_spinFor(msecs(1000));
    }
}

void print_joy_stick_message(struct JoyStickMessage *message) {
    printf("%s: %d -> Joystick message: \n\r", __FILE__, __LINE__);
    printf("%s: %d -> \tID: %d \n\r", __FILE__, __LINE__, message->id);
    printf("%s: %d -> \tLength: %d \n\r", __FILE__, __LINE__, message->length);
    printf("%s: %d -> \tX: %d \n\r", __FILE__, __LINE__, message->x);
    printf("%s: %d -> \tY: %d \n\r", __FILE__, __LINE__, message->y);
    printf("%s: %d -> \tDistance: %d \n\r", __FILE__, __LINE__, message->distance);
    printf("%s: %d -> \tAngle: %d \n\r", __FILE__, __LINE__, message->angle);
    printf("%s: %d -> \tDirection: %d \n\r", __FILE__, __LINE__, message->direction);
    printf("%s: %d -> \tButton pressed: %s \n\r", __FILE__, __LINE__, message->button_pressed ? "true" : "false");
}
