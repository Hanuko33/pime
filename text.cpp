#include "text.h"
#include "window.h"
int Text::load_font()
{
    font = TTF_OpenFont("nerdfont.ttf", 500);
    if (!(font)) return 1;
    else return 0;
}
SDL_Texture* Text::create_font(const char* text, bool warning)
{
    if (!(font)) abort();
    SDL_Surface* surface;
    if (!(warning))
    {
        surface = TTF_RenderText_Solid(font, text, White);
    }
    else
    {
        surface = TTF_RenderText_Solid(font, text, Red);
    }

    SDL_Texture* text_sdl = SDL_CreateTextureFromSurface(renderer, surface);
   
    SDL_FreeSurface(surface);
    return text_sdl;
}
