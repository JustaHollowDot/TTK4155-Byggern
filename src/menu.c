#include <stdlib.h>
#include "menu.h"

void menu_update(struct Menu *menu, struct JoyStick *joyStick, uint8_t *current_menu_index) {
    /*
    if (joyStick->current_angle == UP) {
        uint8_t tmp = *current_menu_index;
        tmp += 1;
        *current_menu_index = tmp;
    } else if (joyStick->current_angle == DOWN) {
        uint8_t tmp = *current_menu_index;
        tmp -= 1;
        *current_menu_index = tmp;
    }
     */

    printf("current_index: %d \n", (int) *current_menu_index);
}

struct Menu *menu_add_sub_menu(struct Menu *parent_menu, char *text, void (*function)()) {
    struct Menu* sub_menu = malloc(sizeof(struct Menu));
    sub_menu->text = text;
    sub_menu->function = function;
    sub_menu->parent_menu = parent_menu;

    for (int i = 0; i < MAX_SUBMENUS; ++i) {
        if (parent_menu->sub_menu[i] == NULL) {
            parent_menu->sub_menu[i] = sub_menu;
            break;
        }
    }

    return sub_menu;
}

void menu_print_current_menu(struct Oled *oled, struct Menu *menu, uint8_t current_menu_index) {
    uint8_t start, end;
    if (current_menu_index <= 5) {
        start = 0;
        end = 6;
    } else {
        start = current_menu_index - 5;
        end = current_menu_index + 1;
    }

    oled_write_line(oled, " |./ (parent dir)");

    for (int i = start; i < end; ++i) {
        if (menu->sub_menu[i] == NULL) {
            break;
        }

        if (i == current_menu_index) {
            oled_write_string(oled, "*|");
        } else {
            oled_write_string(oled, " |");
        }

        oled_write_line(oled, menu->sub_menu[i]->text);
    }
}


