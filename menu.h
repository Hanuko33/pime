#ifndef MENU_H
#define MENU_H

#include "text.h"

enum menu_actions
{
    MENU_CANCEL,
    MENU_SAVE_EXIT,
    MENU_EXIT,
    MENU_SAVE,
    MENU_LOAD,
    MENU_HELP,
    MENU_REGAIN,
    MENU_BOOST,
    MENU_LOUDER,
    MENU_QUIETER
};


struct menu_struct
{
    int options;
    int menu_pos;
    int added;
    const char ** entries;
    enum menu_actions * actions;
};

extern enum menu_types in_menu;
extern struct menu_struct menu_main;
extern struct menu_struct menu_energy;
extern struct menu_struct menu_help;
extern struct menu_struct * current_menu;

int interact();
void show_menu();
void create_menus();
int  menu_interact(int key);

#endif

