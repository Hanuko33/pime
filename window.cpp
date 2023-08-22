#include "window.h"

SDL_Renderer *renderer;
SDL_Window *main_window;

int  init_window()
{
	Uint32 flags;
    flags = SDL_WINDOW_RESIZABLE;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) { fprintf(stderr, "\nUnable to initialize SDL:  %s\n", SDL_GetError()); }

    if (SDL_CreateWindowAndRenderer(0, 0, flags, &main_window, &renderer) < 0) {
        SDL_Log("SDL_CreateWindowAndRenderer() failed: %s\n", SDL_GetError());
        return 1;
	}
    SDL_SetWindowTitle(main_window, "game_window");
//    app_surface = SDL_GetWindowSurface(main_window);
    SDL_SetWindowPosition(main_window, 0,0);
    SDL_SetWindowSize(main_window, 600, 600);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) 
    {
        printf("\nUnable to initialize sdl_image:  %s\n", IMG_GetError());
        return 1;
    }
    TTF_Init();

    return 0;
}

void clear_window()
{
     SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

unsigned int color(int r, int g, int b, int a) {
	return (a << 24) | (b << 16) | (g << 8) | r;
}

