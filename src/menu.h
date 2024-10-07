#ifndef BYGGERN_PING_PONG_MENU_H
#define BYGGERN_PING_PONG_MENU_H

#include "avr/io.h"
#include <stdbool.h>

#include "./peripherals/joystick/joystick.h"
#include "./peripherals/oled/oled.h"

#define MAX_SUBMENUS 10

struct Menu {
    char* text;
    void (*function)();
    struct Menu* parent_menu;
    struct Menu* sub_menu[MAX_SUBMENUS];
};

void menu_update(struct Menu *menu, struct JoyStick *joyStick, uint8_t *current_menu_index);
void menu_print_current_menu(struct Oled *oled, struct Menu *menu, uint8_t current_menu_index);

struct Menu* menu_add_sub_menu(struct Menu* parent_menu, char* text, void (*function)());



#endif //BYGGERN_PING_PONG_MENU_H
