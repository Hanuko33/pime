#ifndef WINDOW_H
#define WINDOW_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern SDL_Renderer *renderer;
extern SDL_Window *main_window;

int init_window();
void clear_window();
unsigned int color(int r, int g, int b, int a);

#endif // WINDOW_H
