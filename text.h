#ifndef TEXT_H
#define TEXT_H

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font* font;
extern SDL_Color White;
extern SDL_Color Red;


SDL_Texture* create_font(const char* text, char  warning);
void write_text(int x, int y, const char * text, SDL_Color color); 
int load_font();


#endif
