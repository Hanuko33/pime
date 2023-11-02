#ifndef TEXT_H
#define TEXT_H

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

enum class menu_types
{
    CLOSED,
    ENERGY_MENU,
    EXIT,
    HELP
};
extern menu_types in_menu;

class Text
{
    TTF_Font* font;
    void draw_text_to_menu(const char* text, int which_option, int options, bool first);
    public:
    SDL_Color White = {255, 255, 255};
    SDL_Color Red = {255, 0, 0};
    void interact();
    int menu_pos = 0;
    SDL_Texture* create_font(const char* text, bool warning);
    int load_font();
    void show_menu(menu_types menu_type);
	void write_text(int x, int y, const char * text, SDL_Color color); 
};

#endif
