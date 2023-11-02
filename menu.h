#ifndef MENU_H
#define MENU_H

#include "text.h"

enum menu_types
{
    MENU_CLOSED,
    MENU_ENERGY,
    MENU_EXIT,
    MENU_HELP
};
extern enum menu_types in_menu;
extern int menu_pos;

void interact();
void show_menu(enum menu_types menu_type);
void draw_text_to_menu(const char* text, int which_option, int options, char first);

#endif

