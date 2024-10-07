#include <stdlib.h>
#include "menu.h"

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

