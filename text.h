#ifndef TEXT_H
#define TEXT_H

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum menu_types
{
    MENU_CLOSED,
    MENU_ENERGY,
    MENU_EXIT,
    MENU_HELP
};
extern enum menu_types in_menu;

extern TTF_Font* font;
extern int menu_pos;
extern SDL_Color White;
extern SDL_Color Red;

void interact();

SDL_Texture* create_font(const char* text, char  warning);
void write_text(int x, int y, const char * text, SDL_Color color); 
int load_font();

void show_menu(enum menu_types menu_type);
void draw_text_to_menu(const char* text, int which_option, int options, char first);

#endif
