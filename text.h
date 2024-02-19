#ifndef TEXT_H
#define TEXT_H

#include "player.h"
#include <stdio.h>
#include <SDL2/SDL_ttf.h>

extern TTF_Font* font;
extern SDL_Color White;
extern SDL_Color Red;
extern SDL_Color Gray;
extern SDL_Color Cyan;
extern SDL_Color Yellow;

void write_text(int x, int y, const char * text, SDL_Color color, int scale_x, int scale_y);
int load_font();

extern char status_line[256];
extern int status_code;
#endif
