#ifndef TEXT_H
#define TEXT_H

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum class menu_types
{
    CLOSED,
    ENERGY_MENU,
    EXIT
};


class Text
{
    TTF_Font* font;
    SDL_Color White = {255, 255, 255};
    SDL_Color Red = {255, 0, 0};
    void draw_text_to_menu(char* text, int which_option, int options, bool first);
    public:
    void interact(menu_types in_menu);
    int pointer_y = 1;
    SDL_Texture* create_font(const char* test, bool warning);
    int load_font();
    void show_menu(menu_types menu_type);
};

#endif
