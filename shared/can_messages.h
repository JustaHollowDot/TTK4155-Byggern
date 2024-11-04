#ifndef BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H
#define BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H

#include <stdint.h>
#include <stdbool.h>

#define JOYSTICK_ID 0x01


struct JoyStickMessage {
    uint16_t id;
    uint8_t length;
    union {
        uint8_t data[8];
        struct {
            uint8_t x;
            uint8_t y;
            uint16_t distance;
            uint16_t angle;
            uint8_t direction;
            bool button_pressed;
        };
    };
};

struct SliderMessage {
    uint16_t id;
    uint8_t length;
    union {
        uint8_t data[8];
        struct {
            uint8_t x;
            uint8_t y;
        };
    };
};

typedef struct Byte8 Byte8;
struct Byte8 {
    uint8_t bytes[8];
};

struct Message {
    uint16_t id;
    uint8_t length;
    union {
        uint8_t     data[8];
        uint32_t    dword[2];
        Byte8       byte8;
    };
};

struct Test {
    union {
        struct Message message;
        struct JoyStickMessage joyStickMessage;
        struct SliderMessage sliderMessage;
    };
};

// struct JoyStickMessage create_joy_stick_message(uint8_t x, uint8_t y, uint16_t distance, uint16_t angle, uint8_t direction, bool button_pressed);

#endif //BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H
