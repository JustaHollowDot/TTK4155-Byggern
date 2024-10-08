#ifndef BYGGERN_PING_PONG_MENU_H
#define BYGGERN_PING_PONG_MENU_H

#include "avr/io.h"
#include <stdbool.h>

#include "./peripherals/joystick/joystick.h"

#define MAX_SUBMENUS 10

struct Menu {
    char* text;
    void (*function)();
    struct Menu* parent_menu;
    struct Menu* sub_menu[MAX_SUBMENUS];
};

void menu_update(struct Menu *menu, struct JoyStick *joyStick);

struct Menu* menu_add_sub_menu(struct Menu* parent_menu, char* text, void (*function)());



#endif //BYGGERN_PING_PONG_MENU_H
