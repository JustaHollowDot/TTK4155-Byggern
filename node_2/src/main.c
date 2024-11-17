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

uint32_t get_position() {
    return TC2->TC_CHANNEL[0].TC_CV;
}

void reset_position() {
    TC2->TC_CHANNEL[0].TC_CV = 0;
}

void activate_solenoid() {
    PIOC->PIO_CODR |= PIO_PC12;
    time_spinFor(msecs(30));
    PIOC->PIO_SODR |= PIO_PC12;
}

int main() {
    SystemInit();
    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PMC->PMC_PCER0 |= (1 << ID_PIOA);
    PMC->PMC_PCER0 |= (1 << ID_PIOB);
    PMC->PMC_PCER0 |= (1 << ID_PIOC);
    PMC->PMC_PCER0 |= (1 << ID_PIOD);


    uart_init(84000000, 9600);

    printf("\r\n\r\n");
    printf("%s : %d -> Hello World\n\r", __FILE__, __LINE__);
    printf("\r\n");

    can_init((CanInit){.brp = 42 - 1, .phase1 = 7-1, .phase2 = 6-1, .propag = 2-1}, 0);

    struct Servo servo;
    servo_init(&servo, 0, 0, usecs(20000), usecs(900), usecs(2100));
    servo_set_angle(&servo, 90);

    tc2_init();

    struct IR_beam ir_beam;
    IR_beam_init(&ir_beam);

    struct JoyStickMessage joyStickMessage = {
            .id = JOYSTICK_ID,
            .length = 8,
            .x = 0,
            .y = 0,
            .distance = 0,
            .angle = 0,
            .direction = 0,
            .button_pressed = false
    };


    struct Pwm_config pwm_config = {
            .pin = 0,
            .channel = 0,
            .frequency = 40000,
            .duty_cycle = 0
    };

    pwm_test_2();

    // init PC23 as output
    PIOC->PIO_OER |= PIO_PC23;
    PIOC->PIO_PER |= PIO_PC23;

    // init PC12 as output
    PIOC->PIO_OER |= PIO_PC12;
    PIOC->PIO_PER |= PIO_PC12;
    PIOC->PIO_PUDR |= PIO_PC12;


    bool direction_left = true;

    uint32_t max_position = 4400;
    uint32_t min_position = 0;
    uint32_t current_position = 0;
    uint32_t target_position = 0;

    // Move motor to position 0
    direction_left = false;
    pwm_set_duty_cycle(&pwm_config, 2000);
    time_spinFor(seconds(1));
    pwm_set_duty_cycle(&pwm_config, 4000);

    // set min_position as current position
    min_position = get_position() + 10000; // prevent underflow
    max_position = min_position + 4400;

    float integral_value = 0;
    int32_t last_diff_position = 0;

    bool button_pressed = false;
    bool last_button_pressed = false;

    uint32_t i = 0;
    uint64_t start_time = time_now();
    while (1) {
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
        button_pressed = msg.byte[7] & 0x01;

        if (result) {
            // print can message
            // can_printmsg(msg);

            // print_joy_stick_message(&joyStickMessage);

            // printf("%s: %d -> Setting servo angle to: %d\n\r", __FILE__, __LINE__, joyStickMessage.angle);
            // set servo angle
            joyStickMessage.y = joyStickMessage.y * 180 / 255;
            servo_set_angle(&servo, joyStickMessage.y);
            printf("%s: %d -> Setting servo angle to: %d\n\r", __FILE__, __LINE__, joyStickMessage.y);


            // set motor position
            // target_position = 4400 * ((float) joyStickMessage.x) / 255;
            target_position = min_position + 4400 * (((float) joyStickMessage.x) / 255);
            // target_position = min_position + 2200;
        }

        if (!(REG_PIOB_PDSR & PIO_PDSR_P14)) {
            if (!ir_beam.beam_blocked) {
                printf("%s: %d -> IR beam blocked\n\r", __FILE__, __LINE__);
                ir_beam.beam_blocked = true;
                ir_beam.beam_blocked_start_time = time_now();
            }
        } else {
            if (ir_beam.beam_blocked && totalMsecs(time_now() - ir_beam.beam_blocked_start_time) > 10) {
                printf("%s: %d -> IR beam blocked for %f seconds\n\r", __FILE__, __LINE__, totalSeconds(time_now() - ir_beam.beam_blocked_start_time));
                printf("%s: %d -> You survived for %f seconds\n\r", __FILE__, __LINE__, totalSeconds(time_now() - start_time) - totalSeconds(time_now() - ir_beam.beam_blocked_start_time));
                ir_beam.beam_blocked = false;
                start_time = time_now();
            }
        }

        /*
        // set pin PC 23 to direction_left
        if (direction_left) {
            PIOC->PIO_SODR |= PIO_PC23;
        } else {
            PIOC->PIO_CODR |= PIO_PC23;
        }

        direction_left = !direction_left;

        pwm_set_duty_cycle(&pwm_config, 4000);

        time_spinFor(seconds(1));

        printf("%s: %d -> Setting direction to: %s\n\r", __FILE__, __LINE__, direction_left ? "left" : "right");

        pwm_set_duty_cycle(&pwm_config, 2000);

        time_spinFor(msecs(300));
        */

        // Move motor to target position
        current_position = get_position() + 10000;

        int32_t diff_position = target_position - current_position;
        if (diff_position < 0) {
            diff_position = -diff_position;
        }

        // proportional
        float scale = 1.0;
        float speed = 4000 * ((float) diff_position / 4400);
        speed *= scale;
        if (speed < 0) {
            speed = -speed;
        }


        // differential
        int32_t change_position = diff_position - last_diff_position;
        last_diff_position = diff_position;
        if (change_position < 0) {
            change_position = -change_position;
        }
        speed -= 3 * change_position;


        // integral
        integral_value *= 0.98;
        integral_value += 50 * ((float) diff_position / 4400);

        speed += integral_value;

        if (speed > 2000) {
            speed = 2000;
        } else if (speed < 0) {
            speed = 0;
        }


        if (i % 10 == 0) {
            printf("%s: %d -> Speed: %f\n\r", __FILE__, __LINE__, speed);
        }

        if (current_position < min_position) {
            min_position = current_position;
            max_position = min_position + 4400;
        }

        if (current_position < target_position - 75) {
            if (i % 10 == 0) {
                printf("%s: %d -> Moving motor left\n\r", __FILE__, __LINE__);
            }

            direction_left = false;
            if (direction_left) {
                PIOC->PIO_SODR |= PIO_PC23;
            } else {
                PIOC->PIO_CODR |= PIO_PC23;
            }

            pwm_set_duty_cycle(&pwm_config, 4000 - speed);
        } else if (current_position > target_position + 75) {
            if (i % 10 == 0) {
                printf("%s: %d -> Moving motor right\n\r", __FILE__, __LINE__);
            }

            direction_left = true;
            if (direction_left) {
                PIOC->PIO_SODR |= PIO_PC23;
            } else {
                PIOC->PIO_CODR |= PIO_PC23;
            }

            pwm_set_duty_cycle(&pwm_config, 4000 - speed);
        } else {
            pwm_set_duty_cycle(&pwm_config, 4000);
        }


        if (button_pressed && !last_button_pressed) {
            activate_solenoid();
            printf("%s: %d -> Solenoid activated\n\r", __FILE__, __LINE__);
            last_button_pressed = true;

        } else if (!button_pressed && last_button_pressed) {
            printf("%s: %d -> Solenoid deactivated\n\r", __FILE__, __LINE__);
            last_button_pressed = false;
        }
        
        time_spinFor(msecs(10));

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
