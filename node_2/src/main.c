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
#include "IR_beam.h"
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

    can_init((CanInit){.brp = 42 - 1, .phase1 = 7-1, .phase2 = 6-1, .propag = 2-1}, 0);

    struct Servo servo;
    servo_init(&servo, 0, 0, usecs(20000), usecs(900), usecs(2100));

    struct IR_beam ir_beam;
    IR_beam_init(&ir_beam);

    struct JoyStickMessage joyStickMessage;

    // enable pin pb14 as digital input

    uint64_t start_time = time_now();

    while (1)
    {
        if (!(REG_PIOB_PDSR & PIO_PDSR_P14)) {
            if (!ir_beam.beam_blocked) {
                printf("%s: %d -> IR beam blocked\n\r", __FILE__, __LINE__);
                ir_beam.beam_blocked = true;
                ir_beam.beam_blocked_start_time = time_now();
            }
        } else {
            if (ir_beam.beam_blocked && totalMsecs(time_now() - ir_beam.beam_blocked_start_time) > 10) {
                printf("%s: %d -> IR beam blocked for %f seconds\n\r", __FILE__, __LINE__, totalSeconds(time_now() - ir_beam.beam_blocked_start_time));
                printf("%s: %d -> You survived for %f seconds\n\r", __FILE__, __LINE__, totalSeconds(time_now() - start_time));
                ir_beam.beam_blocked = false;
                start_time = time_now();
            }
        }

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

            // print_joy_stick_message(&joyStickMessage);

            // printf("%s: %d -> Setting servo angle to: %d\n\r", __FILE__, __LINE__, joyStickMessage.angle);
            // set servo angle
            if (joyStickMessage.angle > 180) {
                joyStickMessage.angle = 180;
            }

            servo_set_angle(&servo, joyStickMessage.angle);
        }
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
