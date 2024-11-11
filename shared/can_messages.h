#ifndef BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H
#define BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H

#include <stdint.h>
#include <stdbool.h>

#define CAN_MESSAGE_LENGTH 8
#define MESSAGE_ID 0x00
#define JOYSTICK_ID 0x01
#define SLIDER_ID 0x02


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

struct CanMessage {
    union {
        struct Message message;
        struct JoyStickMessage joyStickMessage;
        struct SliderMessage sliderMessage;
    };
};

struct JoyStickMessage create_joy_stick_message(uint8_t x, uint8_t y, uint16_t distance, uint16_t angle, uint8_t direction, bool button_pressed);
void print_joy_stick_message(struct JoyStickMessage *message);

struct SliderMessage create_slider_message(uint8_t x, uint8_t y);
void print_slider_message(struct SliderMessage *message);

struct Message create_message(uint16_t id, uint8_t length, uint8_t *data);
void print_message(struct Message *message);

#endif //BYGGERN_PING_PONG_KOPI_CAN_MESSAGES_H
