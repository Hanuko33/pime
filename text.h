#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include <SDL2/SDL_ttf.h>
class Text
{
    TTF_Font* font;
    SDL_Color White = {255, 255, 255};
    SDL_Color Red = {255, 0, 0};
    public:
    SDL_Texture* create_font(const char* test, bool warning);
    int load_font();
};

#endif
