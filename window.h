#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

extern SDL_Renderer *renderer;
extern SDL_Window *main_window;

extern int window_width;
extern int window_height;

int init_window();
void clear_window();
unsigned int color(int r, int g, int b, int a);
#define GAME_WINDOW 512
#define PANEL_WINDOW 512
#endif // WINDOW_H
